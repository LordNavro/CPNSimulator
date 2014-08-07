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

#include "placeitem.h"
#include "simulatorplaceitem.h"

PlaceItem::PlaceItem() :
    QGraphicsEllipseItem()
{
    setRect(-40,-40,80,80);
    QPen pen(Qt::black);
    pen.setWidth(1);
    setPen(pen);

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
    QString set;
    switch (place->colourSet) {
    case Place::UNIT:
        set = "UNIT";
        break;
    case Place::BOOL:
        set = "BOOL";
        break;
    case Place::INT:
        set = "INT";
        break;
    }
    painter->drawText(-40, -40, 80, 40, Qt::AlignHCenter | Qt::AlignBottom | Qt::TextWordWrap, place->name + " (" + set + ")");
    if(type() == SimulatorPlaceItem::Type && place->currentMarkingValue)
    {
        setToolTip(place->name + " (" + set + ") :" + place->currentMarkingValue->toString());
        painter->drawText(-40, 0, 80, 40, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, place->currentMarkingValue->toString());
    }
    else
    {
        setToolTip(place->name + " (" + set + ") :" + place->currentMarking);
        painter->drawText(-40, 0, 80, 40, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, place->currentMarking);
    }
}



