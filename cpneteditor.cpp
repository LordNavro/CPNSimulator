#include "cpneteditor.h"

CPNetEditor::CPNetEditor(QWidget *parent) :
    QWidget(parent)
{
    view = new QGraphicsView(this);
    scene = new CPNetScene(this);
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view->setScene(scene);
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
    table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setMaximumHeight(100);

    QStringList labels;
    labels.append(tr("Compilation output"));
    table->setHorizontalHeaderLabels(labels);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    table->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    table->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(16);

    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(slotCellClicked(int,int)));

    formNet = new CPNetForm(this);
    formPlace = new PlaceForm(this);
    formTransition = new TransitionForm(this);
    formArc = new ArcForm(this);

    layout = new QGridLayout(this);
    layout->addWidget(view, 0, 0);
    layout->addWidget(formNet,0,1);
    layout->addWidget(formPlace,0,1);
    layout->addWidget(formTransition,0,1);
    layout->addWidget(formArc,0,1);
    layout->addWidget(table, 1,0,1,2);

    setLayout(layout);
    slotSelectionChanged();
}

void CPNetEditor::compile()
{
    CPNet *net = &scene->net;
    net->compile();
    if(net->errorList.isEmpty())
    {
        table->setRowCount(1);
        QTableWidgetItem *item = new QTableWidgetItem(tr("Compilation completed succesfully."));
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(0, 0, item);
        return;
    }

    table->setRowCount(net->errorList.length());
    int row = 0;
    foreach(CPNet::Error error, net->errorList)
    {
        QString eType;
        switch(error.type)
        {
        case CPNet::LEXICAL:
            eType = tr("Lexical");
            break;
        case CPNet::SYNTACTIC:
            eType = tr("Syntax");
            break;
        case CPNet::SEMANTIC:
            eType = tr("Semantic");
            break;
        }

        QString eInscription;
        switch(error.inscription)
        {
        case CPNet::EXPRESSION:
            eInscription = tr("expression");
            break;
        case CPNet::GUARD:
            eInscription = tr("guard");
            break;
        case CPNet::CURRENT:
            eInscription = tr("current marking");
            break;
        case CPNet::INITIAL:
            eInscription = tr("initial marking");
            break;
        case CPNet::DECLARATION:
            eInscription = tr("declaration");
            break;
        }
        QString eItem;
        switch(error.item)
        {
        case CPNet::NET:
            eItem = tr("net ");
            eItem += error.reference.net->name;
            break;
        case CPNet::PLACE:
            eItem = tr("place ");
            eItem += error.reference.place->name;
            break;
        case CPNet::TRANSITION:
            eItem = tr("transition");
            eItem += error.reference.transition->name;
            break;
        case CPNet::ARC:
            eItem = tr("arc");
            eItem += error.reference.arc->place->name + " <-> " + error.reference.arc->transition->name;
            break;
        }

        QString message = tr("%1 error on line %2 in %3 of %4: %5").arg(eType, QString::number(error.lineNo),
                                                                        eInscription, eItem, error.message);

        QTableWidgetItem *item = new QTableWidgetItem(message);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(row++, 0, item);
    }
}

void CPNetEditor::slotSelectionChanged()
{
    formNet->hide();
    formPlace->hide();
    formTransition->hide();
    formArc->hide();
    QList<QGraphicsItem *> selected = scene->selectedItems();
    if(selected.count() != 1)
    {
        formNet->setNet(&scene->net);
        formNet->show();
        return;
    }
    QGraphicsItem *item = selected.first();
    if(PlaceItem *placeItem = qgraphicsitem_cast<PlaceItem *>(item))
    {
        formPlace->setPlaceItem(placeItem);
        formPlace->show();
    }
    else if(TransitionItem *transitionItem = qgraphicsitem_cast<TransitionItem *>(item))
    {
        formTransition->setTransitionItem(transitionItem);
        formTransition->show();
    }
    else if(ArcItem *arcItem = qgraphicsitem_cast<ArcItem *>(item))
    {
        formArc->setArcItem(arcItem);
        formArc->show();
    }
}

void CPNetEditor::slotCellClicked(int x, int /*y*/)
{
    CPNet *net = &scene->net;
    if(x >= net->errorList.length())
        return;
    foreach(QGraphicsItem *item, scene->items())
        item->setSelected(false);
    CPNet::Error error = net->errorList[x];
    InscriptionEdit *i = NULL;
    switch(error.item)
    {
    case CPNet::NET:
        i = formNet->inscriptionDeclaration;
        break;
    case CPNet::PLACE:
        if(error.inscription == CPNet::CURRENT)
            i = formPlace->inscriptionCurrentMarking;
        else
            i = formPlace->inscriptionInitialMarking;
        foreach(QGraphicsItem *item, scene->items())
            if(PlaceItem *placeItem = qgraphicsitem_cast<PlaceItem *>(item))
                if(placeItem->place == error.reference.place)
                    placeItem->setSelected(true);
        break;
    case CPNet::TRANSITION:
        i = formTransition->inscriptionGuard;
        foreach(QGraphicsItem *item, scene->items())
            if(TransitionItem *transitionItem = qgraphicsitem_cast<TransitionItem *>(item))
                if(transitionItem->transition == error.reference.transition)
                    transitionItem->setSelected(true);
        break;
    case CPNet::ARC:
        i = formArc->inscriptionExpression;
        foreach(QGraphicsItem *item, scene->items())
            if(ArcItem *arcItem = qgraphicsitem_cast<ArcItem *>(item))
                if(arcItem->arc == error.reference.arc)
                    arcItem->setSelected(true);
        break;
    }
    if(i)
    {
        QTextCursor c;
        i->setFocus();
        c = i->textCursor();
        c.setPosition(0);
        c.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, error.lineNo);
        i->setTextCursor(c);
    }
}
