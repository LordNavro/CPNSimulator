#include "arcitem.h"

ArcItem::ArcItem(QGraphicsItem *from, QGraphicsItem *to)
    : QGraphicsItem()
{
    this->from = from;
    this->to = to;
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QRectF ArcItem::boundingRect() const
{
    return QRectF(start, end);
}

void ArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isSelected())
        painter->setPen(QPen(Qt::red));
    painter->drawLine(start, end);
    painter->drawEllipse(end, 5, 5);
}

void ArcItem::geometryChanged()
{
    start = from->pos();
    end = to->pos();
    update();
    this->scene()->update();
}
