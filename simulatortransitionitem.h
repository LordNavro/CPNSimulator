#ifndef SIMULATORTRANSITIONITEM_H
#define SIMULATORTRANSITIONITEM_H

#include "editortransitionitem.h"


class SimulatorTransitionItem : public TransitionItem
{
public:
    enum { Type = UserType + 22 };

    explicit SimulatorTransitionItem(EditorTransitionItem *eti);

    QGraphicsProxyWidget *proxyBinding;
    QGraphicsProxyWidget *proxyFire;
    QComboBox *comboBinding;
    QPushButton *buttonFire;

    void populateCombo();

    int type() const{ return Type; }
};

#endif // SIMULATORTRANSITIONITEM_H
