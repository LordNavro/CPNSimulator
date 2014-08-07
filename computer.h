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
