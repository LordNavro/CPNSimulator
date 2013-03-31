#include "compiler.h"

Expression::~Expression()
{
    switch(this->type)
    {
    case Expression::ASSIGN:
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
    case Expression::MULT:
    case Expression::DIV:
    case Expression::MOD:
    case Expression::NOT:
    case Expression::UMINUS:
    case Expression::FN:
    case Expression::VAR:
    case Expression::DATA:
        break;
    }
}

Declaration::~Declaration()
{
    switch(this->type)
    {
    case Declaration::VAR:
    case Declaration::FN:
        break;
    }
}


Command::~Command()
{
    switch(this->type)
    {
    case Command::IF:
    case Command::IFELSE:
    case Command::EXPR:
    case Command::WHILE:
    case Command::DOWHILE:
    case Command::BLOCK:
    case Command::RETURN:
    case Command::DECL:
        break;
    }
}

