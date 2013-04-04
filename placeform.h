#ifndef PLACEFORM_H
#define PLACEFORM_H

#include <QtGui>
#include "placeitem.h"
#include "inscriptionedit.h"

class PlaceForm : public QWidget
{
    Q_OBJECT
private:
    PlaceItem *placeItem;
public:
    explicit PlaceForm(QWidget *parent = 0);
    void setPlaceItem(PlaceItem *placeItem);

    QFormLayout *layout;
    QLineEdit *lineName;
    QComboBox *comboColourSet;
    InscriptionEdit *inscriptionInitialMarking;
    InscriptionEdit *inscriptionCurrentMarking;

signals:
    void compilationNeeded();
public slots:
    void nameChanged(QString name);
    void colourSetChanged(int colourSet);
    void initialMarkingChanged(void);
    void currentMarkingChanged(void);
    
};

#endif // PLACEFORM_H
