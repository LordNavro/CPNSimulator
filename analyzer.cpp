#include "analyzer.h"
#include "computer.h"


void StateSpaceGraph::generate(CPNet *net, Computer *computer, int depth)
{
    vertices.clear();
    edges.clear();
    depthLimitReached = false;
    StateSpaceVertex v(net->currentMarking());
    vertices.append(v);
    findNewVertices(net, &vertices.last(), computer, depth);
    qDebug() << "Verices: " << vertices.count() << "Edges:" << edges.count() << "Limit:" << depthLimitReached;
}

void StateSpaceGraph::findNewVertices(CPNet *net, StateSpaceVertex *vertex, Computer *computer, int depth)
{
    if(depth-- <= 0)
    {
        depthLimitReached = true;
        return;
    }
    if(computer->cancelRequest)
        throw(QString("Cancel request received"));

    computer->findBinding(vertex->marking);
    QList<StateSpaceVertex *> newVertices;
    foreach(Transition *transition, net->transitions)
    {
        foreach(Binding binding, transition->possibleBindings)
        {
            NetMarking m = computer->fireTransition(vertex->marking, transition, binding);
            StateSpaceVertex newVertex(m);
            if(vertices.contains(newVertex))
            {
                StateSpaceEdge edge(vertex, &vertices[vertices.indexOf(newVertex)],
                                    transition, binding);
                edges.append(edge);
                continue;
            }
            vertices.append(newVertex);
            newVertices.append(&vertices.last());
            StateSpaceEdge edge(vertex, &vertices.last(), transition, binding);
            edges.append(edge);
        }
    }
    foreach(StateSpaceVertex *newVertex, newVertices)
    {
        findNewVertices(net, newVertex, computer, depth);
    }
}


bool StateSpaceVertex::operator ==(const StateSpaceVertex &vertex)
{
    return marking == vertex.marking;
}


bool StateSpaceEdge::operator ==(const StateSpaceEdge &edge)
{
    return from == edge.from
            && to == edge.to
            && binding == edge.binding
            && transition == edge.transition;
}
