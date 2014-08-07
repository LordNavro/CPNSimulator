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

#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include <QtGui>
#include "cpnet.h"
#include "editorplaceitem.h"
#include "editortransitionitem.h"
#include "editorarcitem.h"
#include "cpnetform.h"
#include "placeform.h"
#include "transitionform.h"
#include "arcform.h"

class EditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EditorScene(CPNet *net, QObject *parent = 0);

    typedef enum{SELECT, PLACE, TRANSITION, ARC, DELETE} Tool;

    Tool currentTool;
    CPNet *net;

    QPointF selectionStartPoint;
    QGraphicsLineItem *line;
    QGraphicsRectItem *rect;

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *event);

    void addPlace(QGraphicsSceneMouseEvent *mouseEvent);
    void addTransition(QGraphicsSceneMouseEvent *mouseEvent);
    void addArc(QPointF from, QPointF to);
    void deleteOnPos(QGraphicsSceneMouseEvent *mouseEvent);
    void deleteItem(QGraphicsItem *item);
    void deleteArc(EditorArcItem *arcItem);

    EditorPlaceItem *getPlaceItem(Place *place);
    EditorTransitionItem *getTransitionItem(Transition *transition);
    EditorArcItem *getArcItem(Arc *arc);

signals:
    void signalForceSelect();
public slots:
};

#endif // EDITORSCENE_H
