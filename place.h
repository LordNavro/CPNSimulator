#ifndef PLACE_H
#define PLACE_H

#include "compiler.h"


class Place
{
public:
    explicit Place();
    ~Place();

    typedef enum {UNIT, BOOL, INT} ColourSet;

    QString name;
    Place::ColourSet colourSet;
    QString initialMarking;
    QString currentMarking;

    Expression *parsedInitialMarking;
    Expression *parsedCurrentMarking;

    Data *currentMarkingValue;

signals:
    
public slots:
    
};

#endif // PLACE_H
