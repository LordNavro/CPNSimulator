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
