#include "simulatorplaceitem.h"

SimulatorPlaceItem::SimulatorPlaceItem() :
    PlaceItem()
{
}

SimulatorPlaceItem::SimulatorPlaceItem(EditorPlaceItem *pi)
{
    setRect(pi->rect());
    setPos(pi->pos());
    place = pi->place;
}
