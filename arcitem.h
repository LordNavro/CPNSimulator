#ifndef ARCITEM_H
#define ARCITEM_H

#include <QtGui>
#include "arc.h"

class ArcItem : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 30 };

    explicit ArcItem(QGraphicsItem *from, QGraphicsItem *to, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    Arc *arc;

    QGraphicsItem *from;
    QGraphicsItem *to;


    int type() const{ return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPolygonF arrowHead;

private:
    void computeLineCoords();
    QPointF placeIntersect(QGraphicsItem *place, QLineF line);
    QPointF transitionIntersect(QGraphicsItem *transition, QLineF line);
signals:

public slots:
    void geometryChanged();
};
#endif // ARCITEM_H
