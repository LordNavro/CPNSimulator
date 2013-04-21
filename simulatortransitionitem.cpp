#include "simulatortransitionitem.h"

SimulatorTransitionItem::SimulatorTransitionItem()
    : TransitionItem()
{
    comboBinding = new QComboBox;
    comboBinding->addItem("test");
}

SimulatorTransitionItem::SimulatorTransitionItem(EditorTransitionItem *eti)
{
    setRect(eti->rect());
    setPos(eti->pos());
    transition = eti->transition;
    comboBinding = new QComboBox;
    proxyCombo = new QGraphicsProxyWidget(this);
    proxyCombo->setWidget(comboBinding);
}

