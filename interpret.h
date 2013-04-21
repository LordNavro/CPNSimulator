#ifndef INTERPRET_H
#define INTERPRET_H

#include "compiler.h"
#include "symboltable.h"
#include <QtCore>

    class BindingElement : QPair<QString, Data>
    {
        QString &id(){return first;}
        Data &data(){return second;}
    };
    typedef QList<BindingElement> Binding;

    class InterCode;

    class InterCode{
    public:
        enum Type {LABEL, INCSCOPE, DECSCOPE, DECL, EVAL, BRANCH, BRANCHIF, BRANCHIFN, RETURN};
        Type type;

        InterCode(Type type, Command *command = NULL) : type(type), command(command), label(NULL){}
        ~InterCode(){if(next != NULL) delete next;}
        Command *command;
        InterCode *label;
        InterCode *next;
        InterCode *last();
        InterCode *append(InterCode *ic);
    };




    Data eval(Expression *expression, SymbolTable *funTable, SymbolTable *varTable);

    InterCode *generate3AC(Command *command);

    Data execute(Command *command, SymbolTable *funTable, SymbolTable *varTable);

#endif // INTERPRET_H
