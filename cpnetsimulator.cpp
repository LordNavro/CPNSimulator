/*
 *    Copyright 2013, 2014 Ondrej Navratil.
 *    This project and all of its contents are distributed under the terms of the GNU General Public License.
 *
 *    This file is part of CPNSimulator.
 *
 *    CPNSimulator is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published b
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    CPNSimulator is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with CPNSimulator. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cpnetsimulator.h"
#include "interpret.h"
#include "statespaceexplorer.h"

CPNetSimulator::CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent) :
    QWidget(parent), net(net), editor(editor), threadComputer(net, this), transitionsToFire(0)
{
    scene = new SimulatorScene(net, this);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);

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
    toCurrentMarking();
}

void CPNetSimulator::toInitialMarking()
{
    if(threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot move to initial marking"), tr("Cannot move to initial marking while computations still running"));
        return;
    }
    threadComputer.mode = Computer::ToInitialMarking;
    showOverlay(tr("Moving to initial marking"));
    threadComputer.start();
}

void CPNetSimulator::toCurrentMarking()
{
    if(threadComputer.isRunning())
    {
        QMessageBox::information(this, tr("Cannot move to current marking"), tr("Cannot move to current marking while computations still running"));
        return;
    }
    threadComputer.mode = Computer::ToCurrentMarking;
    showOverlay(tr("Moving to current marking"));
    threadComputer.start();
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
            if(!sti->transition->possibleBindings.isEmpty())
                available.append(sti);
    }
    if(available.isEmpty())
    {
        findBindings();
        return;
    }
    SimulatorTransitionItem *sti = available.at(CPNetSimulator::randInt(0, available.count() -1));
    sti->populateCombo();
    sti->comboBinding->setCurrentIndex(CPNetSimulator::randInt(0, sti->comboBinding->count() - 1));
    slotFire(sti);
}

void CPNetSimulator::showOverlay(QString message)
{
    labelComputationDescription->setText(message);
    overlay->show();
}

void CPNetSimulator::hideOverlay()
{
    overlay->hide();
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

void CPNetSimulator::slotGenerateStateSpace()
{
    bool ok;
    int depth = QInputDialog::getInt(this, tr("Genrate state space"), tr("Enter maximum step limit for state space generation:"), 20, 1, 1000, 10, &ok);
    if(!ok)
        return;
    threadComputer.mode = Computer::GenerateStateSpace;
    threadComputer.depth = depth;
    showOverlay(tr("Generating state space"));
    threadComputer.start();
}

void CPNetSimulator::slotCancelComputation()
{
    threadComputer.cancelRequest = true;
}

void CPNetSimulator::slotComputerCompleted()
{
    threadComputer.wait(3000);
    if(threadComputer.mode == Computer::FireTransition
            || threadComputer.mode == Computer::ToCurrentMarking
            || threadComputer.mode == Computer::ToInitialMarking)
    {
        findBindings();
    }
    else if(threadComputer.mode == Computer::GenerateStateSpace)
    {
        StateSpaceExplorer *explorer = new StateSpaceExplorer(threadComputer.graph, this);
        explorer->exec();
        findBindings();
    }
    else
    {
        if(--transitionsToFire > 0 && !threadComputer.cancelRequest)
            fireTransitions(transitionsToFire);
        else
        {
            foreach(QGraphicsItem *item, scene->items())
            {
                if(SimulatorTransitionItem *sti = qgraphicsitem_cast<SimulatorTransitionItem *>(item))
                    sti->populateCombo();
            }
            hideOverlay();
            threadComputer.cancelRequest = false;
        }
    }
}

void CPNetSimulator::slotComputerFailed(QString message)
{
    threadComputer.wait(3000);
    transitionsToFire = 0;
    QMessageBox::critical(this, tr("Computation failed"), message);
    if(threadComputer.mode == Computer::ToInitialMarking || threadComputer.mode == Computer::ToCurrentMarking)
    {
        foreach(Place *place, net->places)
        {
            if(place->currentMarkingValue)
                delete place->currentMarkingValue;
            if(place->colourSet == Place::UNIT)
                place->currentMarkingValue = new Data(Data::MULTIUNIT);
            if(place->colourSet == Place::BOOL)
                place->currentMarkingValue = new Data(Data::MULTIBOOL);
            if(place->colourSet == Place::INT)
                place->currentMarkingValue = new Data(Data::MULTIINT);
        }
        QMessageBox::critical(this, tr("Warning"), tr("All markings changed to empty sets. This state may not be reachable in the original net. Return to edit mode or change to initial marking is suggested."));
        threadComputer.cancelRequest = false;
        findBindings();
    }
    else if(threadComputer.mode == Computer::FireTransition || threadComputer.mode == Computer::GenerateStateSpace)
    {
        threadComputer.cancelRequest = false;
        findBindings();
    }
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
        QMessageBox::critical(this, tr("Warning"), tr("Computation of possible bindings terminated during execution. To display enabled transitions, recompute it using the Find binding button."));
        hideOverlay();
        threadComputer.cancelRequest = false;
    }
}

int CPNetSimulator::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}
