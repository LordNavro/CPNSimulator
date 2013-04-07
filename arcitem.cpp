#include "arcitem.h"
#define PI 3.141592653589

ArcItem::ArcItem(QGraphicsItem *from, QGraphicsItem *to, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    this->from = from;
    this->to = to;
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QRectF ArcItem::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath ArcItem::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void ArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(from->collidesWithItem(to))
        return;

    computeLineCoords();

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = 2 * PI - angle;

    qreal arrowSize = 20;
    QPointF arrowP1 = line().p1() + QPointF(sin(angle + PI / 3) * arrowSize, cos(angle + PI / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + PI - PI / 3) * arrowSize, cos(angle + PI - PI / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;

    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected())
    {
        painter->setPen(QPen(Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}

void ArcItem::computeLineCoords()
{
    QLineF centerLine(from->pos(), to->pos());
    if(arc->isPreset)
        setLine(QLineF(transitionIntersect(to, centerLine), placeIntersect(from, centerLine)));
    else
        setLine(QLineF(placeIntersect(to, centerLine), transitionIntersect(from, centerLine)));

}

QPointF ArcItem::placeIntersect(QGraphicsItem *place, QLineF line)
{
    if(line.p1() == place->pos())
        line.setLine(line.x2(), line.y2(), line.x1(), line.y1());
    line.setLength(line.length() - place->boundingRect().width() / 2);
    return line.p2();
}

QPointF ArcItem::transitionIntersect(QGraphicsItem *transition, QLineF line)
{
    QPolygonF endPolygon = transition->boundingRect();
    QPointF p1 = endPolygon.first() + transition->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i)
    {
        p2 = endPolygon.at(i) + transition->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType = polyLine.intersect(line, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }
    return intersectPoint;
}

void ArcItem::geometryChanged()
{
    QLineF line(mapFromItem(from, 0, 0), mapFromItem(to, 0, 0));
    setLine(line);
}
