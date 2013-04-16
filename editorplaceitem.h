#ifndef EDITORPLACEITEM_H
#define EDITORPLACEITEM_H

#include <QtGui>
#include "place.h"
#include "placeitem.h"

class EditorPlaceItem : public PlaceItem
{
public:
    enum { Type = UserType + 11 };

    explicit EditorPlaceItem();

    int type() const{ return Type; }

};

#endif // EDITORPLACEITEM_H
