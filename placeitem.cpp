#include "placeitem.h"
#include "simulatorplaceitem.h"

PlaceItem::PlaceItem() :
    QGraphicsEllipseItem()
{
    setRect(-40,-40,80,80);
    //setFlag(QGraphicsItem::ItemClipsToShape, true);
}

QVariant PlaceItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange || change == QGraphicsItem::ItemScenePositionHasChanged || change == QGraphicsItem::ItemVisibleChange)
    {
        foreach(ArcItem *arcItem, arcItems)
            arcItem->geometryChanged();
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void PlaceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);
    painter->drawText(-40, -40, 80, 40, Qt::AlignHCenter | Qt::AlignBottom | Qt::TextWordWrap, place->name);
    if(type() == SimulatorPlaceItem::Type && place->currentMarkingValue)
        painter->drawText(-40, 0, 80, 40, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, place->currentMarkingValue->toString());
    else
        painter->drawText(-40, 0, 80, 40, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, place->currentMarking);
}



