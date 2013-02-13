#ifndef CPNETSCENE_H
#define CPNETSCENE_H

#include <QtGui>
#include "cpnet.h"
#include "placeitem.h"
#include "transitionitem.h"
#include "arcitem.h"
#include "cpnetform.h"
#include "placeform.h"
#include "transitionform.h"
#include "arcform.h"

class CPNetScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CPNetScene(QObject *parent = 0);

    typedef enum{SELECT, PLACE, TRANSITION, ARC, DELETE} Tool;

    QString fileName;
    QString name;
    Tool currentTool;
    CPNet net;

    QGraphicsLineItem *line;

    CPNetForm *netForm;
    PlaceForm *placeForm;
    TransitionForm *transitionForm;
    ArcForm *arcForm;


    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    void addPlace(QGraphicsSceneMouseEvent *mouseEvent);
    void addTransition(QGraphicsSceneMouseEvent *mouseEvent);
    void addArc(QPointF from, QPointF to);
    void deleteItem(QGraphicsSceneMouseEvent *mouseEvent);
    void deleteArc(ArcItem *arcItem);

signals:
    
public slots:
};

#endif // CPNETSCENE_H
