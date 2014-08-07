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

#include "simulatortransitionitem.h"


SimulatorTransitionItem::SimulatorTransitionItem(EditorTransitionItem *eti) : QObject()
{
    setRect(eti->rect());
    setPos(eti->pos());
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    transition = eti->transition;
    proxyBinding = new QGraphicsProxyWidget(this);
    comboBinding = new QComboBox;
    comboBinding->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    proxyBinding->setWidget(comboBinding);
    proxyBinding->setPos(42, -10);
    proxyFire = new QGraphicsProxyWidget(this);
    buttonFire = new QPushButton("Fire");
    proxyFire->setWidget(buttonFire);
    proxyFire->setPos(42, -40);
    connect(buttonFire, SIGNAL(clicked()), this, SLOT(slotFire()));
}

void SimulatorTransitionItem::populateCombo()
{
    comboBinding->clear();
    foreach(Binding binding, transition->possibleBindings)
        comboBinding->addItem(binding.toString());
    if(transition->possibleBindings.isEmpty())
    {
        comboBinding->setVisible(false);
        buttonFire->setVisible(false);
    }
    else
    {
        comboBinding->setVisible(true);
        buttonFire->setVisible(true);
    }
}

void SimulatorTransitionItem::slotFire()
{
    emit signalFire(this);
}

