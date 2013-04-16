#ifndef SIMULATORTRANSITIONITEM_H
#define SIMULATORTRANSITIONITEM_H

#include "transitionitem.h"

class SimulatorTransitionItem : public TransitionItem
{
    enum { Type = UserType + 22 };
public:
    SimulatorTransitionItem();

    int type() const{ return Type; }
};

#endif // SIMULATORTRANSITIONITEM_H
