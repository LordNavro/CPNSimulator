#include "interpret.h"
#include <QStringList>
#include "computer.h"

Data eval(Expression *expression, SymbolTable *funTable, SymbolTable *varTable, Computer *computer)
{
    if(computer->cancelRequest)
        throw(QString("Stop request recieved, terminating evaluation."));
    if(expression->type == Expression::DATA)
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
        for(int i = 0; i < expression->expressionList->count(); i++)
        {
            SymbolTable::Symbol *symbol = new SymbolTable::Symbol(SymbolTable::VAR);
            symbol->data = new Data(eval(expression->expressionList->at(i), funTable, varTable, computer));
            tempVarTable->addSymbol(fn->parameterList.at(i).second, symbol);
        }
        Data ret = execute(fn->command, funTable, tempVarTable, computer);
        delete tempVarTable;
        return ret;
    }
    else if(expression->type == Expression::DPLUSPRE)
    {
        SymbolTable::Symbol *symbol = varTable->findSymbol(expression->id);
        symbol->data->value.i += 1;
        return *symbol->data;
    }
    else if(expression->type == Expression::DPLUSPOST)
    {
        SymbolTable::Symbol *symbol = varTable->findSymbol(expression->id);
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
        Data ret(*symbol->data);
        symbol->data->value.i -= 1;
        return ret;
    }
    Data left = eval(expression->left, funTable, varTable, computer);
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
        else
        {
            Data ret(Data::INT);
            ret.value.i = left.value.b ? 1 : 0;
            return ret;
        }
    }
    Data right = eval(expression->right, funTable, varTable, computer);
    if(expression->type == Expression::AND)
    {
        Data ret(Data::BOOL);
        ret.value.b = left.value.b && right.value.b;
        return ret;
    }
    else if(expression->type == Expression::OR)
    {
        Data ret(Data::BOOL);
        ret.value.b = left.value.b || right.value.b;
        return ret;
    }
    else if(expression->type == Expression::LEQ)
    {
        Data ret(Data::BOOL);
        ret.value.b = left.value.i <= right.value.i;
        return ret;
    }
    else if(expression->type == Expression::EQ)
    {
        Data ret(Data::BOOL);
        ret.value.b = left.value.i == right.value.i;
        return ret;
    }
    else if(expression->type == Expression::NEQ)
    {
        Data ret(Data::BOOL);
        ret.value.b = left.value.i != right.value.i;
        return ret;
    }
    else if(expression->type == Expression::GEQ)
    {
        Data ret(Data::BOOL);
        ret.value.b = left.value.i >= right.value.i;
        return ret;
    }
    else if(expression->type == Expression::GT)
    {
        Data ret(Data::BOOL);
        ret.value.b = left.value.i > right.value.i;
        return ret;
    }
    else if(expression->type == Expression::LT)
    {
        Data ret(Data::BOOL);
        ret.value.b = left.value.i < right.value.i;
        return ret;
    }
    else if(expression->type == Expression::MINUS)
    {
        Data ret(Data::INT);
        ret.value.i = left.value.i - right.value.i;
        return ret;
    }
    else if(expression->type == Expression::MUL)
    {
        Data ret(Data::INT);
        ret.value.i = left.value.i * right.value.i;
        return ret;
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
        if(left.type == Data::INT)
        {
            Data ret(Data::INT);
            ret.value.i = left.value.i + right.value.i;
            return ret;
        }
        else if(left.type == Data::MULTIUNIT)
        {
            Data ret(Data::MULTIUNIT);
            ret.value.multiUnit = left.value.multiUnit + right.value.multiUnit;
            return ret;
        }
        else if(left.type == Data::MULTIBOOL)
        {
            Data ret(Data::MULTIBOOL);
            ret.value.multiBool.t = left.value.multiBool.t + right.value.multiBool.t;
            ret.value.multiBool.f = left.value.multiBool.f + right.value.multiBool.f;
            return ret;
        }
        else if(left.type == Data::MULTIINT)
        {
            Data ret(Data::MULTIINT);
            QMapIterator<int, int>i(*left.value.multiInt);
            while(i.hasNext())
            {
                i.next();
                ret.value.multiInt->insert(i.key(), i.value());
            }
            QMapIterator<int, int>j(*right.value.multiInt);
            while(j.hasNext())
            {
                j.next();
                (*ret.value.multiInt)[j.key()] += j.value();
            }
            return ret;
        }
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
    InterCode *icStart = generate3AC(command);
    InterCode *icCurrent = icStart;
    while(icCurrent != NULL)
    {
        if(computer->cancelRequest)
        {
            delete icStart;
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
                varTable->addSymbol(id, symbol);
            }
            icCurrent = icCurrent->next;
        }
        else if(icCurrent->type == InterCode::EVAL)
        {
            eval(icCurrent->expression, funTable, varTable, computer);
            icCurrent = icCurrent->next;
        }
        else if(icCurrent->type == InterCode::BRANCH)
        {
            icCurrent = icCurrent->label;
        }
        else if(icCurrent->type == InterCode::BRANCHIF)
        {
            icCurrent = eval(icCurrent->expression, funTable, varTable, computer).value.b ? icCurrent->label : icCurrent->next;
        }
        else if(icCurrent->type == InterCode::BRANCHIFN)
        {
            icCurrent = eval(icCurrent->expression, funTable, varTable, computer).value.b ? icCurrent->next : icCurrent->label;
        }
        else if(icCurrent->type == InterCode::RETURN)
        {
            Data data = eval(icCurrent->expression, funTable, varTable, computer);
            delete icStart;
            return data;
        }
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


QString Binding::toString()
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
