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

#include "transitionitem.h"

TransitionItem::TransitionItem() :
    QGraphicsRectItem()
{
    setRect(-40,-40,80,80);
    QPen pen(Qt::black);
    pen.setWidth(1);
    setPen(pen);
}



QVariant TransitionItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange || change == QGraphicsItem::ItemScenePositionHasChanged || change == QGraphicsItem::ItemVisibleChange)
    {
        foreach(ArcItem *arcItem, arcItems)
            arcItem->geometryChanged();
    }
    return QGraphicsRectItem::itemChange(change, value);
}

void TransitionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
    painter->drawText(-40, -40, 80, 40, Qt::AlignHCenter | Qt::AlignBottom | Qt::TextWordWrap, transition->name);
    painter->drawText(-40, 0, 80, 40, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, transition->guard);
    setToolTip(transition->name + ": " + transition->guard);
}
