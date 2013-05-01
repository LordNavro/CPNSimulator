#ifndef INTERPRET_H
#define INTERPRET_H

#include "compiler.h"
#include "symboltable.h"
#include <QtCore>

    class BindingElement : QPair<QString, Data>
    {
    public:
        BindingElement(QString id, Data::Type dataType) : QPair(id, dataType){}
        QString &id(){return first;}
        Data &data(){return second;}
    };
    class Binding : public QList<BindingElement>
    {
    public:
        QString toString();
    };

    class InterCode;
    class Computer;

    class InterCode{
    public:
        enum Type {LABEL, INCSCOPE, DECSCOPE, DECL, EVAL, BRANCH, BRANCHIF, BRANCHIFN, RETURN};
        Type type;

        InterCode(Type type, Command *command = NULL) : type(type), command(command), label(NULL), next(NULL){}
        ~InterCode(){if(next != NULL) delete next;}
        Command *command;
        InterCode *label;
        InterCode *next;
        InterCode *last();
        InterCode *append(InterCode *ic);
    };




    Data eval(Expression *expression, SymbolTable *funTable, SymbolTable *varTable, Computer *computer);

    InterCode *generate3AC(Command *command);

    Data execute(Command *command, SymbolTable *funTable, SymbolTable *varTable, Computer *computer);

#endif // INTERPRET_H
