#ifndef CPNETEDITOR_H
#define CPNETEDITOR_H

#include <QtGui>
#include "cpnet.h"
#include "cpnetscene.h"

class CPNetEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CPNetEditor(QWidget *parent = 0);

    CPNetScene *scene;
    QGraphicsView *view;
    CPNet net;

    QHBoxLayout *layout;

signals:
    
public slots:
    
};

#endif // CPNETEDITOR_H
