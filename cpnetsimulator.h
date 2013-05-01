#ifndef CPNETSIMULATOR_H
#define CPNETSIMULATOR_H

#include <QWidget>
#include "cpneteditor.h"
#include "simulatorscene.h"
#include "computer.h"

class CPNetSimulator : public QWidget
{
    Q_OBJECT

protected:
    static int randInt(int low, int high);

    SimulatorScene *scene;
    QGraphicsView *view;
    QGridLayout *layout;

    QList<Binding> mergeBindings(QList<Binding> possibleBindings, QList<QList<Binding> > arcBindings);

    int transitionsToFire;

public:
    explicit CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent = 0);

    CPNet *net;
    CPNetEditor *editor;
    Computer threadComputer;
    QWidget *overlay;
    QLabel *labelComputationDescription;
    QPushButton *buttonCancelComputation;

    void loadNetGraph();

    void toInitialMarking();

    void findBindings();

    void fireTransitions(int count);

    void showOverlay(QString message);
    void hideOverlay();

signals:
    
public slots:
    void slotFire(SimulatorTransitionItem *sti);
    void slotCancelComputation();
    void slotComputerCompleted();
    void slotComputerFailed(QString message);
};

#endif // CPNETSIMULATOR_H
