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

QList<Binding> Arc::findBindings()
{
    QList<Binding> bindings;
    return bindings;
}
