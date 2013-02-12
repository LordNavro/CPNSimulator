#ifndef PLACE_H
#define PLACE_H

#include <QObject>

class Place : public QObject
{
    Q_OBJECT
public:
    explicit Place(QObject *parent = 0);
    
    typedef enum {UNIT, BOOL, INT} ColourSet;

    QString name;
    Place::ColourSet colourSet;
    QString initialMarking;
    QString currentMarking;



signals:
    
public slots:
    
};

#endif // PLACE_H
