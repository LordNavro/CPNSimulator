#ifndef SIMULATORSCENE_H
#define SIMULATORSCENE_H

#include <QGraphicsScene>
#include "simulatorplaceitem.h"
#include "simulatortransitionitem.h"
#include "simulatorarcitem.h"

class SimulatorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SimulatorScene(CPNet *net, QObject *parent = 0);

    CPNet *net;

    SimulatorPlaceItem *getPlaceItem(Place *place);
    SimulatorTransitionItem *getTransitionItem(Transition *transition);
    SimulatorArcItem *getArcItem(Arc *arc);
    
signals:
    
public slots:
    
};

#endif // SIMULATORSCENE_H
