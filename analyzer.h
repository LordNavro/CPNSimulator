#ifndef ANALYZER_H
#define ANALYZER_H

#include <QtGui>
#include "interpret.h"
#include "compiler.h"
#include "cpnet.h"

class NetMarking : public QMap<Place *, Data>
{

};

class StateSpaceVertex
{
public:
    NetMarking marking;
};

class StateSpaceEdge
{
public:
    StateSpaceVertex *from;
    StateSpaceVertex *to;
    Binding binding;
    Transition *transition;
};

class StateSpaceGraph
{
public:
    CPNet *net;
    QList<StateSpaceVertex> vertices;
    QList<StateSpaceEdge> edges;
};


#endif // ANALYZER_H
