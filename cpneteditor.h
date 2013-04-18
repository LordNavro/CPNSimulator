#ifndef CPNETEDITOR_H
#define CPNETEDITOR_H

#include <QtGui>
#include "cpnet.h"
#include "editorscene.h"
#include "cpnetform.h"
#include "placeform.h"
#include "transitionform.h"
#include "arcform.h"
#include "compiler.h"
#include <QtXml/QDomDocument>

class CPNetEditor : public QWidget
{
    Q_OBJECT
protected:
    CPNet *net;
public:
    explicit CPNetEditor(CPNet* net, QWidget *parent = 0);

    EditorScene *scene;
    QGraphicsView *view;
    QTableWidget *table;

    CPNetForm *formNet;
    PlaceForm *formPlace;
    TransitionForm *formTransition;
    ArcForm *formArc;

    QGridLayout *layout;

    QString fileName;

    void setCompilationOutput(void);
    QDomDocument netToXml();
    void xmlToNet(QDomDocument xml);

    void setNet(CPNet *net);
    CPNet *getNet();

signals:
public slots:
    void slotSelectionChanged();
    void slotCellClicked(int x, int y);
    
};

#endif // CPNETEDITOR_H
