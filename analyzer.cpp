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

#include "analyzer.h"
#include "computer.h"


void StateSpaceGraph::generate(CPNet *net, Computer *computer, int depth)
{
    vertices.clear();
    depthLimitReached = false;
    StateSpaceVertex v(net->currentMarking());
    vertices.append(v);
    findNewVertices(net, &vertices.last(), 0, computer, depth);
}

QList<int> StateSpaceGraph::findPath(int endIndex, int startIndex)
{
    QList<bool> walked;
    for(int i = 0; i < vertices.size(); i++)
        walked.append(false);
    QList<QPair<QList<int>, int> > toWalk;
    toWalk.prepend(QPair<QList<int>, int>(QList<int>(), startIndex));
    while(!toWalk.isEmpty())
    {
        QPair<QList<int>, int> current = toWalk.first();
        toWalk.removeFirst();
        QList<int> currentPath = current.first;
        int currentIndex = current.second;
        walked[currentIndex] = true;
        currentPath.append(currentIndex);
        if(currentIndex == endIndex)
            return currentPath;
        const QList<StateSpaceEdge> &edges = vertices.at(currentIndex).edges;
        for(int j = 0; j < edges.count(); j++)
        {
            if(walked.at(edges.at(j).to))
                continue;
            QList<int> newPath(currentPath);
            newPath.append(j);
            toWalk.prepend(QPair<QList<int>, int>(newPath, edges.at(j).to));
        }
    }
    return QList<int>();
}

void StateSpaceGraph::findNewVertices(CPNet *net, StateSpaceVertex *vertex, int vertexIndex, Computer *computer, int depth)
{
    if(depth-- <= 0)
    {
        depthLimitReached = true;
        return;
    }
    if(computer->cancelRequest)
        throw(QString("Cancel request received"));

    computer->findBinding(vertex->marking);
    QList<QPair<StateSpaceVertex *, int> > newVertices;
    foreach(Transition *transition, net->transitions)
    {
        foreach(Binding binding, transition->possibleBindings)
        {
            NetMarking m = computer->fireTransition(vertex->marking, transition, binding);
            StateSpaceVertex newVertex(m);
            if(vertices.contains(newVertex))
            {
                StateSpaceEdge edge(vertexIndex, vertices.indexOf(newVertex), transition, binding);
                vertex->edges.append(edge);
                continue;
            }
            vertices.append(newVertex);
            newVertices.append(QPair<StateSpaceVertex *, int>(&vertices.last(), vertices.count() - 1));
            StateSpaceEdge edge(vertexIndex, vertices.count() - 1, transition, binding);
            vertex->edges.append(edge);
        }
    }
    QPair<StateSpaceVertex *, int> newVertex;
    foreach(newVertex, newVertices)
    {
        findNewVertices(net, newVertex.first, newVertex.second, computer, depth);
    }
}


bool StateSpaceVertex::operator ==(const StateSpaceVertex &vertex)
{
    return marking == vertex.marking;
}

QString StateSpaceVertex::toString() const
{
    QStringList ret;
    NetMarking::const_iterator i = marking.constBegin();
    while(i != marking.constEnd())
    {
        if(!i.value().toString().isEmpty())
            ret += i.key()->name + ": " + i.value().toString();
        ++i;
    }
    return ret.join("; ");
}


bool StateSpaceEdge::operator ==(const StateSpaceEdge &edge)
{
    return from == edge.from
            && to == edge.to
            && binding == edge.binding
            && transition == edge.transition;
}

QString StateSpaceEdge::toString() const
{
    QString text;
    text += QObject::tr("[%1> (%2)").arg(transition->name, binding.toString());
    return text;
}
