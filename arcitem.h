#ifndef ARCITEM_H
#define ARCITEM_H

#include <QtGui>
#include "arc.h"

class ArcItem : public QGraphicsItem
{
public:
    enum { Type = UserType + 30 };

    explicit ArcItem();

    Arc *arc;


    int type() const{ return Type; }
signals:

public slots:
};

#endif // ARCITEM_H
