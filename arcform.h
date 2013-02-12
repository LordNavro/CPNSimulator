#ifndef ARCFORM_H
#define ARCFORM_H

#include <QtGui>
#include "arcitem.h"

class ArcForm : public QWidget
{
    Q_OBJECT
private:
    ArcItem *arcItem;
public:
    explicit ArcForm(QWidget *parent = 0);
    
    void setArcItem(ArcItem *arcItem);
signals:
    
public slots:
    
};

#endif // ARCFORM_H
