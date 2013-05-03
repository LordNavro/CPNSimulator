#include "computer.h"

Computer::Computer(CPNet *net, QObject *parent) :
    QThread(parent), net(net), cancelRequest(false)
{
}

Computer::~Computer()
{
}

void Computer::run()
{
    cancelRequest = false;
    try{
        if(mode == Computer::FindBinding)
        {
            findBinding(net->currentMarking());
        }
        else if(mode == Computer::FireTransition)
        {
            NetMarking newMarking = fireTransition(net->currentMarking(), sti->transition, sti->transition->possibleBindings.at(sti->comboBinding->currentIndex()));
            net->setCurrentMarking(newMarking);
        }
        else if(mode == Computer::GenerateStateSpace)
        {
            graph.generate(net, this, depth);
        }
        emit signalCompleted();
    }
    catch(QString message)
    {
        emit signalFailed(message);
    }
}

void Computer::findBinding(NetMarking marking)
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
            arcBindings.append(arc->findBindings(this, marking));
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
                bool value = eval(transition->parsedGuard, net->globalSymbolTable, net->globalSymbolTable, this).value.b;
                if(value)
                    transition->possibleBindings << b;
            }
        }
    }
}

QList<Binding> Computer::mergeBindings(QList<Binding> possibleBindings, QList<QList<Binding> > arcBindings)
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

NetMarking Computer::fireTransition(NetMarking marking, Transition *transition, Binding binding)
{
    net->globalSymbolTable->bindVariables(binding);
    QList<QPair<Place *, Data> > add, subtract;
    foreach(Arc *arc, net->arcs)
    {
        if(arc->transition != transition)
            continue;
        if(arc->isPreset)
            subtract.append(QPair<Place *, Data>(arc->place, eval(arc->parsedExpression, net->globalSymbolTable, net->globalSymbolTable, this)));
        else
            add.append(QPair<Place *, Data>(arc->place, eval(arc->parsedExpression, net->globalSymbolTable, net->globalSymbolTable, this)));
    }
    QPair<Place*, Data> pair(NULL, Data(Data::UNIT));
    foreach(pair, subtract)
        marking[pair.first] = marking[pair.first] - pair.second;
    foreach(pair, add)
        marking[pair.first] = marking[pair.first] + pair.second;
    return marking;
}
