#ifndef ARCITEM_H
#define ARCITEM_H

#include <QtGui>
#include "arc.h"

class ArcItem : public QGraphicsItem
{
public:
    enum { Type = UserType + 30 };

    explicit ArcItem(QGraphicsItem *from, QGraphicsItem *to);

    Arc *arc;

    QGraphicsItem *from;
    QGraphicsItem *to;

    QPointF start;
    QPointF end;

    int type() const{ return Type; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

public slots:
    void geometryChanged();
};

#endif // ARCITEM_H
