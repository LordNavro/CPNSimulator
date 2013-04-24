#include "simulatorplaceitem.h"


SimulatorPlaceItem::SimulatorPlaceItem(EditorPlaceItem *pi)
    : PlaceItem()
{
    setRect(pi->rect());
    setPos(pi->pos());
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    place = pi->place;
}

SimulatorPlaceItem::~SimulatorPlaceItem()
{
}
