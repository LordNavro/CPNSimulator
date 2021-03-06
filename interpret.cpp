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

#include "interpret.h"
#include <QStringList>
#include "computer.h"

const static int RECURSION_MAX = 800;

Data eval(Expression *expression, SymbolTable *funTable, SymbolTable *varTable, Computer *computer, bool resetCounter)
{
    if(resetCounter)
        computer->recursionCounter = 0;
    if(computer->cancelRequest)
        throw(QString("Stop request recieved, terminating evaluation."));

    if(expression->type == Expression::TERNAR)
    {
        Data condition = eval(expression->condition, funTable, varTable, computer, false);
        if(condition.value.b)
            return eval(expression->left, funTable, varTable, computer, false);
        else
            return eval(expression->right, funTable, varTable, computer, false);
    }
    else if(expression->type == Expression::DATA)
        return *expression->data;
    else if(expression->type == Expression::VAR)
    {
        SymbolTable::Symbol *s = varTable->findSymbol(expression->id);
        if(s == NULL)
            throw(QString("Runtime error: Usage of undefined variable " + expression->id));
        Data* d = s->data;
        if(d == NULL)
            throw(QString("Runtime error: Usage of uninitialized variable " + expression->id));
        return *d;
    }
    else if(expression->type == Expression::FN)
    {
        SymbolTable::Symbol *fn = funTable->findSymbol(expression->id);
        SymbolTable *tempVarTable = new SymbolTable();
        tempVarTable->increaseScope();
        for(int i = 0; i < expression->expressionList->count(); i++)
        {
            SymbolTable::Symbol *symbol = new SymbolTable::Symbol(SymbolTable::VAR);
            try{
                symbol->data = new Data(eval(expression->expressionList->at(i), funTable, varTable, computer, false));
            }catch(QString s)
            {
                delete symbol;
                delete tempVarTable;
                throw;
            }
            catch(const std::bad_alloc &a)
            {
                qDebug() << "badalloc cought";
                delete symbol;
                delete tempVarTable;
                throw;
            }

            if(!tempVarTable->addSymbol(fn->parameterList.at(i).second, symbol))
            {
                delete symbol;
                delete tempVarTable;
                throw(QString("Runtime error: Redefinition of symbol " + fn->parameterList.at(i).second));
            }
        }
        try{
            Data ret = execute(fn->command, funTable, tempVarTable, computer);
            delete tempVarTable;
            return ret;
        }catch(QString s)
        {
            delete tempVarTable;
            throw;
        }
        catch(const std::bad_alloc &a)
        {
            qDebug() << "badalloc cought";
            delete tempVarTable;
            throw;
        }
    }
    else if(expression->type == Expression::DPLUSPRE)
    {
        SymbolTable::Symbol *symbol = varTable->findSymbol(expression->id);
        if(symbol == NULL)
            throw(QString("Runtime error: Usage of undefined variable " + expression->id));
        symbol->data->value.i += 1;
        return *symbol->data;
    }
    else if(expression->type == Expression::DPLUSPOST)
    {
        SymbolTable::Symbol *symbol = varTable->findSymbol(expression->id);
        if(symbol == NULL)
            throw(QString("Runtime error: Usage of undefined variable " + expression->id));
        Data ret(*symbol->data);
        symbol->data->value.i += 1;
        return ret;
    }
    else if(expression->type == Expression::DMINUSPRE)
    {
        SymbolTable::Symbol *symbol = varTable->findSymbol(expression->id);
        symbol->data->value.i -= 1;
        return *symbol->data;
    }
    else if(expression->type == Expression::DMINUSPOST)
    {
        SymbolTable::Symbol *symbol = varTable->findSymbol(expression->id);
        if(symbol == NULL)
            throw(QString("Runtime error: Usage of undefined variable " + expression->id));
        Data ret(*symbol->data);
        symbol->data->value.i -= 1;
        return ret;
    }

    Data left = eval(expression->left, funTable, varTable, computer, false);

    if(expression->type == Expression::ASSIGN)
    {
        SymbolTable::Symbol *s = varTable->findSymbol(expression->id);
        if(!s)
            throw(QString("Runtime error: Assignment to undefined variable " + expression->id));
        if(s->data)
            delete s->data;
        s->data = new Data(left);
        return left;
    }
    else if(expression->type == Expression::NOT)
    {
        left.value.b = !left.value.b;
        return left;
    }
    else if(expression->type == Expression::UMINUS)
    {
        left.value.i = -left.value.i;
        return left;
    }
    else if(expression->type == Expression::CONVERT)
    {
        if(expression->dataType == Data::UNIT)
        {
            Data ret(Data::UNIT);
            if(left.type == Data::BOOL && left.value.b)
                ret.value.b = true;
            else if(left.type == Data::INT && left.value.i)
                ret.value.b = true;
            else
                ret.value.b = false;
            return ret;
        }
        else if(expression->dataType == Data::BOOL)
        {
            Data ret(Data::BOOL);
            if(left.type == Data::UNIT)
                ret.value.b = left.value.b;
            else if(left.type == Data::INT && left.value.i)
                ret.value.b = true;
            else ret.value.b = false;
            return ret;
        }
        else if(expression->dataType == Data::INT)
        {
            Data ret(Data::INT);
            ret.value.i = left.value.b ? 1 : 0;
            return ret;
        }
        else if(expression->dataType == Data::MULTIUNIT)
        {
            Data ret(Data::MULTIUNIT);
            ret.value.multiUnit = left.value.b ? 1 : 0;
            return ret;
        }
        else if(expression->dataType == Data::MULTIBOOL)
        {
            Data ret(Data::MULTIBOOL);
            if(left.value.b)
                ret.value.multiBool.t = 1;
            else
                ret.value.multiBool.f = 1;
            return ret;
        }
        else if(expression->dataType == Data::MULTIINT)
        {
            Data ret(Data::MULTIINT);
            ret.value.multiInt->insert(left.value.i, 1);
            return ret;
        }
    }
    else if(expression->type == Expression::AND)
    {
        if(left.value.b == false)
            return left;
        return eval(expression->right, funTable, varTable, computer, false);
    }
    else if(expression->type == Expression::OR)
    {
        if(left.value.b == true)
            return left;
        return eval(expression->right, funTable, varTable, computer, false);
    }

    Data right = eval(expression->right, funTable, varTable, computer, false);

    if(expression->type == Expression::LEQ)
    {
        Data ret(Data::BOOL);
        ret.value.b = left <= right;
        return ret;
    }
    else if(expression->type == Expression::EQ)
    {
        Data ret(Data::BOOL);
        ret.value.b = left == right;
        return ret;
    }
    else if(expression->type == Expression::NEQ)
    {
        Data ret(Data::BOOL);
        ret.value.b = left != right;
        return ret;
    }
    else if(expression->type == Expression::GEQ)
    {
        Data ret(Data::BOOL);
        ret.value.b = left >= right;
        return ret;
    }
    else if(expression->type == Expression::GT)
    {
        Data ret(Data::BOOL);
        ret.value.b = left > right;
        return ret;
    }
    else if(expression->type == Expression::LT)
    {
        Data ret(Data::BOOL);
        ret.value.b = left < right;
        return ret;
    }
    else if(expression->type == Expression::MINUS)
    {
        return left - right;
    }
    else if(expression->type == Expression::MUL)
    {
        return left * right;
    }
    else if(expression->type == Expression::DIV)
    {
        if(right.value.i == 0)
            throw(QString("Runtime error: division by zero"));
        Data ret(Data::INT);
        ret.value.i = left.value.i / right.value.i;
        return ret;
    }
    else if(expression->type == Expression::MOD)
    {
        if(right.value.i == 0)
            throw(QString("Runtime error: division by zero"));
        Data ret(Data::INT);
        ret.value.i = left.value.i % right.value.i;
        return ret;
    }
    else if(expression->type == Expression::PLUS)
    {
        return left + right;
    }
    else if(expression->type == Expression::MULTISET)
    {
        if(right.type == Data::UNIT)
        {
            Data ret(Data::MULTIUNIT);
            if(right.value.b)
                ret.value.multiUnit = left.value.i;
            else
                ret.value.multiUnit = 0;
            return ret;
        }
        if(right.type == Data::BOOL)
        {
            Data ret(Data::MULTIBOOL);
            if(right.value.b)
                ret.value.multiBool.t = left.value.i;
            else
                ret.value.multiBool.f = left.value.i;
            return ret;
        }
        else if(right.type == Data::INT)
        {
            Data ret(Data::MULTIINT);
            ret.value.multiInt->insert(right.value.i, left.value.i);
            return ret;
        }
    }

    Q_ASSERT(false);
    return Data(Data::UNIT); //compile-killer
}

