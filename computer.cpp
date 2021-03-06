/*
 *    Copyright 2013, 2014 Ondrej Navratil.
 *    This project and all of its contents are distributed under the terms of the GNU General Public License.
 *
 *    This file is part of CPNSimulator.
 *
 *    CPNSimulator is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published b
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    CPNSimulator is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with CPNSimulator. If not, see <http://www.gnu.org/licenses/>.
*/

#include "computer.h"

Computer::Computer(CPNet *net, QObject *parent) :
    QThread(parent), net(net), cancelRequest(false)
{
    setStackSize(1024*1024);
}

Computer::~Computer()
{
}

void Computer::run()
{
    try{
        if(cancelRequest)
            throw(QString("Cancel request received, terminating computation."));
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
        else if(mode == Computer::ToInitialMarking)
        {
            toInitialMarking();
        }
        else if(mode == Computer::ToCurrentMarking)
        {
            toCurrentMarking();
        }
        emit signalCompleted();
    }
    catch(QString message)
    {
        emit signalFailed(message);
    }
    catch(const std::bad_alloc &a)
    {
        emit signalFailed(tr("std::bad_alloc cought: ran out of memory during computation."));
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

void Computer::setPlaceMarking(Place *place, Expression *expression)
{
    if(place->currentMarkingValue)
        delete place->currentMarkingValue;
    place->currentMarkingValue = NULL;
    if(expression)
    {
       place->currentMarkingValue = new Data(eval(expression, net->globalSymbolTable, NULL, this));
    }
    else
    {
        switch(place->colourSet)
        {
        case Place::UNIT:
            place->currentMarkingValue = new Data(Data::MULTIUNIT);
            place->currentMarkingValue->value.multiUnit = 0;
            break;
        case Place::BOOL:
            place->currentMarkingValue = new Data(Data::MULTIBOOL);
            place->currentMarkingValue->value.multiBool.f = 0;
            place->currentMarkingValue->value.multiBool.t = 0;
            break;
        case Place::INT:
            place->currentMarkingValue = new Data(Data::MULTIINT);
            break;
        }
    }
}

void Computer::toInitialMarking()
{
    foreach(Place *place, net->places)
    {
        setPlaceMarking(place, place->parsedInitialMarking);
    }
}

void Computer::toCurrentMarking()
{
    foreach(Place *place, net->places)
    {
        setPlaceMarking(place, place->parsedCurrentMarking);
    }
}
