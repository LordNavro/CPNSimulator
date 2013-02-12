#include "transitionform.h"

TransitionForm::TransitionForm(QWidget *parent) :
    QWidget(parent)
{
}

void TransitionForm::setTransitionItem(TransitionItem *transitionItem)
{
    this->transitionItem = transitionItem;
}
