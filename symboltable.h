#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "compiler.h"
#include "QMap"

class SymbolTable
{
public:
    SymbolTable();

    enum SymbolType {VAR, FN};
    struct Symbol
    {
        Symbol(SymbolTable::SymbolType type) : type(type), command(NULL), data(NULL){}
        ~Symbol();
        SymbolTable::SymbolType type;
        Data::Type dataType;
        ParameterList parameterList;
        Command *command;
        Data *data;
    };
    typedef QMap<QString, SymbolTable::Symbol *> Table;
    typedef QList<SymbolTable::Table> TableStack;

    SymbolTable::TableStack stack;

    SymbolTable::Symbol *findSymbol(QString id);
    bool addSymbol(QString id, SymbolTable::Symbol *symbol);

    void increaseScope();
    void decreaseScope();
};

#endif // SYMBOLTABLE_H
