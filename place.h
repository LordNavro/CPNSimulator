#ifndef PLACE_H
#define PLACE_H

#include <QObject>
#include "compiler.h"


class Place : public QObject
{
    Q_OBJECT
public:
    explicit Place(QObject *parent);
    
    typedef enum {UNIT, BOOL, INT} ColourSet;

    QString name;
    Place::ColourSet colourSet;
    QString initialMarking;
    QString currentMarking;

signals:
    
public slots:
    
};

#endif // PLACE_H
