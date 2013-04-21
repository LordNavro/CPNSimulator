#include "symboltable.h"
#include "compiler.h"
#include "parser.parser.hpp"

int startSymbol;

Expression *currentParsedExpression = NULL;
DeclarationList *currentParsedDeclarationList = NULL;
SymbolTable *currentGlobalSymbolTable = NULL;
SymbolTable *currentLocalSymbolTable = NULL;
SymbolTable *currentSymbolTable = NULL;
Data::Type currentReturnType;

Data::Data(const Data &data)
    : type(data.type)
{
    if(type == Data::MULTIINT)
        value.multiInt = new MultiInt(*data.value.multiInt);
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
        else
            value = data.value;
    }
    return *this;
}

Expression::~Expression()
{
    switch(this->type)
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
    }
}

Declaration::~Declaration()
{
    switch(this->type)
    {
    case Declaration::FN:
        delete command;
        break;
    case Declaration::VAR:
        break;
    }
}


Command::~Command()
{
    switch(this->type)
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
    }
}

