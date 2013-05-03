#include "symboltable.h"
#include "interpret.h"

SymbolTable::SymbolTable()
{
    increaseScope();
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
        if(!s || s->type == SymbolTable::FN)
            qDebug() << "Invalid symbol" << elem.id() << " to be bound in table";
        else
        {
            delete s->data;
            s->data = new Data(elem.data());
        }
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
