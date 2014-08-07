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

#ifndef SIMULATORARCITEM_H
#define SIMULATORARCITEM_H

#include "arcitem.h"

class SimulatorArcItem : public ArcItem
{
public:
    enum { Type = UserType + 32 };
    explicit SimulatorArcItem(QGraphicsItem *from, QGraphicsItem *to, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const{ return Type; }
};

#endif // SIMULATORARCITEM_H
