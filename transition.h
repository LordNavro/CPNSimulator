#ifndef TRANSITION_H
#define TRANSITION_H

#include <QObject>

class Transition : public QObject
{
    Q_OBJECT
public:
    explicit Transition(QObject *parent = 0);
    
    QString name;
    QString guard;
signals:
    
public slots:
    
};

#endif // TRANSITION_H
