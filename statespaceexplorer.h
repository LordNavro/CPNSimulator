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

#ifndef STATESPACEEXPLORER_H
#define STATESPACEEXPLORER_H

#include <QWidget>
#include "analyzer.h"

class StateSpaceExplorer : public QDialog
{
    Q_OBJECT
public:
    explicit StateSpaceExplorer(StateSpaceGraph graph, QWidget *parent = 0);

    StateSpaceGraph graph;

    QListWidget *listWidget;
    QTextEdit *textEdit;

signals:
    
public slots:
    void slotItemSelected(int i);
};

#endif // STATESPACEEXPLORER_H
