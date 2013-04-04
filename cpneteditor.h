#ifndef CPNETEDITOR_H
#define CPNETEDITOR_H

#include <QtGui>
#include "cpnet.h"
#include "cpnetscene.h"
#include "cpnetform.h"
#include "placeform.h"
#include "transitionform.h"
#include "arcform.h"

class CPNetEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CPNetEditor(QWidget *parent = 0);

    CPNetScene *scene;
    QGraphicsView *view;

    CPNetForm *formNet;
    PlaceForm *formPlace;
    TransitionForm *formTransition;
    ArcForm *formArc;

    QHBoxLayout *layout;

signals:
    void compilationNeeded();
public slots:
    void slotSelectionChanged();
    
};

#endif // CPNETEDITOR_H
