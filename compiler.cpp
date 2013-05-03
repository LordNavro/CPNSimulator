#include "symboltable.h"
#include "compiler.h"
#include "parser.parser.hpp"
#include <QStringList>
#include "QtGui"

int startSymbol;

Expression *currentParsedExpression = NULL;
DeclarationList *currentParsedDeclarationList = NULL;
SymbolTable *currentGlobalSymbolTable = NULL;
SymbolTable *currentLocalSymbolTable = NULL;
SymbolTable *currentSymbolTable = NULL;
Data::Type currentReturnType;
bool currentAssignmentAllowed = true;

Data::Data(Data::Type type): type(type)
{
    if(type == Data::MULTIINT)
        value.multiInt = new MultiInt;
    else if(type == Data::MULTIBOOL)
        value.multiBool.f = value.multiBool.t = 0;
}

Data::Data(const Data &data)
    : type(data.type)
{
    if(type == Data::MULTIINT)
        value.multiInt = new MultiInt(*data.value.multiInt);
    else if(type == Data::MULTIBOOL)
    {
        value.multiBool.t = data.value.multiBool.t;
        value.multiBool.f = data.value.multiBool.f;
    }
    else
        value = data.value;
}

Data &Data::operator=(const Data &data)
{
    if(this != &data)
    {
        if(type == Data::MULTIINT)
            delete value.multiInt;
        type = data.type;
        if(type == Data::MULTIINT)
            value.multiInt = new MultiInt(*data.value.multiInt);
        else if(type == Data::MULTIBOOL)
        {
            value.multiBool.t = data.value.multiBool.t;
            value.multiBool.f = data.value.multiBool.f;
        }
        else
            value = data.value;
    }
    return *this;
}

bool Data::operator ==(const Data &data) const
{
    if(type != data.type)
        return false;
    if(type == Data::UNIT || type == Data::BOOL)
        return value.b == data.value.b;
    else if(type == Data::INT)
        return value.i == data.value.i;
    else if(type == Data::MULTIUNIT)
        return value.multiUnit == data.value.multiUnit;
    else if(type == Data::MULTIBOOL)
        return value.multiBool.t == data.value.multiBool.t && value.multiBool.f == data.value.multiBool.f;
    else if(type == Data::MULTIINT)
        return *value.multiInt == *data.value.multiInt;
    Q_ASSERT(false);
    return false;
}

Data Data::operator +(const Data &data)
{
    if(data.type == Data::INT)
    {
        Data ret(Data::INT);
        ret.value.i = this->value.i + data.value.i;
        return ret;
    }
    else if(data.type == Data::MULTIUNIT)
    {
        Data ret(Data::MULTIUNIT);
        ret.value.multiUnit = this->value.multiUnit + data.value.multiUnit;
        return ret;
    }
    else if(data.type == Data::MULTIBOOL)
    {
        Data ret(Data::MULTIBOOL);
        ret.value.multiBool.t = this->value.multiBool.t + data.value.multiBool.t;
        ret.value.multiBool.f = this->value.multiBool.f + data.value.multiBool.f;
        return ret;
    }
    else if(data.type == Data::MULTIINT)
    {
        Data ret(Data::MULTIINT);
        QMapIterator<int, int>i(*this->value.multiInt);
        while(i.hasNext())
        {
            i.next();
            ret.value.multiInt->insert(i.key(), i.value());
        }
        QMapIterator<int, int>j(*data.value.multiInt);
        while(j.hasNext())
        {
            j.next();
            (*ret.value.multiInt)[j.key()] += j.value();
            if((*ret.value.multiInt)[j.key()] == 0)
                (*ret.value.multiInt).remove(j.key());
        }
        return ret;
    }
    Q_ASSERT(false);
    return Data();
}

Data Data::operator -(const Data &data)
{
    if(data.type == Data::INT)
    {
        Data ret(Data::INT);
        ret.value.i = this->value.i - data.value.i;
        return ret;
    }
    else if(data.type == Data::MULTIUNIT)
    {
        Data ret(Data::MULTIUNIT);
        ret.value.multiUnit = this->value.multiUnit - data.value.multiUnit;
        return ret;
    }
    else if(data.type == Data::MULTIBOOL)
    {
        Data ret(Data::MULTIBOOL);
        ret.value.multiBool.t = this->value.multiBool.t - data.value.multiBool.t;
        ret.value.multiBool.f = this->value.multiBool.f - data.value.multiBool.f;
        return ret;
    }
    else if(data.type == Data::MULTIINT)
    {
        Data ret(Data::MULTIINT);
        QMapIterator<int, int>i(*this->value.multiInt);
        while(i.hasNext())
        {
            i.next();
            ret.value.multiInt->insert(i.key(), i.value());
        }
        QMapIterator<int, int>j(*data.value.multiInt);
        while(j.hasNext())
        {
            j.next();
            (*ret.value.multiInt)[j.key()] -= j.value();
            if((*ret.value.multiInt)[j.key()] == 0)
                (*ret.value.multiInt).remove(j.key());
        }
        return ret;
    }
    Q_ASSERT(false);
    return Data();
}

QString Data::toString()
{
    switch(type)
    {
    case Data::UNIT:
        return value.b ? "unit" : "nounit";
    case Data::BOOL:
        return value.b ? "true" : "false";
    case Data::INT:
        return QString::number(value.i);
    case Data::MULTIUNIT:
        return QString::number(value.multiUnit) + " ^ unit";
    case Data::MULTIBOOL:
        return QString::number(value.multiBool.t) + " ^ true + " + QString::number(value.multiBool.f) + " ^ false";
    case Data::MULTIINT:
        break;
    }
    //handle multiint
    QStringList values;
    for(MultiInt::iterator i = value.multiInt->begin(); i != value.multiInt->end(); i++)
        values.append(QString::number(i.value()) + " ^ " + QString::number(i.key()));
    if(values.isEmpty())
        return "";
    return values.join(" + ");
}

Expression::~Expression()
{
    switch(type)
    {
    case Expression::MULTISET:
    case Expression::AND:
    case Expression::OR:
    case Expression::LEQ:
    case Expression::EQ:
    case Expression::NEQ:
    case Expression::GEQ:
    case Expression::GT:
    case Expression::LT:
    case Expression::PLUS:
    case Expression::MINUS:
    case Expression::MUL:
    case Expression::DIV:
    case Expression::MOD:
        delete right;
    case Expression::ASSIGN:
    case Expression::NOT:
    case Expression::UMINUS:
    case Expression::CONVERT:
        delete left;
        break;
    case Expression::FN:
        qDeleteAll(*expressionList);
        delete expressionList;
        break;
    case Expression::DATA:
        delete data;
        break;
    case Expression::VAR:
        break;
    case Expression::DPLUSPRE:
    case Expression::DPLUSPOST:
    case Expression::DMINUSPRE:
    case Expression::DMINUSPOST:
        break;
    }
}

Declaration::~Declaration()
{
    switch(type)
    {
    case Declaration::FN:
        if(command)
            delete command;
        break;
    case Declaration::VAR:
        break;
    }
}


Command::~Command()
{
    switch(type)
    {
    case Command::IFELSE:
        delete command2;
    case Command::IF:
    case Command::WHILE:
    case Command::DOWHILE:
        delete command;
    case Command::EXPR:
    case Command::RETURN:
        delete expression;
        break;
    case Command::BLOCK:
        qDeleteAll(commandList);
        break;
    case Command::DECL:
        break;
    case Command::FOR:
        delete expression;
        delete loopInit;
        delete loopIteration;
        delete command;
    }
}

