#ifndef CPNETSIMULATOR_H
#define CPNETSIMULATOR_H

#include <QWidget>
#include "cpneteditor.h"
#include "simulatorscene.h"

class CPNetSimulator : public QWidget
{
    Q_OBJECT

protected:
    static int randInt(int low, int high);

    SimulatorScene *scene;
    QGraphicsView *view;
    QGridLayout *layout;

    QList<Binding> mergeBindings(QList<Binding> possibleBindings, QList<QList<Binding> > arcBindings);

public:
    explicit CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent = 0);

    CPNet *net;
    CPNetEditor *editor;

    void loadNetGraph();

    void toInitialMarking();

    void findBindings();

    void fireEvents(int count);

signals:
    
public slots:
    void slotFire(SimulatorTransitionItem *sti);
};

#endif // CPNETSIMULATOR_H
