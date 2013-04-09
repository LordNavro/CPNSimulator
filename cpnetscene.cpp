#include "cpnetscene.h"

CPNetScene::CPNetScene(QObject *parent) :
    QGraphicsScene(parent)
{
    line = NULL;
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
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()), NULL, this);
        break;
    case CPNetScene::DELETE:
        deleteItem(mouseEvent);
        break;
    default:
        break;
    }
}

void CPNetScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(currentTool == ARC && line != NULL)
    {
        line->setLine(QLineF(line->line().p1(), mouseEvent->scenePos()));
    }
    else
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void CPNetScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(currentTool == ARC && line != NULL)
    {
        addArc(line->line().p1(), line->line().p2());
        removeItem(line);
        delete line;
        line = NULL;
    }
    else
    {
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }
}

void CPNetScene::addPlace(QGraphicsSceneMouseEvent *mouseEvent)
{
    Place *place;
    PlaceItem *placeItem;
    place = new Place(this);
    placeItem = new PlaceItem;
    placeItem->place = place;
    addItem(placeItem);
    placeItem->setPos(mouseEvent->scenePos());
    net.places.append(place);
    QGraphicsScene::mousePressEvent(mouseEvent);
    update();
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
    update();
}

void CPNetScene::addArc(QPointF from, QPointF to)
{
    QList<QGraphicsItem *> startItems = items(from);
    if(startItems.count() && startItems.first() == line)
        startItems.removeFirst();
    QList<QGraphicsItem *> endItems = items(to);
    if(endItems.count() && endItems.first() == line)
        endItems.removeFirst();

    if(!startItems.count() || !endItems.count())
        return;

    QGraphicsItem *startItem = startItems.first(), *endItem = endItems.first();
    if(PlaceItem *placeItem = qgraphicsitem_cast<PlaceItem *>(startItem))
    {
        if(TransitionItem *transitionItem = qgraphicsitem_cast<TransitionItem *>(endItem))
        {
            Arc *arc = new Arc();
            arc->isPreset = true;
            arc->place = placeItem->place;
            arc->transition = transitionItem->transition;
            net.arcs.append(arc);
            ArcItem *arcItem = new ArcItem(placeItem, transitionItem);
            arcItem->arc = arc;
            addItem(arcItem);
            placeItem->arcItems.append(arcItem);
            transitionItem->arcItems.append(arcItem);
            arcItem->geometryChanged();
            foreach(QGraphicsItem *item, selectedItems())
                item->setSelected(false);
            arcItem->setSelected(true);
        }
    }
    else if(TransitionItem *transitionItem = qgraphicsitem_cast<TransitionItem *>(startItem))
    {
        if(PlaceItem *placeItem = qgraphicsitem_cast<PlaceItem *>(endItem))
        {
            Arc *arc = new Arc();
            arc->isPreset = false;
            arc->place = placeItem->place;
            arc->transition = transitionItem->transition;
            net.arcs.append(arc);
            ArcItem *arcItem = new ArcItem(transitionItem, placeItem);
            arcItem->arc = arc;
            addItem(arcItem);
            placeItem->arcItems.append(arcItem);
            transitionItem->arcItems.append(arcItem);
            arcItem->geometryChanged();
            foreach(QGraphicsItem *item, selectedItems())
                item->setSelected(false);
            arcItem->setSelected(true);
        }
    }

}

void CPNetScene::deleteItem(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<QGraphicsItem *> affectedItems = items(mouseEvent->scenePos());
    if(affectedItems.isEmpty())
        return;
    QGraphicsItem *item = affectedItems.first();
    if(PlaceItem *placeItem = qgraphicsitem_cast<PlaceItem *>(item))
    {
        foreach(ArcItem *arcItem, placeItem->arcItems)
            deleteArc(arcItem);
        net.places.removeAt(net.places.indexOf(placeItem->place));
        removeItem(placeItem);
        delete placeItem->place;
        delete placeItem;
        update();
    }
    else if(TransitionItem *transitionItem = qgraphicsitem_cast<TransitionItem *>(item))
    {
        foreach(ArcItem *arcItem, transitionItem->arcItems)
            deleteArc(arcItem);
        net.transitions.removeAt(net.transitions.indexOf(transitionItem->transition));
        removeItem(transitionItem);
        delete transitionItem->transition;
        delete transitionItem;
        update();
    }
    else if(ArcItem *arcItem = qgraphicsitem_cast<ArcItem *>(item))
    {
        deleteArc(arcItem);
        update();
    }

}

void CPNetScene::deleteArc(ArcItem *arcItem)
{
    PlaceItem *placeItem;
    TransitionItem *transitionItem;
    if((placeItem = qgraphicsitem_cast<PlaceItem *>(arcItem->from)) != NULL)
    {
        transitionItem = qgraphicsitem_cast<TransitionItem *>(arcItem->to);
    }
    else
    {
        placeItem = qgraphicsitem_cast<PlaceItem *>(arcItem->to);
        transitionItem = qgraphicsitem_cast<TransitionItem *>(arcItem->from);
    }
    placeItem->arcItems.removeAt(placeItem->arcItems.indexOf(arcItem));
    transitionItem->arcItems.removeAt(transitionItem->arcItems.indexOf(arcItem));
    net.arcs.removeAt(net.arcs.indexOf(arcItem->arc));
    removeItem(arcItem);
    delete arcItem->arc;
    delete arcItem;
}

PlaceItem *CPNetScene::getPlaceItem(Place *place)
{
    foreach(QGraphicsItem *item, items())
        if(PlaceItem *placeItem = qgraphicsitem_cast<PlaceItem *>(item))
            if(placeItem->place == place)
                return placeItem;
    return NULL;
}

TransitionItem *CPNetScene::getTransitionItem(Transition *transition)
{
    foreach(QGraphicsItem *item, items())
        if(TransitionItem *transitionItem = qgraphicsitem_cast<TransitionItem *>(item))
            if(transitionItem->transition == transition)
                return transitionItem;
    return NULL;
}

ArcItem *CPNetScene::getArcItem(Arc *arc)
{
    foreach(QGraphicsItem *item, items())
        if(ArcItem *arcItem = qgraphicsitem_cast<ArcItem *>(item))
            if(arcItem->arc == arc)
                return arcItem;
    return NULL;
}

