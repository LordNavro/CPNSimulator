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
    toWalk.append(QPair<QList<int>, int>(QList<int>(), startIndex));
    for(int i = 0; i < toWalk.count(); i++)
    {
        QList<int> currentPath = toWalk.at(i).first;
        int currentIndex = toWalk.at(i).second;
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
            toWalk.append(QPair<QList<int>, int>(newPath, edges.at(j).to));
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
