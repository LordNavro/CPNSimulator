#include "simulatorscene.h"

SimulatorScene::SimulatorScene(CPNet *net, QObject *parent) :
    QGraphicsScene(parent), net(net)
{
}

SimulatorPlaceItem *SimulatorScene::getPlaceItem(Place *place)
{
    foreach(QGraphicsItem *item, items())
        if(SimulatorPlaceItem *placeItem = qgraphicsitem_cast<SimulatorPlaceItem *>(item))
            if(placeItem->place == place)
                return placeItem;
    return NULL;
}

SimulatorTransitionItem *SimulatorScene::getTransitionItem(Transition *transition)
{
    foreach(QGraphicsItem *item, items())
        if(SimulatorTransitionItem *transitionItem = qgraphicsitem_cast<SimulatorTransitionItem *>(item))
            if(transitionItem->transition == transition)
                return transitionItem;
    return NULL;
}

SimulatorArcItem *SimulatorScene::getArcItem(Arc *arc)
{
    foreach(QGraphicsItem *item, items())
        if(SimulatorArcItem *arcItem = qgraphicsitem_cast<SimulatorArcItem *>(item))
            if(arcItem->arc == arc)
                return arcItem;
    return NULL;
}
