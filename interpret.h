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

#ifndef INTERPRET_H
#define INTERPRET_H

#include "compiler.h"
#include "symboltable.h"
#include <QtCore>

    class BindingElement : public QPair<QString, Data>
    {
    public:
        BindingElement(QString id, Data::Type dataType) : QPair(id, dataType){}
        QString &id(){return first;}
        Data &data(){return second;}
    };
    class Binding : public QList<BindingElement>
    {
    public:
        QString toString() const;
    };

    class InterCode;
    class Computer;

    class InterCode{
    public:
        enum Type {LABEL, INCSCOPE, DECSCOPE, DECL, EVAL, BRANCH, BRANCHIF, BRANCHIFN, RETURN};
        Type type;

        InterCode(Type type) : type(type), command(NULL), expression(NULL), label(NULL), next(NULL){}
        InterCode(Type type, Command *command) : type(type), command(command), expression(NULL), label(NULL), next(NULL){}
        InterCode(Type type, Expression *expression) : type(type), command(NULL), expression(expression), label(NULL), next(NULL){}
        ~InterCode(){if(next != NULL) delete next;}
        Command *command;
        Expression *expression;
        InterCode *label;
        InterCode *next;
        InterCode *last();
        InterCode *append(InterCode *ic);
    };

    Data eval(Expression *expression, SymbolTable *funTable, SymbolTable *varTable, Computer *computer, bool resetCounter = true);

    InterCode *generate3AC(Command *command);

    Data execute(Command *command, SymbolTable *funTable, SymbolTable *varTable, Computer *computer);

#endif // INTERPRET_H
