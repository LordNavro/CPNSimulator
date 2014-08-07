/*
 *    Copyright 2013, 2014 Ondrej Navratil.
 *    This project and all of its contents are distributed under the terms of the GNU General Public License.
 *
 *    This file is part of CPNSimulator.
 *
 *    CPNSimulator is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published b
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    CPNSimulator is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with CPNSimulator. If not, see <http://www.gnu.org/licenses/>.
*/

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
    QRectF rectText;

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
