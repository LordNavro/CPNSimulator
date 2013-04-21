#include "place.h"

Place::Place() :
    parsedInitialMarking(NULL), parsedCurrentMarking(NULL), currentMarkingValue(NULL)
{
    colourSet = UNIT;
}

Place::~Place()
{
    if(parsedInitialMarking)
        delete parsedInitialMarking;
    if(parsedCurrentMarking)
        delete parsedCurrentMarking;
    if(currentMarkingValue)
        delete currentMarkingValue;
}
