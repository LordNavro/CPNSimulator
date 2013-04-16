#ifndef CPNETSIMULATOR_H
#define CPNETSIMULATOR_H

#include <QWidget>
#include "cpnet.h"

class CPNetSimulator : public QWidget
{
    Q_OBJECT
public:
    explicit CPNetSimulator(CPNet *net, QWidget *parent = 0);

    CPNet *net;

signals:
    
public slots:
    
};

#endif // CPNETSIMULATOR_H
