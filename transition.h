#ifndef TRANSITION_H
#define TRANSITION_H

#include <QObject>
#include "compiler.h"

class CPNet;

class Transition : public QObject
{
    Q_OBJECT
public:
    explicit Transition(QObject *parent = 0);
    
    QString name;
    QString guard;

    Expression *parsedGuard;

signals:
    
public slots:
    
};

#endif // TRANSITION_H
