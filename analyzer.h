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

#ifndef ANALYZER_H
#define ANALYZER_H

#include <QtGui>
#include "interpret.h"
#include "compiler.h"
#include "cpnet.h"

class Computer;

class NetMarking : public QMap<Place *, Data>
{

};

class StateSpaceVertex;

class StateSpaceEdge
{
public:
    StateSpaceEdge(int from, int to, Transition *transition, Binding binding) :
        from(from), to(to), transition(transition), binding(binding){}

    int from;
    int to;
    Transition *transition;
    Binding binding;
    bool operator ==(const StateSpaceEdge &edge);
    QString toString() const;
};

class StateSpaceVertex
{
public:
    explicit StateSpaceVertex(NetMarking marking) : marking(marking){}
    QList<StateSpaceEdge> edges;
    NetMarking marking;
    bool operator ==(const StateSpaceVertex &vertex);
    QString toString() const;
};


class StateSpaceGraph
{
public:
    QList<StateSpaceVertex> vertices;

    bool depthLimitReached;

    void generate(CPNet *net, Computer *computer, int depth);
    QList<int> findPath(int endIndex, int startIndex = 0);

protected:
    void findNewVertices(CPNet *net, StateSpaceVertex *vertex, int vertexIndex, Computer *computer, int depth);
};


#endif // ANALYZER_H
