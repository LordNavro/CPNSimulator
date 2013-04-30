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

void Place::subtract(Data data)
{
    if(colourSet == Place::UNIT)
        currentMarkingValue->value.multiUnit -= data.value.multiUnit;
    else if(colourSet == Place::BOOL)
    {
        currentMarkingValue->value.multiBool.t -= data.value.multiBool.t;
        currentMarkingValue->value.multiBool.f -= data.value.multiBool.f;
    }
    else if(colourSet == Place::INT)
    {
        QMapIterator<int, int>i(*data.value.multiInt);
        while(i.hasNext())
        {
            i.next();
            (*currentMarkingValue->value.multiInt)[i.key()] -= i.value();
            if(currentMarkingValue->value.multiInt->value(i.key()) <= 0)
                currentMarkingValue->value.multiInt->remove(i.key());
        }
    }
}

void Place::add(Data data)
{
    if(colourSet == Place::UNIT)
        currentMarkingValue->value.multiUnit += data.value.multiUnit;
    else if(colourSet == Place::BOOL)
    {
        currentMarkingValue->value.multiBool.t += data.value.multiBool.t;
        currentMarkingValue->value.multiBool.f += data.value.multiBool.f;
    }
    else if(colourSet == Place::INT)
    {
        QMapIterator<int, int>i(*data.value.multiInt);
        while(i.hasNext())
        {
            i.next();
            (*currentMarkingValue->value.multiInt)[i.key()] += i.value();
            if(currentMarkingValue->value.multiInt->value(i.key()) <= 0)
                currentMarkingValue->value.multiInt->remove(i.key());
        }
    }
}
