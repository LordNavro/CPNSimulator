#include "editorscene.h"

EditorScene::EditorScene(CPNet *net, QObject *parent) :
    QGraphicsScene(parent), net(net)
{
    line = NULL;
    addLine(0,0,1,1,QPen(Qt::white));
}

void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    switch(currentTool)
    {
    case EditorScene::SELECT:
        QGraphicsScene::mousePressEvent(mouseEvent);
        break;
    case EditorScene::PLACE:
        addPlace(mouseEvent);
        break;
    case EditorScene::TRANSITION:
        addTransition(mouseEvent);
        break;
    case EditorScene::ARC:
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()), NULL, this);
        break;
    case EditorScene::DELETE:
        deleteItem(mouseEvent);
        break;
    default:
        break;
    }
}

void EditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
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

void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
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

void EditorScene::addPlace(QGraphicsSceneMouseEvent *mouseEvent)
{
    Place *place;
    EditorPlaceItem *placeItem;
    place = new Place(this);
    placeItem = new EditorPlaceItem;
    placeItem->place = place;
    addItem(placeItem);
    placeItem->setPos(mouseEvent->scenePos());
    net->places.append(place);
    QGraphicsScene::mousePressEvent(mouseEvent);
    update();
}

void EditorScene::addTransition(QGraphicsSceneMouseEvent *mouseEvent)
{
    Transition *transition;
    EditorTransitionItem *transitionItem;
    transition = new Transition;
    transitionItem = new EditorTransitionItem;
    transitionItem->transition = transition;
    transitionItem->setPos(mouseEvent->scenePos());
    addItem(transitionItem);
    net->transitions.append(transition);
    QGraphicsScene::mousePressEvent(mouseEvent);
    update();
}

void EditorScene::addArc(QPointF from, QPointF to)
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
    if(EditorPlaceItem *placeItem = qgraphicsitem_cast<EditorPlaceItem *>(startItem))
    {
        if(EditorTransitionItem *transitionItem = qgraphicsitem_cast<EditorTransitionItem *>(endItem))
        {
            Arc *arc = new Arc();
            arc->isPreset = true;
            arc->place = placeItem->place;
            arc->transition = transitionItem->transition;
            net->arcs.append(arc);
            EditorArcItem *arcItem = new EditorArcItem(placeItem, transitionItem);
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
    else if(EditorTransitionItem *transitionItem = qgraphicsitem_cast<EditorTransitionItem *>(startItem))
    {
        if(EditorPlaceItem *placeItem = qgraphicsitem_cast<EditorPlaceItem *>(endItem))
        {
            Arc *arc = new Arc();
            arc->isPreset = false;
            arc->place = placeItem->place;
            arc->transition = transitionItem->transition;
            net->arcs.append(arc);
            EditorArcItem *arcItem = new EditorArcItem(transitionItem, placeItem);
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

void EditorScene::deleteItem(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<QGraphicsItem *> affectedItems = items(mouseEvent->scenePos());
    if(affectedItems.isEmpty())
        return;
    QGraphicsItem *item = affectedItems.first();
    if(EditorPlaceItem *placeItem = qgraphicsitem_cast<EditorPlaceItem *>(item))
    {
        foreach(ArcItem *arcItem, placeItem->arcItems)
            deleteArc(qgraphicsitem_cast<EditorArcItem *>(arcItem));
        net->places.removeAt(net->places.indexOf(placeItem->place));
        removeItem(placeItem);
        delete placeItem->place;
        delete placeItem;
        update();
    }
    else if(EditorTransitionItem *transitionItem = qgraphicsitem_cast<EditorTransitionItem *>(item))
    {
        foreach(ArcItem *arcItem, transitionItem->arcItems)
            deleteArc(qgraphicsitem_cast<EditorArcItem *>(arcItem));
        net->transitions.removeAt(net->transitions.indexOf(transitionItem->transition));
        removeItem(transitionItem);
        delete transitionItem->transition;
        delete transitionItem;
        update();
    }
    else if(EditorArcItem *arcItem = qgraphicsitem_cast<EditorArcItem *>(item))
    {
        deleteArc(arcItem);
        update();
    }

}

void EditorScene::deleteArc(EditorArcItem *arcItem)
{
    EditorPlaceItem *placeItem;
    EditorTransitionItem *transitionItem;
    if((placeItem = qgraphicsitem_cast<EditorPlaceItem *>(arcItem->from)) != NULL)
    {
        transitionItem = qgraphicsitem_cast<EditorTransitionItem *>(arcItem->to);
    }
    else
    {
        placeItem = qgraphicsitem_cast<EditorPlaceItem *>(arcItem->to);
        transitionItem = qgraphicsitem_cast<EditorTransitionItem *>(arcItem->from);
    }
    placeItem->arcItems.removeAt(placeItem->arcItems.indexOf(arcItem));
    transitionItem->arcItems.removeAt(transitionItem->arcItems.indexOf(arcItem));
    net->arcs.removeAt(net->arcs.indexOf(arcItem->arc));
    removeItem(arcItem);
    delete arcItem->arc;
    delete arcItem;
}

EditorPlaceItem *EditorScene::getPlaceItem(Place *place)
{
    foreach(QGraphicsItem *item, items())
        if(EditorPlaceItem *placeItem = qgraphicsitem_cast<EditorPlaceItem *>(item))
            if(placeItem->place == place)
                return placeItem;
    return NULL;
}

EditorTransitionItem *EditorScene::getTransitionItem(Transition *transition)
{
    foreach(QGraphicsItem *item, items())
        if(EditorTransitionItem *transitionItem = qgraphicsitem_cast<EditorTransitionItem *>(item))
            if(transitionItem->transition == transition)
                return transitionItem;
    return NULL;
}

EditorArcItem *EditorScene::getArcItem(Arc *arc)
{
    foreach(QGraphicsItem *item, items())
        if(EditorArcItem *arcItem = qgraphicsitem_cast<EditorArcItem *>(item))
            if(arcItem->arc == arc)
                return arcItem;
    return NULL;
}

