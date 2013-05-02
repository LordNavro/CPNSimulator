#include "cpnetsimulator.h"
#include "interpret.h"

CPNetSimulator::CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent) :
    QWidget(parent), net(net), editor(editor), threadComputer(net, this), transitionsToFire(0)
{
    scene = new SimulatorScene(net, this);
    view = new QGraphicsView(scene, this);

    overlay = new QWidget(this);
    overlay->hide();
    overlay->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    overlay->setAutoFillBackground(true);
    QPalette p(overlay->palette());
    p.setColor(overlay->backgroundRole(), QColor(255,255,255,200));
    overlay->setPalette(p);
    buttonCancelComputation = new QPushButton(tr("&Cancel operation"), overlay);
    labelComputationDescription = new QLabel(this);
    labelComputationDescription->setAlignment(Qt::AlignCenter);
    labelComputationDescription->setAutoFillBackground(true);
    QPalette p2(labelComputationDescription->palette());
    p2.setColor(labelComputationDescription->backgroundRole(), QColor(255,255,255,255));
    labelComputationDescription->setPalette(p2);
    connect(buttonCancelComputation, SIGNAL(clicked()), this, SLOT(slotCancelComputation()));

    QGridLayout *l = new QGridLayout(overlay);
    l->addWidget(labelComputationDescription, 0, 0,1,1,Qt::AlignHCenter | Qt::AlignBottom);
    l->addWidget(buttonCancelComputation, 1, 0,1,1,Qt::AlignHCenter | Qt::AlignTop);
    overlay->setLayout(l);

    layout = new QGridLayout(this);
    layout->addWidget(view, 0, 0);
    layout->addWidget(overlay, 0, 0);
    this->setLayout(layout);

    connect(&threadComputer, SIGNAL(signalCompleted()), this, SLOT(slotComputerCompleted()));
    connect(&threadComputer, SIGNAL(signalFailed(QString)), this, SLOT(slotComputerFailed(QString)));
}

void CPNetSimulator::loadNetGraph()
{
    qDeleteAll(scene->items());
    foreach(Place *place, net->places)
    {
        EditorPlaceItem *epi = editor->scene->getPlaceItem(place);
        SimulatorPlaceItem *spi = new SimulatorPlaceItem(epi);
        scene->addItem(spi);
        if(spi->place->currentMarkingValue)
            delete spi->place->currentMarkingValue;
        if(spi->place->parsedCurrentMarking)
            spi->place->currentMarkingValue = new Data(eval(spi->place->parsedCurrentMarking, NULL, NULL, &threadComputer));
        else
            switch(spi->place->colourSet)
            {
            case Place::UNIT:
                spi->place->currentMarkingValue = new Data(Data::MULTIUNIT);
                spi->place->currentMarkingValue->value.multiUnit = 0;
                break;
            case Place::BOOL:
                spi->place->currentMarkingValue = new Data(Data::MULTIBOOL);
                spi->place->currentMarkingValue->value.multiBool.f = 0;
                spi->place->currentMarkingValue->value.multiBool.t = 0;
                break;
            case Place::INT:
                spi->place->currentMarkingValue = new Data(Data::MULTIINT);
                break;
            }
    }
    foreach(Transition *transition, net->transitions)
    {
        EditorTransitionItem *eti = editor->scene->getTransitionItem(transition);
        SimulatorTransitionItem *sti = new SimulatorTransitionItem(eti);
        connect(sti, SIGNAL(signalFire(SimulatorTransitionItem*)), this, SLOT(slotFire(SimulatorTransitionItem *)));
        scene->addItem(sti);
    }
    foreach(Arc *arc, net->arcs)
    {
        QGraphicsItem *from = scene->getPlaceItem(arc->place);
        QGraphicsItem *to = scene->getTransitionItem(arc->transition);

        if(!arc->isPreset)
            qSwap(from, to);
        SimulatorArcItem *sai = new SimulatorArcItem(from, to);
        sai->arc = arc;
        scene->addItem(sai);
        scene->getPlaceItem(arc->place)->arcItems.append(sai);
        scene->getTransitionItem(arc->transition)->arcItems.append(sai);
    }
    findBindings();
}

