#include "place.h"

Place::Place(QObject *parent, CPNet *net) :
    QObject(parent), net(net)
{
    colourSet = UNIT;
}
