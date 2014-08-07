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

#ifndef PLACE_H
#define PLACE_H

#include "compiler.h"


class Place
{
public:
    explicit Place();
    ~Place();

    typedef enum {UNIT, BOOL, INT} ColourSet;

    QString name;
    Place::ColourSet colourSet;
    QString initialMarking;
    QString currentMarking;

    Expression *parsedInitialMarking;
    Expression *parsedCurrentMarking;

    Data *currentMarkingValue;

signals:
    
public slots:
    
};

#endif // PLACE_H
