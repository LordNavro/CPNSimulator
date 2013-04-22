#include "cpnetsimulator.h"
#include "interpret.h"

CPNetSimulator::CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent) :
    QWidget(parent), net(net), editor(editor)
{
    scene = new SimulatorScene(net, this);
    view = new QGraphicsView(scene, this);

    layout = new QGridLayout(this);
    layout->addWidget(view, 0, 0);
    this->setLayout(layout);
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
            spi->place->currentMarkingValue = new Data(eval(spi->place->parsedCurrentMarking, NULL, NULL));
        else
            switch(spi->place->colourSet)
            {
            case Place::UNIT:
                spi->place->currentMarkingValue = new Data(Data::MULTIUNIT);
                spi->place->currentMarkingValue->value.multiUnit = 0;
            case Place::BOOL:
                spi->place->currentMarkingValue = new Data(Data::MULTIBOOL);
                spi->place->currentMarkingValue->value.multiBool.f = 0;
                spi->place->currentMarkingValue->value.multiBool.t = 0;
            case Place::INT:
                spi->place->currentMarkingValue = new Data(Data::MULTIINT);
            }
    }
    foreach(Transition *transition, net->transitions)
    {
        EditorTransitionItem *eti = editor->scene->getTransitionItem(transition);
        SimulatorTransitionItem *sti = new SimulatorTransitionItem(eti);
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
    }
    findBindings();
}

void CPNetSimulator::toInitialMarking()
{
    foreach(Place *place, net->places)
    {
        if(place->currentMarkingValue)
            delete place->currentMarkingValue;
        if(place->parsedInitialMarking)
           place->currentMarkingValue = new Data(eval(place->parsedInitialMarking, NULL, NULL));
        else
            switch(place->colourSet)
            {
            case Place::UNIT:
                place->currentMarkingValue = new Data(Data::MULTIUNIT);
                place->currentMarkingValue->value.multiUnit = 0;
            case Place::BOOL:
                place->currentMarkingValue = new Data(Data::MULTIBOOL);
                place->currentMarkingValue->value.multiBool.f = 0;
                place->currentMarkingValue->value.multiBool.t = 0;
            case Place::INT:
                place->currentMarkingValue = new Data(Data::MULTIINT);
            }
    }
    findBindings();
}

void CPNetSimulator::findBindings()
{
    foreach(Transition *transition, net->transitions)
    {
        QList<QList<Binding> > bindings;
        foreach(Arc *arc, net->arcs)
        {
            if(!arc->isPreset || arc->transition != transition)
                continue;
            bindings.append(arc->findBindings());
        }
    }
}