InterCode *generate3AC(Command *command)
{
    InterCode *start = NULL;
    if(command->type == Command::IF)
    {
        start = new InterCode(InterCode::BRANCHIFN, command->expression);
        start->append(generate3AC(command->command));
        start->label = start->append(new InterCode(InterCode::LABEL));
    }
    else if(command->type == Command::IFELSE)
    {
        start = new InterCode(InterCode::BRANCHIFN, command->expression);
        start->append(generate3AC(command->command));
        InterCode *toEnd = start->append(new InterCode(InterCode::BRANCH));
        start->label = start->append(new InterCode(InterCode::LABEL));
        start->append(generate3AC(command->command2));
        toEnd->label = start->append(new InterCode(InterCode::LABEL));
    }
    else if(command->type == Command::EXPR)
    {
        start = new InterCode(InterCode::EVAL, command->expression);
    }
    else if(command->type == Command::WHILE)
    {
        start = new InterCode(InterCode::LABEL);
        InterCode *branch = start->append(new InterCode(InterCode::BRANCHIFN, command->expression));
        start->append(generate3AC(command->command));
        start->append(new InterCode(InterCode::BRANCH))->label = start;
        branch->label = start->append(new InterCode(InterCode::LABEL));
    }
    else if(command->type == Command::DOWHILE)
    {
        start = new InterCode(InterCode::LABEL);
        start->append(generate3AC(command->command));
        start->append(new InterCode(InterCode::BRANCHIF, command->expression))->label = start;
    }
    else if(command->type == Command::BLOCK)
    {
        start = new InterCode(InterCode::INCSCOPE);
        foreach(Command *c, command->commandList)
            start->append(generate3AC(c));
        start->append(new InterCode(InterCode::DECSCOPE));
    }
    else if(command->type == Command::RETURN)
    {
        start = new InterCode(InterCode::RETURN, command->expression);
    }
    else if(command->type == Command::DECL)
    {
        start = new InterCode(InterCode::DECL, command);
    }
    else if(command->type == Command::FOR)
    {
        start = new InterCode(InterCode::EVAL, command->loopInit);
        InterCode *branchToCond = start->append(new InterCode(InterCode::BRANCH));
        InterCode *labelStart = start->append(new InterCode(InterCode::LABEL));
        start->append(new InterCode(InterCode::EVAL, command->loopIteration));
        branchToCond->label = start->append(new InterCode(InterCode::LABEL));
        InterCode *branchEnd = start->append(new InterCode(InterCode::BRANCHIFN, command->expression));
        start->append(generate3AC(command->command));
        start->append(new InterCode(InterCode::BRANCH))->label = labelStart;
        branchEnd->label = start->append(new InterCode(InterCode::LABEL));
    }
    return start;
}

