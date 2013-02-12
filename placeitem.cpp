#include "placeitem.h"

PlaceItem::PlaceItem() :
    QGraphicsEllipseItem()
{
    setRect(-40,-40,80,80);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}
