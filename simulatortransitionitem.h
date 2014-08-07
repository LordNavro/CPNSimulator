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

#ifndef SIMULATORTRANSITIONITEM_H
#define SIMULATORTRANSITIONITEM_H

#include "editortransitionitem.h"


class SimulatorTransitionItem : public QObject, public TransitionItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 22 };

    explicit SimulatorTransitionItem(EditorTransitionItem *eti);

    QGraphicsProxyWidget *proxyBinding;
    QGraphicsProxyWidget *proxyFire;
    QComboBox *comboBinding;
    QPushButton *buttonFire;

    void populateCombo();

    int type() const{ return Type; }

public slots:
    void slotFire();
signals:
    void signalFire(SimulatorTransitionItem *item);
};

#endif // SIMULATORTRANSITIONITEM_H
