#include "transition.h"

Transition::Transition() :
    parsedGuard(NULL)
{
}

Transition::~Transition()
{
    //qDebug() << "transition destroyed";
    if(parsedGuard)
        delete parsedGuard;
}
