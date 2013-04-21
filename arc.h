#ifndef ARC_H
#define ARC_H

#include "place.h"
#include "transition.h"

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

signals:
    
public slots:
    
};

#endif // ARC_H
