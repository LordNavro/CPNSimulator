#ifndef TRANSITIONITEM_H
#define TRANSITIONITEM_H

#include <QGraphicsRectItem>
#include "transition.h"

class TransitionItem : public QGraphicsRectItem
{
public:
    enum { Type = UserType + 20 };

    explicit TransitionItem();
    
    Transition *transition;

    int type() const{ return Type; }

signals:
    
public slots:
    
};

#endif // TRANSITIONITEM_H
