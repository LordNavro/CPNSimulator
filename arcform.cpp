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
}
