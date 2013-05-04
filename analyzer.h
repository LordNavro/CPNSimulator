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
