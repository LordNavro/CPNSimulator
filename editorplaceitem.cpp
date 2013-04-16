#include "editorplaceitem.h"

EditorPlaceItem::EditorPlaceItem() :
    PlaceItem()
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
