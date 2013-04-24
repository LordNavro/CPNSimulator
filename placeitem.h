#ifndef PLACEITEM_H
#define PLACEITEM_H

#include <QtGui>
#include "place.h"
#include "arcitem.h"

class PlaceItem : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 10 };

    explicit PlaceItem();

    Place *place;
    QList<ArcItem *>arcItems;

    int type() const{ return Type; }

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

public slots:

};

#endif // PLACEITEM_H
