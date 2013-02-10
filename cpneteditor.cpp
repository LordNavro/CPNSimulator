#include "cpneteditor.h"

CPNetEditor::CPNetEditor(QWidget *parent) :
    QWidget(parent)
{
    view = new QGraphicsView(this);
    scene = new CPNetScene(this);
    view->setScene(scene);


    layout = new QHBoxLayout(this);
    layout->addWidget(view);
    layout->addWidget(new QLabel("form...", this));

    setLayout(layout);
}
