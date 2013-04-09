#ifndef CPNETEDITOR_H
#define CPNETEDITOR_H

#include <QtGui>
#include "cpnet.h"
#include "cpnetscene.h"
#include "cpnetform.h"
#include "placeform.h"
#include "transitionform.h"
#include "arcform.h"
#include "compiler.h"
#include <QtXml/QDomDocument>

class CPNetEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CPNetEditor(QWidget *parent = 0);

    CPNetScene *scene;
    QGraphicsView *view;
    QTableWidget *table;

    CPNetForm *formNet;
    PlaceForm *formPlace;
    TransitionForm *formTransition;
    ArcForm *formArc;

    QGridLayout *layout;

    QString fileName;

    void compile(void);
    QDomDocument netToXml();
    void xmlToNet(QDomDocument xml);

signals:

public slots:
    void slotSelectionChanged();
    void slotCellClicked(int x, int y);
    
};

#endif // CPNETEDITOR_H
