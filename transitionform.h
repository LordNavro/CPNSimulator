#ifndef TRANSITIONFORM_H
#define TRANSITIONFORM_H

#include <QtGui>
#include "editortransitionitem.h"
#include "inscriptionedit.h"

class TransitionForm : public QWidget
{
    Q_OBJECT
private:
    EditorTransitionItem *transitionItem;
public:
    explicit TransitionForm(QWidget *parent = 0);

    QFormLayout *layout;
    QLineEdit *lineName;
    InscriptionEdit *inscriptionGuard;
    
    void setTransitionItem(EditorTransitionItem *transitionItem);
signals:

public slots:
    void nameChanged(QString name);
    void guardChanged();
    
};

#endif // TRANSITIONFORM_H
