#ifndef SIMULATORTRANSITIONITEM_H
#define SIMULATORTRANSITIONITEM_H

#include "editortransitionitem.h"


class SimulatorTransitionItem : public QObject, public TransitionItem
{
    Q_OBJECT
public:
    enum { Type = UserType + 22 };

    explicit SimulatorTransitionItem(EditorTransitionItem *eti);

    QGraphicsProxyWidget *proxyBinding;
    QGraphicsProxyWidget *proxyFire;
    QComboBox *comboBinding;
    QPushButton *buttonFire;

    void populateCombo();

    int type() const{ return Type; }

public slots:
    void slotFire();
signals:
    void signalFire(SimulatorTransitionItem *item);
};

#endif // SIMULATORTRANSITIONITEM_H
