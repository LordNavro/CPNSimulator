#ifndef PLACE_H
#define PLACE_H

#include <QObject>

class Place : public QObject
{
    Q_OBJECT
public:
    explicit Place(QObject *parent = 0);
    
    typedef enum {UNIT, BOOL, INT} ColourSet;

    Place::ColourSet colourSet;
    QString initMarking;



signals:
    
public slots:
    
};

#endif // PLACE_H
