#include "cpneteditor.h"

CPNetEditor::CPNetEditor(QWidget *parent) :
    QWidget(parent)
{
    view = new QGraphicsView(this);
    scene = new CPNetScene(this);
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view->setScene(scene);
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));

    formNet = new CPNetForm(this);
    formPlace = new PlaceForm(this);
    formTransition = new TransitionForm(this);
    formArc = new ArcForm(this);

    layout = new QHBoxLayout(this);
    layout->addWidget(view);
    layout->addWidget(formNet);
    layout->addWidget(formPlace);
    layout->addWidget(formTransition);
    layout->addWidget(formArc);

    setLayout(layout);
    slotSelectionChanged();
}

void CPNetEditor::slotSelectionChanged()
{
    formNet->hide();
    formPlace->hide();
    formTransition->hide();
    formArc->hide();
    QList<QGraphicsItem *> selected = scene->selectedItems();
    if(selected.count() != 1)
    {
        formNet->setNet(&scene->net);
        formNet->show();
        return;
    }
    QGraphicsItem *item = selected.first();
    if(PlaceItem *placeItem = qgraphicsitem_cast<PlaceItem *>(item))
    {
        formPlace->setPlaceItem(placeItem);
        formPlace->show();
    }
    else if(TransitionItem *transitionItem = qgraphicsitem_cast<TransitionItem *>(item))
    {
        formTransition->setTransitionItem(transitionItem);
        formTransition->show();
    }
    else if(ArcItem *arcItem = qgraphicsitem_cast<ArcItem *>(item))
    {
        formArc->setArcItem(arcItem);
        formArc->show();
    }
}
