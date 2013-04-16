#ifndef EDITORTRANSITIONITEM_H
#define EDITORTRANSITIONITEM_H

#include "transitionitem.h"

class EditorTransitionItem : public TransitionItem
{
public:
    enum { Type = UserType + 21 };

    explicit EditorTransitionItem();

    int type() const{ return Type; }
    
};

#endif // EDITORTRANSITIONITEM_H
