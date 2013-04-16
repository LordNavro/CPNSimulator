#ifndef SIMULATORPLACEITEM_H
#define SIMULATORPLACEITEM_H

#include "placeitem.h"

class SimulatorPlaceItem : public PlaceItem
{
    enum { Type = UserType + 11 };

    explicit SimulatorPlaceItem();

    int type() const{ return Type; }
};

#endif // SIMULATORPLACEITEM_H