Data execute(Command *command, SymbolTable *funTable, SymbolTable *varTable, Computer *computer)
{
    computer->recursionCounter++;
    if(computer->recursionCounter > RECURSION_MAX)
    {
        throw(QString("Maximum recursion level reached. Terminating computation to prevent stack overflow."));
    }


    InterCode *icStart = generate3AC(command);
    InterCode *icCurrent = icStart;
    try{
        while(icCurrent != NULL)
        {
            if(computer->cancelRequest)
            {
                throw(QString("Stop request recieved, terminating execution."));
            }

            if(icCurrent->type == InterCode::LABEL)
            {
                icCurrent = icCurrent->next;
                continue;
            }
            else if(icCurrent->type == InterCode::INCSCOPE)
            {
                icCurrent = icCurrent->next;
                varTable->increaseScope();
            }
            else if(icCurrent->type == InterCode::DECSCOPE)
            {
                icCurrent = icCurrent->next;
                varTable->decreaseScope();
            }
            else if(icCurrent->type == InterCode::DECL)
            {
                foreach(QString id, icCurrent->command->idList)
                {
                    SymbolTable::Symbol *symbol = new SymbolTable::Symbol(SymbolTable::VAR);
                    symbol->data = new Data(icCurrent->command->dataType);
                    if(!varTable->addSymbol(id, symbol))
                    {
                        delete symbol;
                        throw(QString("Redeclaration of variable " + id));
                    }
                }
                icCurrent = icCurrent->next;
            }
            else if(icCurrent->type == InterCode::EVAL)
            {
                eval(icCurrent->expression, funTable, varTable, computer, false);
                icCurrent = icCurrent->next;
            }
            else if(icCurrent->type == InterCode::BRANCH)
            {
                icCurrent = icCurrent->label;
            }
            else if(icCurrent->type == InterCode::BRANCHIF)
            {
                icCurrent = eval(icCurrent->expression, funTable, varTable, computer, false).value.b ? icCurrent->label : icCurrent->next;
            }
            else if(icCurrent->type == InterCode::BRANCHIFN)
            {
                icCurrent = eval(icCurrent->expression, funTable, varTable, computer, false).value.b ? icCurrent->next : icCurrent->label;
            }
            else if(icCurrent->type == InterCode::RETURN)
            {
                Data data = eval(icCurrent->expression, funTable, varTable, computer, false);
                delete icStart;
                return data;
            }
        }
    }
    catch(QString s)
    {
        delete icStart;
        throw;
    }
    catch(const std::bad_alloc &a)
    {
        delete icStart;
        throw;
    }

    Q_ASSERT(false);
    return Data(Data::UNIT); //compile-killer
}


InterCode *InterCode::last()
{
    InterCode *ic = this;
    while(ic->next != NULL)
        ic = ic->next;
    return ic;
}

InterCode *InterCode::append(InterCode *ic)
{
    return last()->next = ic;
}


QString Binding::toString() const
{
    QStringList values;
    foreach(BindingElement binding, *this)
    {
        values << binding.id() + " = " + binding.data().toString();
    }
    if(values.isEmpty())
        return "<constants>";
    return values.join(", ");
}
