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

#include "symboltable.h"
#include "interpret.h"

SymbolTable::SymbolTable()
{
    //increaseScope();
}

SymbolTable::~SymbolTable()
{
    while(!stack.isEmpty())
        decreaseScope();
}

SymbolTable::Symbol *SymbolTable::findSymbol(QString id)
{
    for(int i = stack.count() - 1; i >= 0; i--)
    {
        if(stack.at(i).contains(id))
        {
            return stack.at(i)[id];
        }
    }
    return NULL;
}

bool SymbolTable::addSymbol(QString id, Symbol *symbol)
{
    if(stack.last().contains(id))
        return false;
    stack.last().insert(id, symbol);
    return true;
}

void SymbolTable::bindVariables(Binding binding)
{
    foreach(BindingElement elem, binding)
    {
        SymbolTable::Symbol *s = findSymbol(elem.id());
        delete s->data;
        s->data = new Data(elem.data());
    }
}

void SymbolTable::increaseScope()
{
    stack.append(SymbolTable::Table());
}

void SymbolTable::decreaseScope()
{
    qDeleteAll(stack.last());
    stack.removeLast();
}

SymbolTable::Symbol::~Symbol()
{
    if(type == SymbolTable::VAR && data != NULL)
        delete data;
    if(type == SymbolTable::FN && command != NULL)
        delete command;
}
