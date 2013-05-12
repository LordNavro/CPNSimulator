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
    friend class StateSpaceGraph;
public:
    explicit Computer(CPNet *net, QObject *parent = 0);
    ~Computer();
    CPNet *net;
    bool cancelRequest;
    SimulatorTransitionItem *sti;
    int depth;
    StateSpaceGraph graph;
    enum Mode{FindBinding, FireTransition, GenerateStateSpace, ToInitialMarking, ToCurrentMarking};
    Mode mode;
    int recursionCounter;

protected:
    void run();
    void findBinding(NetMarking marking);
    NetMarking fireTransition(NetMarking marking, Transition *transition, Binding binding);

    void setPlaceMarking(Place *place, Expression *expression);

    void toInitialMarking();
    void toCurrentMarking();

    QList<Binding> mergeBindings(QList<Binding> possibleBindings, QList<QList<Binding> > arcBindings);

signals:
    void signalCompleted();
    void signalFailed(QString message);

public slots:
};

#endif // COMPUTER_H
