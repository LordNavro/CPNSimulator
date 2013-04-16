#ifndef EDITORARCITEM_H
#define EDITORARCITEM_H

#include <QtGui>
#include "arcitem.h"

class EditorArcItem : public ArcItem
{
public:
    enum { Type = UserType + 31 };

    explicit EditorArcItem(QGraphicsItem *from, QGraphicsItem *to, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const{ return Type; }

};

#endif // EDITORARCITEM_H
