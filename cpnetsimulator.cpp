#include "cpnetsimulator.h"

CPNetSimulator::CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent) :
    QWidget(parent), net(net), editor(editor)
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    layout = new QGridLayout(this);
    layout->addWidget(view, 0, 0);
    this->setLayout(layout);
}

void CPNetSimulator::loadNetGraph()
{
    qDeleteAll(scene->items());
    foreach(QGraphicsItem *item, editor->scene->items())
    {

    }
}
