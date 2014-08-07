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

#include "transitionform.h"

TransitionForm::TransitionForm(QWidget *parent) :
    QWidget(parent)
{
    transitionItem = NULL;
    lineName = new QLineEdit(this);
    connect(lineName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));
    inscriptionGuard = new InscriptionEdit(this);
    connect(inscriptionGuard, SIGNAL(textChanged()), this, SLOT(guardChanged()));

    layout = new QFormLayout(this);
    layout->addRow(tr("Transition &name"), lineName);
    layout->addRow(tr("&Guard"), inscriptionGuard);
}

void TransitionForm::setTransitionItem(EditorTransitionItem *transitionItem)
{
    this->transitionItem = transitionItem;
    lineName->setText(transitionItem->transition->name);
    inscriptionGuard->setText(transitionItem->transition->guard);
}

void TransitionForm::nameChanged(QString name)
{
    if(this->transitionItem == NULL)
        return;
    transitionItem->transition->name = name;
    transitionItem->update();
}

void TransitionForm::guardChanged()
{
    if(this->transitionItem == NULL)
        return;
    transitionItem->transition->guard = inscriptionGuard->document()->toPlainText();
    transitionItem->update();
}
