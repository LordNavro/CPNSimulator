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

#include "arcform.h"

ArcForm::ArcForm(QWidget *parent) :
    QWidget(parent)
{
    arcItem = NULL;

    inscriptionExpression = new InscriptionEdit(this);
    connect(inscriptionExpression, SIGNAL(textChanged()), this, SLOT(expressionChanged()));

    layout = new QFormLayout(this);
    layout->addRow(tr("Arc &expression"), inscriptionExpression);
    this->setLayout(layout);
}

void ArcForm::setArcItem(EditorArcItem *arcItem)
{
    this->arcItem = arcItem;
    inscriptionExpression->setText(arcItem->arc->expression);
}

void ArcForm::expressionChanged()
{
    if(arcItem == NULL)
        return;
    arcItem->arc->expression = inscriptionExpression->document()->toPlainText();
    arcItem->update();
}
