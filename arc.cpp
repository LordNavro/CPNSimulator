#include "arc.h"

Arc::Arc() :
    parsedExpression(NULL)
{
}

Arc::~Arc()
{
    if(parsedExpression)
        delete parsedExpression;
}
