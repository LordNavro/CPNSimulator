#include "transitionitem.h"

TransitionItem::TransitionItem() :
    QGraphicsRectItem()
{
    setRect(-40,-40,80,80);
}

QVariant TransitionItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange || change == QGraphicsItem::ItemScenePositionHasChanged || change == QGraphicsItem::ItemVisibleChange)
    {
        foreach(ArcItem *arcItem, arcItems)
            arcItem->geometryChanged();
    }
    return value;
}
