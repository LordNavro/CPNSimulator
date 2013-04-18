#include "cpnetsimulator.h"

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
}
