#include "cpnetsimulator.h"
#include "interpret.h"

CPNetSimulator::CPNetSimulator(CPNet *net, CPNetEditor *editor, QWidget *parent) :
    QWidget(parent), net(net), editor(editor)
{
    scene = new SimulatorScene(net, this);
    view = new QGraphicsView(scene, this);

    layout = new QGridLayout(this);
    layout->addWidget(view, 0, 0);
    this->setLayout(layout);
}

void CPNetSimulator::loadNetGraph()
{
    qDeleteAll(scene->items());
    foreach(Place *place, net->places)
    {
        EditorPlaceItem *epi = editor->scene->getPlaceItem(place);
        SimulatorPlaceItem *spi = new SimulatorPlaceItem(epi);
        scene->addItem(spi);
        if(spi->place->currentMarkingValue)
            delete spi->place->currentMarkingValue;
        if(spi->place->parsedCurrentMarking)
            spi->place->currentMarkingValue = new Data(eval(spi->place->parsedCurrentMarking, NULL, NULL));
        else
            switch(spi->place->colourSet)
            {
            case Place::UNIT:
                spi->place->currentMarkingValue = new Data(Data::MULTIUNIT);
                spi->place->currentMarkingValue->value.multiUnit = 0;
            case Place::BOOL:
                spi->place->currentMarkingValue = new Data(Data::MULTIBOOL);
                spi->place->currentMarkingValue->value.multiBool.f = 0;
                spi->place->currentMarkingValue->value.multiBool.t = 0;
            case Place::INT:
                spi->place->currentMarkingValue = new Data(Data::MULTIINT);
            }
    }
    foreach(Transition *transition, net->transitions)
    {
        EditorTransitionItem *eti = editor->scene->getTransitionItem(transition);
        SimulatorTransitionItem *sti = new SimulatorTransitionItem(eti);
        scene->addItem(sti);
    }
    foreach(Arc *arc, net->arcs)
    {
        QGraphicsItem *from = scene->getPlaceItem(arc->place);
        QGraphicsItem *to = scene->getTransitionItem(arc->transition);

        if(!arc->isPreset)
            qSwap(from, to);
        SimulatorArcItem *sai = new SimulatorArcItem(from, to);
        sai->arc = arc;
        scene->addItem(sai);
        scene->getPlaceItem(arc->place)->arcItems.append(sai);
        scene->getTransitionItem(arc->transition)->arcItems.append(sai);
    }
    findBindings();
}

void CPNetSimulator::toInitialMarking()
{
    foreach(Place *place, net->places)
    {
        if(place->currentMarkingValue)
            delete place->currentMarkingValue;
        if(place->parsedInitialMarking)
           place->currentMarkingValue = new Data(eval(place->parsedInitialMarking, NULL, NULL));
        else
            switch(place->colourSet)
            {
            case Place::UNIT:
                place->currentMarkingValue = new Data(Data::MULTIUNIT);
                place->currentMarkingValue->value.multiUnit = 0;
            case Place::BOOL:
                place->currentMarkingValue = new Data(Data::MULTIBOOL);
                place->currentMarkingValue->value.multiBool.f = 0;
                place->currentMarkingValue->value.multiBool.t = 0;
            case Place::INT:
                place->currentMarkingValue = new Data(Data::MULTIINT);
            }
    }
    findBindings();
}

void CPNetSimulator::findBindings()
{
    //Black magic bars our way. But the will of a templar is stronger!
    foreach(Transition *transition, net->transitions)
    {
        QList<Binding> possibleBindings;
        possibleBindings.append(Binding());
        QList<QList<Binding> > arcBindings;
        //collect arc bindings for all preset arcs
        foreach(Arc *arc, net->arcs)
        {
            if(!arc->isPreset || arc->transition != transition)
                continue;
            arcBindings.append(arc->findBindings());
        }
        transition->possibleBindings.clear();
        QList<Binding> mergedBindings = mergeBindings(possibleBindings, arcBindings);
        //check guards if needed
        foreach(Binding b, mergedBindings)
        {
            net->globalSymbolTable->bindVariables(b);
            if(!transition->parsedGuard)
                transition->possibleBindings << b;
            else
            {
                bool value = eval(transition->parsedGuard, net->globalSymbolTable, net->globalSymbolTable).value.b;
                if(value)
                    transition->possibleBindings << b;
            }
        }

        scene->getTransitionItem(transition)->populateCombo();
    }
}


QList<Binding> CPNetSimulator::mergeBindings(QList<Binding> possibleBindings, QList<QList<Binding> > arcBindings)
{
    if(possibleBindings.isEmpty() || arcBindings.isEmpty())
        return possibleBindings;
    //take one arc binding
    QList<Binding> arcBindingVariants = arcBindings.first();
    arcBindings.removeFirst();
    //prepare result list
    QList<Binding> newPossibleBindings;
    //try to merge all arc binding variants with all possible bindings, add them to result list if succeeded
    foreach(Binding possibleBinding, possibleBindings)
    {
        //take all possible arc variants
        foreach(Binding arcBindingVariant, arcBindingVariants)
        {
            Binding mergedBinding = possibleBinding;
            bool collision = false;
            //and with every binding element
            foreach(BindingElement arcElem, arcBindingVariant)
            {
                //try to look its clone in the possible binding, remember if found or not
                bool found = false;
                foreach(BindingElement existingElem, mergedBinding)
                {
                    if(existingElem.id() != arcElem.id())
                        continue;
                    found = true;
                    if(!(existingElem.data() == arcElem.data()))
                        collision = true;
                    break;
                }
                //just a shortcut
                if(collision)
                    break;
                //ad the item to the merged binding as it is not present there
                if(!found)
                    mergedBinding.append(arcElem);
            }
            if(!collision)
                newPossibleBindings.append(mergedBinding);
        }

    }
    return mergeBindings(newPossibleBindings, arcBindings);
}
