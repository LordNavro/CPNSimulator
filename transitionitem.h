#ifndef TRANSITIONITEM_H
#define TRANSITIONITEM_H

#include <QtGui>
#include "transition.h"
#include "arcitem.h"

class TransitionItem : public QGraphicsRectItem
{
public:
    enum { Type = UserType + 20 };

    explicit TransitionItem();

    Transition *transition;
    QList<ArcItem *>arcItems;

    int type() const{ return Type; }

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
    
};

#endif // TRANSITIONITEM_H
