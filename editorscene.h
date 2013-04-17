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

    QGraphicsLineItem *line;

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    void addPlace(QGraphicsSceneMouseEvent *mouseEvent);
    void addTransition(QGraphicsSceneMouseEvent *mouseEvent);
    void addArc(QPointF from, QPointF to);
    void deleteItem(QGraphicsSceneMouseEvent *mouseEvent);
    void deleteArc(EditorArcItem *arcItem);

    EditorPlaceItem *getPlaceItem(Place *place);
    EditorTransitionItem *getTransitionItem(Transition *transition);
    EditorArcItem *getArcItem(Arc *arc);

signals:
    
public slots:
};

#endif // EDITORSCENE_H
