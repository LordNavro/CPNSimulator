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

class StateSpaceVertex
{
public:
    explicit StateSpaceVertex(NetMarking marking) : marking(marking){}
    NetMarking marking;
    bool operator ==(const StateSpaceVertex &vertex);
};

class StateSpaceEdge
{
public:
    StateSpaceEdge(StateSpaceVertex *from, StateSpaceVertex *to, Transition *transition, Binding binding) :
        from(from), to(to), transition(transition), binding(binding){}

    StateSpaceVertex *from;
    StateSpaceVertex *to;
    Transition *transition;
    Binding binding;
    bool operator ==(const StateSpaceEdge &edge);
};

class StateSpaceGraph
{
public:
    QList<StateSpaceVertex> vertices;
    QList<StateSpaceEdge> edges;

    bool depthLimitReached;

    void generate(CPNet *net, Computer *computer, int depth);

protected:
    void findNewVertices(CPNet *net, StateSpaceVertex *vertex, Computer *computer, int depth);
};


#endif // ANALYZER_H
