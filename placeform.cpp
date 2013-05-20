#include "placeform.h"

PlaceForm::PlaceForm(QWidget *parent) :
    QWidget(parent)
{
    placeItem = NULL;
    lineName = new QLineEdit(this);
    connect(lineName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));
    comboColourSet = new QComboBox(this);
    comboColourSet->addItem(tr("Unit"));
    comboColourSet->addItem(tr("Boolean"));
    comboColourSet->addItem(tr("Integer"));
    connect(comboColourSet, SIGNAL(currentIndexChanged(int)), this, SLOT(colourSetChanged(int)));
    inscriptionInitialMarking = new InscriptionEdit(this);
    connect(inscriptionInitialMarking, SIGNAL(textChanged()), this, SLOT(initialMarkingChanged()));
    inscriptionCurrentMarking = new InscriptionEdit(this);
    connect(inscriptionCurrentMarking, SIGNAL(textChanged()), this, SLOT(currentMarkingChanged()));

    layout = new QFormLayout(this);
    layout->addRow(tr("Place &name"), lineName);
    layout->addRow(tr("C&olour set"), comboColourSet);
    layout->addRow(tr("&Initial marking"), inscriptionInitialMarking);
    layout->addRow(tr("&Current marking"), inscriptionCurrentMarking);

}

void PlaceForm::setPlaceItem(EditorPlaceItem *placeItem)
{
    this->placeItem = placeItem;
    lineName->setText(placeItem->place->name);
    comboColourSet->setCurrentIndex(placeItem->place->colourSet);
    inscriptionInitialMarking->setText(placeItem->place->initialMarking);
    inscriptionCurrentMarking->setText(placeItem->place->currentMarking);
}

void PlaceForm::nameChanged(QString name)
{
    placeItem->place->name = name;
}


void PlaceForm::colourSetChanged(int colourSet)
{
    switch(colourSet)
    {
    case 0:
        placeItem->place->colourSet = Place::UNIT;
        break;
    case 1:
        placeItem->place->colourSet = Place::BOOL;
        break;
    case 2:
        placeItem->place->colourSet = Place::INT;
        break;
    }
}

void PlaceForm::initialMarkingChanged()
{
    if(placeItem == NULL)
        return;
    placeItem->place->initialMarking = inscriptionInitialMarking->document()->toPlainText();
}

void PlaceForm::currentMarkingChanged()
{
    if(placeItem == NULL)
        return;
    placeItem->place->currentMarking = inscriptionCurrentMarking->document()->toPlainText();
}