void CPNetSimulator::toInitialMarking()
{
    if(threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot change to initial marking"), tr("Cannot change to initial marking while computations still running"));
        return;
    }
    foreach(Place *place, net->places)
    {
        if(place->currentMarkingValue)
            delete place->currentMarkingValue;
        if(place->parsedInitialMarking)
           place->currentMarkingValue = new Data(eval(place->parsedInitialMarking, NULL, NULL, &threadComputer));
        else
            switch(place->colourSet)
            {
            case Place::UNIT:
                place->currentMarkingValue = new Data(Data::MULTIUNIT);
                place->currentMarkingValue->value.multiUnit = 0;
                break;
            case Place::BOOL:
                place->currentMarkingValue = new Data(Data::MULTIBOOL);
                place->currentMarkingValue->value.multiBool.f = 0;
                place->currentMarkingValue->value.multiBool.t = 0;
                break;
            case Place::INT:
                place->currentMarkingValue = new Data(Data::MULTIINT);
                break;
            }
        scene->getPlaceItem(place)->update();
    }
    findBindings();
}

void CPNetSimulator::findBindings()
{
    if(threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot find binding"), tr("Cannot find binding while computations still running"));
        return;
    }
    threadComputer.mode = Computer::FindBinding;
    showOverlay(tr("Finding binding"));
    threadComputer.start();
}

void CPNetSimulator::fireTransitions(int count)
{
    if(threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot fire transitions"), tr("Cannot fire any more transitions while computations still running"));
        return;
    }
    transitionsToFire = count;
    QList<SimulatorTransitionItem *> available;
    foreach(QGraphicsItem *item, scene->items())
    {
        if(SimulatorTransitionItem *sti = qgraphicsitem_cast<SimulatorTransitionItem *>(item))
            if(sti->transition->possibleBindings.count())
                available.append(sti);
    }
    if(available.isEmpty())
        return;
    SimulatorTransitionItem *sti = available.at(CPNetSimulator::randInt(0, available.count() -1));
    sti->comboBinding->setCurrentIndex(CPNetSimulator::randInt(0, sti->comboBinding->count() - 1));
    slotFire(sti);
}

void CPNetSimulator::showOverlay(QString message)
{
    labelComputationDescription->setText(message);
    overlay->show();
    //view->hide();
}

void CPNetSimulator::hideOverlay()
{
    overlay->hide();
    //view->show();
}

void CPNetSimulator::slotFire(SimulatorTransitionItem *sti)
{
    if(threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot fire transitions"), tr("Cannot fire any more transitions while computations still running"));
        return;
    }

    threadComputer.mode = Computer::FireTransition;
    threadComputer.sti = sti;
    showOverlay(tr("Firing transition"));
    threadComputer.start();
}

void CPNetSimulator::slotCancelComputation()
{
    threadComputer.cancelRequest = true;
}

void CPNetSimulator::slotComputerCompleted()
{
    hideOverlay();
    if(threadComputer.mode == Computer::FireTransition)
    {
        findBindings();
    }
    else
    {
        foreach(QGraphicsItem *item, scene->items())
        {
            if(SimulatorTransitionItem *sti = qgraphicsitem_cast<SimulatorTransitionItem *>(item))
                sti->populateCombo();
        }
        if(--transitionsToFire > 0)
            fireTransitions(transitionsToFire);
    }
}

void CPNetSimulator::slotComputerFailed(QString message)
{
    QMessageBox::critical(this, tr("Computation failed"), message);
    if(threadComputer.mode == Computer::FireTransition)
        findBindings();
    else
    {
        foreach(QGraphicsItem *item, scene->items())
        {
            if(SimulatorTransitionItem *sti = qgraphicsitem_cast<SimulatorTransitionItem *>(item))
            {
                sti->transition->possibleBindings.clear();
                sti->populateCombo();
            }
        }
    }
    hideOverlay();
}

int CPNetSimulator::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}
