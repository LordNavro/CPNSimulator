#ifndef CPNETSCENE_H
#define CPNETSCENE_H

#include <QtGui>
#include "cpnet.h"
#include "placeitem.h"
#include "transitionitem.h"
#include "arcitem.h"

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

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void addPlace(QGraphicsSceneMouseEvent *mouseEvent);
    void addTransition(QGraphicsSceneMouseEvent *mouseEvent);
    void deleteItem(QGraphicsSceneMouseEvent *mouseEvent);
    void deleteArc(Place *place, Transition *transition);

signals:
    
public slots:
    
};

#endif // CPNETSCENE_H
