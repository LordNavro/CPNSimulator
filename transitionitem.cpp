#include "transitionitem.h"

TransitionItem::TransitionItem() :
    QGraphicsRectItem()
{
    setRect(-40,-40,80,80);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QVariant TransitionItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange)
    {
        foreach(ArcItem *arcItem, arcItems)
            arcItem->geometryChanged();
    }
    return value;
}
