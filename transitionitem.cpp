#include "transitionitem.h"

TransitionItem::TransitionItem() :
    QGraphicsRectItem()
{
    setRect(-40,-40,80,80);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
