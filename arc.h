#ifndef ARC_H
#define ARC_H

#include "place.h"
#include "transition.h"
#include "interpret.h"

class Computer;

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
    QList<Binding> findBindings(Computer *computer);

signals:
    
public slots:
    
};

#endif // ARC_H
