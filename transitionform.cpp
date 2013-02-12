#include "transitionform.h"

TransitionForm::TransitionForm(QWidget *parent) :
    QWidget(parent)
{
    transitionItem = NULL;
    lineName = new QLineEdit(this);
    connect(lineName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));
    inscriptionGuard = new InscriptionEdit(this);
    connect(inscriptionGuard, SIGNAL(textChanged()), this, SLOT(guardChanged()));

    layout = new QFormLayout(this);
    layout->addRow(tr("Transition &name"), lineName);
    layout->addRow(tr("&Guard"), inscriptionGuard);
}

void TransitionForm::setTransitionItem(TransitionItem *transitionItem)
{
    this->transitionItem = transitionItem;
    lineName->setText(transitionItem->transition->name);
    inscriptionGuard->setText(transitionItem->transition->guard);
}

void TransitionForm::nameChanged(QString name)
{
    if(this->transitionItem == NULL)
        return;
    transitionItem->transition->name = name;
}

void TransitionForm::guardChanged()
{
    if(this->transitionItem == NULL)
        return;
    transitionItem->transition->guard = inscriptionGuard->document()->toPlainText();
}
