#include "simulatortransitionitem.h"

SimulatorTransitionItem::SimulatorTransitionItem()
    : TransitionItem()
{
}

SimulatorTransitionItem::SimulatorTransitionItem(EditorTransitionItem *eti)
{
    setRect(eti->rect());
    setPos(eti->pos());
    transition = eti->transition;
}
