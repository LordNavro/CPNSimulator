#include "cpnetscene.h"

CPNetScene::CPNetScene(QObject *parent) :
    QGraphicsScene(parent)
{
    addLine(0,0,1,1,QPen(Qt::white));
}

void CPNetScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
 {
     if (mouseEvent->button() != Qt::LeftButton)
         return;

     switch(currentTool)
     {
     case CPNetScene::SELECT:
         QGraphicsScene::mousePressEvent(mouseEvent);
         break;
     case CPNetScene::PLACE:
         addPlace(mouseEvent);
         break;
     case CPNetScene::TRANSITION:
         addTransition(mouseEvent);
         break;
     case CPNetScene::ARC:

         break;
     case CPNetScene::DELETE:
         deleteItem(mouseEvent);
         break;
     default:
         break;
     }
}

void CPNetScene::addPlace(QGraphicsSceneMouseEvent *mouseEvent)
{
    Place *place;
    PlaceItem *placeItem;
    place = new Place;
    placeItem = new PlaceItem;
    placeItem->place = place;
    placeItem->setPos(mouseEvent->scenePos());
    addItem(placeItem);
    net.places.append(place);
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CPNetScene::addTransition(QGraphicsSceneMouseEvent *mouseEvent)
{
    Transition *transition;
    TransitionItem *transitionItem;
    transition = new Transition;
    transitionItem = new TransitionItem;
    transitionItem->transition = transition;
    transitionItem->setPos(mouseEvent->scenePos());
    addItem(transitionItem);
    net.transitions.append(transition);
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CPNetScene::deleteItem(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<QGraphicsItem *> affectedItems = items(mouseEvent->scenePos());
    if(affectedItems.isEmpty())
        return;
    QGraphicsItem *item = affectedItems.first();
    if(TransitionItem *transitionItem = qgraphicsitem_cast<TransitionItem *>(item))
    {
        deleteArc(NULL, transitionItem->transition);
        net.transitions.removeAt(net.transitions.indexOf(transitionItem->transition));
        removeItem(transitionItem);
        delete transitionItem->transition;
        delete transitionItem;
    }
    else if(PlaceItem *placeItem = qgraphicsitem_cast<PlaceItem *>(item))
    {
        deleteArc(placeItem->place, NULL);
        net.places.removeAt(net.places.indexOf(placeItem->place));
        removeItem(placeItem);
        delete placeItem->place;
        delete placeItem;
    }

}

void CPNetScene::deleteArc(Place *place, Transition *transition)
{
    QList<QGraphicsItem *> all = items();
    foreach(QGraphicsItem *item, all)
    {
        if(ArcItem *arcItem = qgraphicsitem_cast<ArcItem *>(item))
        {
            if(arcItem->arc->transition == transition || arcItem->arc->place == place)
            {
                net.arcs.removeAt(net.arcs.indexOf(arcItem->arc));
                removeItem(arcItem);
                delete arcItem->arc;
                delete arcItem;
            }
        }
    }
}

