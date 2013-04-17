#ifndef SIMULATORTRANSITIONITEM_H
#define SIMULATORTRANSITIONITEM_H

#include "editortransitionitem.h"


class SimulatorTransitionItem : public TransitionItem
{
public:
    enum { Type = UserType + 22 };

    explicit SimulatorTransitionItem();
    explicit SimulatorTransitionItem(EditorTransitionItem *eti);

    int type() const{ return Type; }
};

#endif // SIMULATORTRANSITIONITEM_H
