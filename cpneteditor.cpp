#include "cpneteditor.h"
#include <QtXml/QDomDocument>

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
    net->syntaxAnalysis();
    net->semanticAnalysis();
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


QDomDocument CPNetEditor::netToXml()
{
    CPNet *net = &scene->net;
    QDomDocument xml("ColouredPetriNet");
    /* net */
    QDomElement root = xml.createElement("cpnet");
    root.setAttribute("name", net->name);
    QDomElement declaration = xml.createElement("declaration");
    QDomCDATASection declarationData = xml.createCDATASection(net->declaration);
    declaration.appendChild(declarationData);
    root.appendChild(declaration);

    /* places */
    QDomElement places = xml.createElement("places");
    int i = 0;
    foreach(Place *place, net->places)
    {
        QDomElement placeElem = xml.createElement("place");
        PlaceItem *placeItem = scene->getPlaceItem(place);
        QDomElement initial = xml.createElement("initialMarking");
        QDomElement current = xml.createElement("currentMarking");
        QDomCDATASection initialData = xml.createCDATASection(place->initialMarking);
        QDomCDATASection currentData = xml.createCDATASection(place->currentMarking);
        initial.appendChild(initialData);
        current.appendChild(currentData);
        placeElem.appendChild(initial);
        placeElem.appendChild(current);
        placeElem.setAttribute("name", place->name);
        placeElem.setAttribute("x", placeItem->x());
        placeElem.setAttribute("y", placeItem->y());
        placeElem.setAttribute("colourSet", place->colourSet);
        placeElem.setAttribute("placeId", i++);
        places.appendChild(placeElem);
    }
    root.appendChild(places);

    /* transitions */
    QDomElement transitions = xml.createElement("transitions");
    i = 0;
    foreach(Transition *transition, net->transitions)
    {
        QDomElement transitionElem = xml.createElement("transition");
        TransitionItem *transitionItem = scene->getTransitionItem(transition);
        transitions.appendChild(transitionElem);
        QDomElement guard = xml.createElement("guard");
        QDomCDATASection guardData = xml.createCDATASection(transition->guard);
        guard.appendChild(guardData);
        transitionElem.appendChild(guard);
        transitionElem.setAttribute("name", transition->name);
        transitionElem.setAttribute("x", transitionItem->x());
        transitionElem.setAttribute("y", transitionItem->y());
        transitionElem.setAttribute("transitionId", i++);
        transitions.appendChild(transitionElem);
    }
    root.appendChild(transitions);

    /* arcs */
    QDomElement arcs = xml.createElement("arcs");
    i = 0;
    foreach(Arc *arc, net->arcs)
    {
        QDomElement arcElem = xml.createElement("arc");
        //ArcItem *arcItem = scene->getArcItem(arc);
        QDomElement expression = xml.createElement("expression");
        QDomCDATASection expressionData = xml.createCDATASection(arc->expression);
        expression.appendChild(expressionData);
        arcElem.appendChild(expression);
        arcElem.setAttribute("arcId", i++);
        arcElem.setAttribute("placeId", net->places.indexOf(arc->place));
        arcElem.setAttribute("transitionId", net->transitions.indexOf(arc->transition));
        arcElem.setAttribute("isPreset", arc->isPreset);
        arcs.appendChild(arcElem);
    }
    root.appendChild(arcs);

    xml.appendChild(root);

    return xml;
}

void CPNetEditor::xmlToNet(QDomDocument xml)
{
    /* clear current net */
    CPNet *net = &scene->net;
    qDeleteAll(net->places);
    qDeleteAll(net->transitions);
    qDeleteAll(net->arcs);
    net->places.clear();
    net->transitions.clear();
    net->arcs.clear();
    qDeleteAll(scene->items());

    QDomElement root = xml.firstChildElement("cpnet");
    /* net */
    net->name = root.attribute("name");
    net->declaration = root.firstChildElement("declaration").firstChild().toCDATASection().data();


    /* places */
    QDomElement places = root.firstChildElement("places");
    for(int i = 0; i < places.childNodes().size(); i++)
    {
        QDomElement placeElem = places.childNodes().at(i).toElement();
        Place *place = new Place(this);
        PlaceItem *placeItem = new PlaceItem();
        placeItem->place = place;
        net->places.append(place);
        placeItem->setX(placeElem.attribute("x").toDouble());
        placeItem->setY(placeElem.attribute("y").toDouble());
        place->name = placeElem.attribute("name");
        switch(placeElem.attribute("colourSet").toInt())
        {
        case 0:
            place->colourSet = Place::UNIT;
        case 1:
            place->colourSet = Place::BOOL;
        default:
            place->colourSet = Place::INT;
        }
        place->initialMarking = placeElem.firstChildElement("initialMarking").firstChild().toCDATASection().data();
        place->currentMarking = placeElem.firstChildElement("currentMarking").firstChild().toCDATASection().data();
        scene->addItem(placeItem);
    }

    /* transitions */
    QDomElement transitions = root.firstChildElement("transitions");
    for(int i = 0; i < transitions.childNodes().size(); i++)
    {
        QDomElement transitionElem = transitions.childNodes().at(i).toElement();
        Transition *transition = new Transition(this);
        TransitionItem *transitionItem = new TransitionItem();
        transitionItem->transition = transition;
        net->transitions.append(transition);
        transitionItem->setX(transitionElem.attribute("x").toDouble());
        transitionItem->setY(transitionElem.attribute("y").toDouble());
        transition->name = transitionElem.attribute("name");
        transition->guard = transitionElem.firstChildElement("guard").firstChild().toCDATASection().data();
        scene->addItem(transitionItem);
    }

    /* arcs */
    QDomElement arcs = root.firstChildElement("arcs");
    for(int i = 0; i < arcs.childNodes().size(); i++)
    {
        QDomElement arcElem = arcs.childNodes().at(i).toElement();
        Arc *arc = new Arc(this);
        net->arcs.append(arc);
        arc->expression = arcElem.firstChildElement("expression").firstChild().toCDATASection().data();
        arc->place = net->places[arcElem.attribute("placeId").toInt()];
        arc->transition = net->transitions[arcElem.attribute("transitionId").toInt()];
        arc->isPreset = arcElem.attribute("isPreset").toInt();
        ArcItem *arcItem;
        if(arc->isPreset)
            arcItem = new ArcItem(scene->getPlaceItem(arc->place), scene->getTransitionItem(arc->transition));
        else
            arcItem = new ArcItem(scene->getTransitionItem(arc->transition), scene->getPlaceItem(arc->place));

        arcItem->arc = arc;
        scene->addItem(arcItem);
        scene->getPlaceItem(arc->place)->arcItems.append(arcItem);
        scene->getTransitionItem(arc->transition)->arcItems.append(arcItem);
    }
    slotSelectionChanged();
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
        if(PlaceItem *placeItem = scene->getPlaceItem(error.reference.place))
            placeItem->setSelected(true);
        break;
    case CPNet::TRANSITION:
        i = formTransition->inscriptionGuard;
        if(TransitionItem *transitionItem = scene->getTransitionItem(error.reference.transition))
            transitionItem->setSelected(true);
        break;
    case CPNet::ARC:
        i = formArc->inscriptionExpression;
        if(ArcItem *arcItem = scene->getArcItem(error.reference.arc))
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
