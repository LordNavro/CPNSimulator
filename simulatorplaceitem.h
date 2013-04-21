#ifndef SIMULATORPLACEITEM_H
#define SIMULATORPLACEITEM_H

#include "placeitem.h"
#include "editorplaceitem.h"

class SimulatorPlaceItem : public PlaceItem
{
public:
    enum { Type = UserType + 12 };
    explicit SimulatorPlaceItem();
    explicit SimulatorPlaceItem(EditorPlaceItem *pi);
    ~SimulatorPlaceItem();

    int type() const{ return Type; }
};

#endif // SIMULATORPLACEITEM_H
