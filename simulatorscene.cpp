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
