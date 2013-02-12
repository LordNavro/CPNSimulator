#include "arcform.h"

ArcForm::ArcForm(QWidget *parent) :
    QWidget(parent)
{
}

void ArcForm::setArcItem(ArcItem *arcItem)
{
    this->arcItem = arcItem;
}
