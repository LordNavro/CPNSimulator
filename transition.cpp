#include "transition.h"

Transition::Transition() :
    parsedGuard(NULL)
{
}

Transition::~Transition()
{
    if(parsedGuard)
        delete parsedGuard;
}
