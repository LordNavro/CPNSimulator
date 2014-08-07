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

#ifndef CPNETEDITOR_H
#define CPNETEDITOR_H

#include <QtGui>
#include "cpnet.h"
#include "editorscene.h"
#include "cpnetform.h"
#include "placeform.h"
#include "transitionform.h"
#include "arcform.h"
#include "compiler.h"
#include <QtXml/QDomDocument>

class CPNetEditor : public QWidget
{
    Q_OBJECT
protected:
    CPNet *net;
public:
    explicit CPNetEditor(CPNet* net, QWidget *parent = 0);

    EditorScene *scene;
    QGraphicsView *view;
    QTableWidget *table;

    CPNetForm *formNet;
    PlaceForm *formPlace;
    TransitionForm *formTransition;
    ArcForm *formArc;

    QGridLayout *layout;

    QString fileName;

    void setCompilationOutput(void);
    QDomDocument netToXml();
    void xmlToNet(QDomDocument xml);

    void setNet(CPNet *net);
    CPNet *getNet();

signals:
public slots:
    void slotSelectionChanged();
    void slotCellClicked(int x, int y);
    
};

#endif // CPNETEDITOR_H
