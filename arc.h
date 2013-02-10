#ifndef ARC_H
#define ARC_H

#include <QObject>
#include "place.h"
#include "transition.h"

class Arc : public QObject
{
    Q_OBJECT
public:
    explicit Arc(QObject *parent = 0);
    
    QString arcExpression;
    Place *place;
    Transition *transition;
    bool isPreset;
signals:
    
public slots:
    
};

#endif // ARC_H
