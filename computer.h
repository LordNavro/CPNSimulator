#ifndef COMPUTER_H
#define COMPUTER_H

#include <QThread>
#include "cpnet.h"
#include "interpret.h"
#include "simulatortransitionitem.h"
#include "analyzer.h"

class Computer : public QThread
{
    Q_OBJECT
public:
    explicit Computer(CPNet *net, QObject *parent = 0);
    ~Computer();
    CPNet *net;
    bool cancelRequest;
    SimulatorTransitionItem *sti;
    StateSpaceGraph graph;
    enum Mode{FindBinding, FireTransition, GenerateStateSpace};
    Mode mode;

protected:
    void run();
    void findBinding(NetMarking marking);
    NetMarking fireTransition(NetMarking marking, Transition *transition, Binding binding);
    void generateStateSpace();

    QList<Binding> mergeBindings(QList<Binding> possibleBindings, QList<QList<Binding> > arcBindings);

signals:
    void signalCompleted();
    void signalFailed(QString message);

public slots:
    
};

#endif // COMPUTER_H
