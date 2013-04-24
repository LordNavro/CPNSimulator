#include "arcitem.h"
#define PI 3.141592653589
#define SEL_WIDTH 10
#define BEZ_WIDTH 50

ArcItem::ArcItem(QGraphicsItem *from, QGraphicsItem *to, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene), from(from), to(to)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

QRectF ArcItem::boundingRect() const
{
    return shape().boundingRect().normalized().adjusted(-10,-10,10,10);
}

QPainterPath ArcItem::shape() const
{
    return pathShape;
}

void ArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    qDebug() << qrand() << "repainting called";

    if(from->collidesWithItem(to))
        return;
    painter->drawPath(pathCurve);
    painter->drawPolygon(polygonHead);

    if (isSelected())
    {
        painter->setPen(QPen(Qt::DashLine));
        painter->drawPath(shape());
    }
}

qreal ArcItem::angle()
{
    return line.angle() / 360 * 2 * PI;
}

void ArcItem::computePath()
{

    QLineF centerLine(from->pos(), to->pos());
    if(arc->isPreset)
    {
        pointStart = placeIntersect(from, centerLine);
        pointEnd = transitionIntersect(to, centerLine);
    }
    else
    {
        pointStart = transitionIntersect(from, centerLine);
        pointEnd = placeIntersect(to, centerLine);
    }
    line.setPoints(pointStart, pointEnd);
    pathCurve = QPainterPath(pointStart);
    QPointF center = line.pointAt(0.5);
    center += QPointF(line.normalVector().p2() - line.p1()) / line.length() * BEZ_WIDTH;
    pathCurve.quadTo(center,line.p2());
}

void ArcItem::computePolygon()
{
    qreal arrowSize = 20;
    qreal skew = ::atan(BEZ_WIDTH /line.length());
    QPointF arrowP1 = line.p2() - QPointF(sin(angle() - skew + PI / 3) * arrowSize, cos(angle() - skew + PI / 3) * arrowSize);
    QPointF arrowP2 = line.p2() - QPointF(sin(angle() - skew + PI - PI / 3) * arrowSize, cos(angle() - skew + PI - PI / 3) * arrowSize);
    polygonHead.clear();
    polygonHead << line.p2() << arrowP1 << arrowP2;
}

void ArcItem::computeShape()
{
    QPainterPath path(line.p1());
    QPointF offset = (line.normalVector().p2() - line.p1()) / line.length() * SEL_WIDTH / 2;
    path.lineTo(line.p1() + offset);
    QPointF center = line.pointAt(0.5);
    center += QPointF(line.normalVector().p2() - line.p1()) / line.length() * BEZ_WIDTH;
    path.quadTo(center + offset, line.p2() + offset);
    path.lineTo(line.p2() - offset);
    path.quadTo(center - offset, line.p1() - offset);
    path.lineTo(line.p1());
    path.closeSubpath();
    path.addPolygon(polygonHead);

    pathShape = path;
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
    qDebug() << qrand() << "updated";
    computePath();
    computePolygon();
    computeShape();
    scene()->update();
}
