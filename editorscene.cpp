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

#include "editorscene.h"

EditorScene::EditorScene(CPNet *net, QObject *parent) :
    QGraphicsScene(parent), net(net), line(NULL), rect(NULL)
{
    //initialize viewport
    QGraphicsLineItem *line = addLine(0,0,10,10);
    emit sceneRectChanged(sceneRect());
    delete line;
}

void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::RightButton)
    {
        if(line)
        {
            delete line;
            line = NULL;
        }
        currentTool = SELECT;
        emit signalForceSelect();
    }
    if (mouseEvent->button() != Qt::LeftButton && currentTool != SELECT)
        return;
    switch(currentTool)
    {
    case EditorScene::SELECT:
        if(mouseEvent->button() == Qt::RightButton)
        {
            selectionStartPoint = mouseEvent->scenePos();
            rect = new QGraphicsRectItem(QRectF(selectionStartPoint, selectionStartPoint), 0, this);
            rect->setPen(QPen(Qt::DashLine));
        }
        else
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
        line->setPen(QPen(Qt::DashLine));
        break;
    case EditorScene::DELETE:
        deleteOnPos(mouseEvent);
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
    else if(currentTool == SELECT && rect != NULL)
    {
        rect->setRect(QRectF(selectionStartPoint, mouseEvent->scenePos()).normalized());
    }
    else
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(currentTool == ARC && line != NULL && mouseEvent->button() == Qt::LeftButton)
    {
        addArc(line->line().p1(), line->line().p2());
        removeItem(line);
        delete line;
        line = NULL;
    }
    else if(currentTool == SELECT && rect != NULL && mouseEvent->button() == Qt::RightButton)
    {
        QPainterPath path;
        path.addRect(rect->rect());
        setSelectionArea(path);
        removeItem(rect);
        delete rect;
        rect = NULL;
    }
    else
    {
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }
}

void EditorScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        //so that we dont delete arcs twice (they get deleted along with the incident item
        blockSignals(true);
        while(!selectedItems().isEmpty())
            deleteItem(selectedItems().first());
        blockSignals(false);
        emit selectionChanged();
    }
    else
    {
        QGraphicsScene::keyPressEvent(event);
    }
}

void EditorScene::addPlace(QGraphicsSceneMouseEvent *mouseEvent)
{
    static int counter = 0;
    Place *place = new Place();
    place->name = "P" + QString::number(++counter);
    EditorPlaceItem *placeItem = new EditorPlaceItem;
    placeItem->place = place;
    addItem(placeItem);
    placeItem->setPos(mouseEvent->scenePos());
    net->places.append(place);
    QGraphicsScene::mousePressEvent(mouseEvent);
    update();
}

void EditorScene::addTransition(QGraphicsSceneMouseEvent *mouseEvent)
{
    static int counter = 0;
    Transition *transition = new Transition();
    transition->name = "T" + QString::number(++counter);
    EditorTransitionItem *transitionItem = new EditorTransitionItem();
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
    //eliminate duplicate arrows
    foreach(QGraphicsItem *item, items())
    {
        EditorArcItem *arcItem = qgraphicsitem_cast<EditorArcItem *>(item);
        if(arcItem && arcItem->from == startItem && arcItem->to == endItem)
            return;
    }

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

void EditorScene::deleteOnPos(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<QGraphicsItem *> affectedItems = items(mouseEvent->scenePos());
    if(affectedItems.isEmpty())
        return;
    QGraphicsItem *item = affectedItems.first();
    deleteItem(item);
}

void EditorScene::deleteItem(QGraphicsItem *item)
{
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

