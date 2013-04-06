#include "place.h"

Place::Place(QObject *parent) :
    QObject(parent), parsedInitialMarking(NULL), parsedCurrentMarking(NULL)
{
    colourSet = UNIT;
}
