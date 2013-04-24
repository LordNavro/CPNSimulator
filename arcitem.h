#ifndef ARCITEM_H
#define ARCITEM_H

#include <QtGui>
#include "arc.h"

class ArcItem : public QGraphicsItem
{
public:
    enum { Type = UserType + 30 };

    explicit ArcItem(QGraphicsItem *from, QGraphicsItem *to, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    Arc *arc;

    QGraphicsItem *from;
    QGraphicsItem *to;
    QPointF pointStart;
    QPointF pointEnd;

    QPainterPath pathCurve;
    QLineF line;
    QPolygonF polygonHead;
    QPainterPath pathShape;

    int type() const{ return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    qreal angle();

    //QPolygonF arrowHead;

private:
    void computePath();
    void computePolygon();
    void computeShape();
    QPointF placeIntersect(QGraphicsItem *place, QLineF line);
    QPointF transitionIntersect(QGraphicsItem *transition, QLineF line);
signals:

public slots:
    void geometryChanged();
};
#endif // ARCITEM_H
