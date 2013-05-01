#ifndef COMPUTER_H
#define COMPUTER_H

#include <QThread>
#include "cpnet.h"
#include "interpret.h"
#include "simulatortransitionitem.h"

class Computer : public QThread
{
    Q_OBJECT
public:
    explicit Computer(CPNet *net, QObject *parent = 0);
    bool cancelRequest;
    CPNet *net;
    SimulatorTransitionItem *sti;
    enum Mode{FindBinding, FireTransition};
    Mode mode;

protected:
    void run();
    void findBinding();
    void fireTransition();

    QList<Binding> mergeBindings(QList<Binding> possibleBindings, QList<QList<Binding> > arcBindings);

signals:
    void signalCompleted();
    void signalFailed(QString message);

public slots:
    
};

#endif // COMPUTER_H
