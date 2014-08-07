/*
 *    Copyright 2013, 2014 Ondrej Navratil.
 *    This project and all of its contents are distributed under the terms of the GNU General Public License.
 *
 *    This file is part of CPNSimulator.
 *
 *    CPNSimulator is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published b
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    CPNSimulator is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with CPNSimulator. If not, see <http://www.gnu.org/licenses/>.
*/

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


public:
    explicit CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent = 0);

    CPNet *net;
    CPNetEditor *editor;
    Computer threadComputer;
    QWidget *overlay;
    QLabel *labelComputationDescription;
    QPushButton *buttonCancelComputation;
    int transitionsToFire;

    void loadNetGraph();

    void toInitialMarking();
    void toCurrentMarking();

    void setPlaceMarking(Place *place, Expression *expression);

    void findBindings();

    void fireTransitions(int count);

    void showOverlay(QString message);
    void hideOverlay();


signals:
    
public slots:
    void slotFire(SimulatorTransitionItem *sti);
    void slotGenerateStateSpace();

    void slotCancelComputation();
    void slotComputerCompleted();
    void slotComputerFailed(QString message);
};

#endif // CPNETSIMULATOR_H
