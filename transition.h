#ifndef TRANSITION_H
#define TRANSITION_H

#include "compiler.h"
#include "interpret.h"

class CPNet;

class Transition
{
public:
    explicit Transition();
    ~Transition();
    
    QString name;
    QString guard;

    Expression *parsedGuard;
    QList<Binding> possibleBindings;

signals:
    
public slots:
    
};

#endif // TRANSITION_H
