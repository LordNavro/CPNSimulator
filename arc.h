#ifndef ARC_H
#define ARC_H

#include "place.h"
#include "transition.h"
#include "interpret.h"

class Computer;
class NetMarking;

class Arc
{
public:
    explicit Arc();
    ~Arc();
    
    QString expression;
    Place *place;
    Transition *transition;
    bool isPreset;

    Expression *parsedExpression;
    QList<Binding> findBindings(Computer *computer, NetMarking marking);

signals:
    
public slots:
    
};

#endif // ARC_H
