#ifndef TRANSITIONFORM_H
#define TRANSITIONFORM_H

#include <QtGui>
#include "transitionitem.h"

class TransitionForm : public QWidget
{
    Q_OBJECT
private:
    TransitionItem *transitionItem;
public:
    explicit TransitionForm(QWidget *parent = 0);
    
    void setTransitionItem(TransitionItem *transitionItem);
signals:
    
public slots:
    
};

#endif // TRANSITIONFORM_H
