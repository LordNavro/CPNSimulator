#include "simulatorplaceitem.h"

SimulatorPlaceItem::SimulatorPlaceItem() :
    PlaceItem()
{
}

SimulatorPlaceItem::SimulatorPlaceItem(EditorPlaceItem *pi)
    : PlaceItem()
{
    setRect(pi->rect());
    setPos(pi->pos());
    place = pi->place;
}

SimulatorPlaceItem::~SimulatorPlaceItem()
{
}
