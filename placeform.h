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

#ifndef PLACEFORM_H
#define PLACEFORM_H

#include <QtGui>
#include "editorplaceitem.h"
#include "inscriptionedit.h"

class PlaceForm : public QWidget
{
    Q_OBJECT
private:
    EditorPlaceItem *placeItem;
public:
    explicit PlaceForm(QWidget *parent = 0);
    void setPlaceItem(EditorPlaceItem *placeItem);

    QFormLayout *layout;
    QLineEdit *lineName;
    QComboBox *comboColourSet;
    InscriptionEdit *inscriptionInitialMarking;
    InscriptionEdit *inscriptionCurrentMarking;

signals:

public slots:
    void nameChanged(QString name);
    void colourSetChanged(int colourSet);
    void initialMarkingChanged(void);
    void currentMarkingChanged(void);
    
};

#endif // PLACEFORM_H
