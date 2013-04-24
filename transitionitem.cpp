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

void TransitionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
    painter->drawText(-40, -40, 80, 40, Qt::AlignHCenter | Qt::AlignBottom | Qt::TextWordWrap, transition->name);
    painter->drawText(-40, 0, 80, 40, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, transition->guard);
}
