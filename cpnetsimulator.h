#ifndef CPNETSIMULATOR_H
#define CPNETSIMULATOR_H

#include <QWidget>
#include "cpneteditor.h"
#include "simulatorscene.h"

class CPNetSimulator : public QWidget
{
    Q_OBJECT

protected:

    SimulatorScene *scene;
    QGraphicsView *view;
    QGridLayout *layout;

public:
    explicit CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent = 0);

    CPNet *net;
    CPNetEditor *editor;

    void loadNetGraph();

signals:
    
public slots:
    
};

#endif // CPNETSIMULATOR_H
