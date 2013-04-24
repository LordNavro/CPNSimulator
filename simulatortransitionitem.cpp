#include "simulatortransitionitem.h"


SimulatorTransitionItem::SimulatorTransitionItem(EditorTransitionItem *eti)
{
    setRect(eti->rect());
    setPos(eti->pos());
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    transition = eti->transition;
    proxyBinding = new QGraphicsProxyWidget(this);
    comboBinding = new QComboBox;
    comboBinding->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    proxyBinding->setWidget(comboBinding);
    proxyBinding->setPos(42, -10);
    proxyFire = new QGraphicsProxyWidget(this);
    buttonFire = new QPushButton("Fire");
    proxyFire->setWidget(buttonFire);
    proxyFire->setPos(42, -40);
}

void SimulatorTransitionItem::populateCombo()
{
    comboBinding->clear();
    foreach(Binding binding, transition->possibleBindings)
        comboBinding->addItem(binding.toString());
    if(transition->possibleBindings.isEmpty())
    {
        comboBinding->setVisible(false);
        buttonFire->setVisible(false);
    }
    else
    {
        comboBinding->setVisible(true);
        buttonFire->setVisible(true);
    }
}

