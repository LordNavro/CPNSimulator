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

#include "cpnetform.h"

CPNetForm::CPNetForm(QWidget *parent) :
    QWidget(parent)
{
    lineName = new QLineEdit(this);
    connect(lineName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));
    inscriptionDeclaration = new InscriptionEdit(this);
    connect(inscriptionDeclaration, SIGNAL(textChanged()), this, SLOT(declarationChanged()));

    layout = new QFormLayout(this);
    layout->addRow(tr("Net &name"), lineName);
    layout->addRow(tr("Net &declarations"), inscriptionDeclaration);
    layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    this->setLayout(layout);
}

void CPNetForm::setNet(CPNet *net)
{
    this->net = net;
    lineName->setText(net->name);
    inscriptionDeclaration->setText(net->declaration);
}

void CPNetForm::nameChanged(QString name)
{
    net->name = name;
}

void CPNetForm::declarationChanged()
{
    net->declaration = inscriptionDeclaration->document()->toPlainText();
}
