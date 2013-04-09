#include "symboltable.h"

SymbolTable::SymbolTable()
{
    increaseScope();
}

SymbolTable::Symbol *SymbolTable::findSymbol(QString id)
{
    for(int i = stack.count() - 1; i >= 0; i--)
    {
        if(stack.at(i).contains(id))
            return stack.at(i)[id];
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
