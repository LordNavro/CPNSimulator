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

#ifndef ARCFORM_H
#define ARCFORM_H

#include <QtGui>
#include "editorarcitem.h"
#include "inscriptionedit.h"

class ArcForm : public QWidget
{
    Q_OBJECT
private:
    EditorArcItem *arcItem;
public:
    explicit ArcForm(QWidget *parent = 0);
    
    QFormLayout *layout;
    InscriptionEdit *inscriptionExpression;

    void setArcItem(EditorArcItem *arcItem);
signals:

public slots:
    void expressionChanged();
    
};

#endif // ARCFORM_H
