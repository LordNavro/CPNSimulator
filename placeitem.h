#ifndef PLACEITEM_H
#define PLACEITEM_H

#include <QtGui>
#include "place.h"

class PlaceItem : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 10 };

    explicit PlaceItem();
    
    Place *place;

    int type() const{ return Type; }
signals:
    
public slots:
    
};

#endif // PLACEITEM_H
