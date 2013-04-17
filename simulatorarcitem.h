#ifndef SIMULATORARCITEM_H
#define SIMULATORARCITEM_H

#include "arcitem.h"

class SimulatorArcItem : public ArcItem
{
public:
    enum { Type = UserType + 32 };
    explicit SimulatorArcItem(QGraphicsItem *from, QGraphicsItem *to, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const{ return Type; }
};

#endif // SIMULATORARCITEM_H
