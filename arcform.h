#ifndef ARCFORM_H
#define ARCFORM_H

#include <QtGui>
#include "arcitem.h"
#include "inscriptionedit.h"

class ArcForm : public QWidget
{
    Q_OBJECT
private:
    ArcItem *arcItem;
public:
    explicit ArcForm(QWidget *parent = 0);
    
    QFormLayout *layout;
    InscriptionEdit *inscriptionExpression;

    void setArcItem(ArcItem *arcItem);
signals:
    void compilationNeeded();
public slots:
    void expressionChanged();
    
};

#endif // ARCFORM_H
