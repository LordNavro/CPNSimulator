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

%{

    #include "compiler.h"
    #include "cpnet.h"
    #include "symboltable.h"
    Command *branchCommand(Command::Type type, Expression *e, Command *c);
    Expression *binaryOp(Data::Type dataType, Expression::Type type, Expression *e1, Expression *e2);
    Expression *unaryOp(Data::Type dataType, Expression::Type type, Expression *e1);
    Expression *incDec(Expression::Type type, Id id);
    Expression *convert(Data::Type type, Expression *e);
    Expression *plus(Expression *e1, Expression *e2);
    Expression *minus(Expression *e1, Expression *e2);
    Expression *mul(Expression *e1, Expression *e2);
    Expression *multiset(Expression *e1, Expression *e2);
    Expression *compare(Expression::Type type, Expression *e1, Expression *e2);
    bool isSimpleType(Data::Type type);

    char typeNames[][20] = {"unit", "bool", "int", "multiunit", "multibool", "multiint"};

    #include <QtGui>
    extern int yylineno;
    extern char *yytext;

    int yylex();
    void yyerror(const char *message)
    {
        currentParsedNet->addError(CPNet::SYNTACTIC, message + 13); // discard default "syntax error," prefix
    }

%}

%union{
    Data *data;
    Data::Type dataType;
    IdList *idList;
    Id *id;
    Command *command;
    CommandList * commandList;
    ParameterList *parameterList;
    Expression *expression;
    ExpressionList *expressionList;
    Declaration *declaration;
    DeclarationList *declarationList;
}

%token START_EXPRESSION START_DECLARATION START_MARKING START_PRESET START_GUARD
%token IF ELSE WHILE DO RETURN FOR
%token UNIT BOOL INT MULTIUNIT MULTIBOOL MULTIINT
%token ID
%token DATATYPE
%token DATA
%token LEQ GEQ EQ NEQ AND OR DMINUS DPLUS


%right '=' ':' '?'
%left OR
%left AND
%left EQ NEQ
%left LEQ GEQ '>' '<'
%left '+' '-'
%left '*' '/' '%'
%left '`'
%nonassoc '!' UMINUS DMINUS DPLUS
%nonassoc THEN
%nonassoc ELSE

%type <data> DATA
%type <expression> preset
%type <id> ID
%type <dataType> DATATYPE
%type <parameterList> parameterList parameterListNE
%type <idList> idList
%type <expression> expression expressionId expressionData
%type <expressionList> expressionList expressionListNE
%type <declaration> declaration
%type <declarationList> declarationList
%type <commandList> commandList
%type <command> command commandND

%destructor {delete $$;} <expression> <id> <data> <declaration> <command> <idList> <parameterList>
%destructor {qDeleteAll(*$$); delete $$;} <expressionList> <declarationList> <commandList>

%error-verbose

%start start

%%

start: START_DECLARATION {
            currentAssignmentAllowed = true;
            currentIdAllowed = true;
            currentSymbolTable = currentLocalSymbolTable;
        }
        declarationList { currentParsedDeclarationList = $3; }
    | START_EXPRESSION {
            currentAssignmentAllowed = false;
            currentIdAllowed = true;
            currentSymbolTable = currentGlobalSymbolTable;
        }
        expression { currentParsedExpression = $3; }
    | START_GUARD {
            currentAssignmentAllowed = false;
            currentIdAllowed = true;
            currentSymbolTable = currentGlobalSymbolTable;
        }
        expression { currentParsedExpression = $3; }
    | START_GUARD
        /* empty */ { currentParsedExpression = NULL; }
    | START_MARKING {
            currentAssignmentAllowed = false;
            currentIdAllowed = false;
            currentSymbolTable = currentGlobalSymbolTable;
        }
        expression { currentParsedExpression = $3; }
    | START_MARKING
        /* empty */  { currentParsedExpression = NULL; }
    | START_PRESET {
            //has special rules w/o assign
            currentIdAllowed = true;
            currentSymbolTable = currentGlobalSymbolTable;
        }
        preset { currentParsedExpression = $3; }
    | error { currentParsedDeclarationList = NULL; currentParsedExpression = NULL; }
    ;


declarationList: /*empty*/ { $$ = new DeclarationList(); }
    | declaration declarationList { $2->append($1); $$ = $2; }
    ;

declaration: DATATYPE idList ';' {
        if($1 == Data::MULTIUNIT || $1 == Data::MULTIBOOL || $1 == Data::MULTIINT)
        {
            currentParsedNet->addError(CPNet::SEMANTIC, "Global variables cant have multiset type");
            YYERROR;
        }
        $$ = new Declaration(Declaration::VAR);
        $$->dataType = $1;
        $$->idList = *$2;
        foreach(Id id, $$->idList)
        {
            SymbolTable::Symbol *symbol = new SymbolTable::Symbol(SymbolTable::VAR);
            symbol->dataType = $1;
            symbol->data = new Data($1);
            if(!currentGlobalSymbolTable->addSymbol(id, symbol))
            {
                currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + id + " already declared/defined in this scope");
                delete symbol;
            }
        }
        delete $2;
    }
    | DATATYPE ID '(' parameterList ')' {
        currentLocalSymbolTable = new SymbolTable();
        currentLocalSymbolTable->increaseScope();
        currentSymbolTable = currentLocalSymbolTable;
        foreach(Parameter parameter, *$4)
        {
            SymbolTable::Symbol *symbol = new SymbolTable::Symbol(SymbolTable::VAR);
            symbol->dataType = parameter.first;
            if(!currentLocalSymbolTable->addSymbol(parameter.second, symbol))
            {
                currentParsedNet->addError(CPNet::SEMANTIC, "Duplicate parameter " + parameter.second);
                delete symbol;
            }
        }
        currentReturnType = $1;
    } commandND {
        $$ = new Declaration(Declaration::FN);
        $$->dataType = $1;
        $$->id = *$2;
        $$->parameterList = *$4;
        $$->command = $7;
        SymbolTable::Symbol *existing = currentGlobalSymbolTable->findSymbol(*$2);
        if(existing)
        {
            if(existing->type != SymbolTable::FN)
            {
                currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$2 + " already declared as a variable");
                //delete $7; will be deleted along with the parsedDeclarationList
            }
            else if(existing->command)
            {
                currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$2 + " already defined");
                //delete $7; will be deleted along with the parsedDeclarationList
            }
            else
            {
                if(existing->dataType != $1 || existing->parameterList != *$4)
                    currentParsedNet->addError(CPNet::SEMANTIC, "Definition of " + *$2 + " does not match declaration");
                existing->command = $7;
            }
        }
        else
        {
            SymbolTable::Symbol *symbol = new SymbolTable::Symbol(SymbolTable::FN);
            symbol->dataType = $1;
            symbol->parameterList = *$4;
            symbol->command = $7;
            currentGlobalSymbolTable->addSymbol(*$2, symbol);
        }
        if($7->type != Command::RETURN && ($7->type != Command::BLOCK || $7->commandList.isEmpty() || $7->commandList.last()->type != Command::RETURN))
            currentParsedNet->addError(CPNet::SEMANTIC, "Missing return from function");

        delete $4;
        delete $2;
        delete(currentLocalSymbolTable);
    }
    | DATATYPE ID '(' parameterList ')' ';' {
        $$ = new Declaration(Declaration::FN);
        $$->dataType = $1;
        $$->id = *$2;
        $$->parameterList = *$4;
        $$->command = NULL;
        SymbolTable::Symbol *symbol = new SymbolTable::Symbol(SymbolTable::FN);
        symbol->dataType = $1;
        symbol->parameterList = *$4;
        symbol->command = NULL;
        if(!currentGlobalSymbolTable->addSymbol(*$2, symbol))
        {
            currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$2 + " already declared/defined in this scope");
            delete symbol;
        }
        delete $4;
        delete $2;
    }
    ;

idList: ID {
        $$ = new IdList();
        $$->append(*$1);
        delete $1;
    }
    | ID ',' idList {
        $$ = $3;
        $$->append(*$1);
        delete $1;
    }
    ;

parameterList: /*empty*/ { $$ = new ParameterList(); }
    | parameterListNE { $$ = $1; }
    ;

parameterListNE: DATATYPE ID {
        $$ = new ParameterList();
        $$->append(Parameter($1, *$2));
        delete $2;
    }
    | DATATYPE ID ',' parameterListNE {
        $$ = $4;
        $$->append(Parameter($1, *$2));
        delete $2;
    }
    ;

commandList: /*empty*/ { $$ = new CommandList(); }
    | commandList command { $$ = $1; $$->append($2); }
    ;

command: commandND
    | DATATYPE idList ';' {
        $$ = new Command(Command::DECL);
        $$->dataType = $1;
        $$->idList = *$2;
        foreach(Id id, $$->idList)
        {
            SymbolTable::Symbol *symbol = new SymbolTable::Symbol(SymbolTable::VAR);
            symbol->dataType = $1;
            symbol->data = new Data($1);
            if(!currentLocalSymbolTable->addSymbol(id, symbol))
            {
                currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + id + " already declared/defined in this scope");
                delete symbol;
            }
        }
        delete $2;
    }
    ;

commandND: WHILE '(' expression ')' commandND { $$ = branchCommand(Command::WHILE, $3, $5); }
    | IF '(' expression ')' commandND { $$ = branchCommand(Command::IF, $3, $5); } %prec THEN
    | IF '(' expression ')' commandND ELSE commandND { $$ = branchCommand(Command::IFELSE, $3, $5); $$->command2 = $7; }
    | DO commandND WHILE '(' expression ')' { $$ = branchCommand(Command::DOWHILE, $5, $2); }
    | expression ';' { $$ = new Command(Command::EXPR); $$->expression = $1; }
    | RETURN expression ';' { $$ = new Command(Command::RETURN); $$->expression = convert(currentReturnType, $2); }
    | '{' { currentLocalSymbolTable->increaseScope(); } commandList { currentLocalSymbolTable->decreaseScope(); } '}' { $$ = new Command(Command::BLOCK); $$->commandList = *$3; delete $3; }
    | FOR '(' expression ';' expression ';' expression ')' command { $$ = branchCommand(Command::FOR, $5, $9); $$->loopInit = $3; $$->loopIteration = $7; }
    ;

expression:
    expression AND expression {$$ = binaryOp(Data::BOOL, Expression::AND, convert(Data::BOOL, $1), convert(Data::BOOL, $3));}
    | expression OR expression {$$ = binaryOp(Data::BOOL, Expression::OR, convert(Data::BOOL, $1), convert(Data::BOOL, $3));}
    | expression LEQ expression {$$ = compare(Expression::LEQ, $1, $3);}
    | expression GEQ expression {$$ = compare(Expression::GEQ, $1, $3);}
    | expression EQ  expression {$$ = compare(Expression::EQ, $1, $3);}
    | expression NEQ expression {$$ = compare(Expression::NEQ, $1, $3);}
    | expression '>' expression {$$ = compare(Expression::GT, $1, $3);}
    | expression '<' expression {$$ = compare(Expression::LT, $1, $3);}
    | expression '+' expression {$$ = plus($1, $3);}
    | expression '-' expression {$$ = minus($1, $3);}
    | expression '*' expression {$$ = mul($1, $3);}
    | expression '/' expression {$$ = binaryOp(Data::INT, Expression::DIV, convert(Data::INT, $1), convert(Data::INT, $3));}
    | expression '%' expression {$$ = binaryOp(Data::INT, Expression::MOD, convert(Data::INT, $1), convert(Data::INT, $3));}
    | expression '`' expression {$$ = multiset($1, $3); }
    | '(' expression ')'            {$$ = $2;}
    | '!' expression                {$$ = unaryOp(Data::BOOL, Expression::NOT, convert(Data::BOOL, $2));}
    | '-' expression %prec UMINUS   {$$ = unaryOp(Data::INT, Expression::UMINUS, convert(Data::INT, $2));}
    | '(' DATATYPE ')' expression %prec UMINUS   {$$ = convert($2, $4);}
    | expression '?' expression ':' expression {
        $$ = binaryOp($3->dataType, Expression::TERNAR, $3, $5);
        $$->condition = convert(Data::BOOL, $1);
        if($3->dataType != $5->dataType)
            currentParsedNet->addError(CPNet::SEMANTIC, "Different data types in ? operation");
    }
    | expressionId
    | expressionData
    | DPLUS ID {$$ = incDec(Expression::DPLUSPRE, *$2); delete $2;}
    | ID DPLUS {$$ = incDec(Expression::DPLUSPOST, *$1); delete $1;}
    | DMINUS ID {$$ = incDec(Expression::DMINUSPRE, *$2); delete $2;}
    | ID DMINUS {$$ = incDec(Expression::DMINUSPOST, *$1); delete $1;}
    | ID '(' expressionList ')' {
        $$ = new Expression(Expression::FN);
        $$->expressionList = new ExpressionList();
        SymbolTable::Symbol *symbol = currentGlobalSymbolTable->findSymbol(*$1);
        if(!symbol)
        {
            currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$1 + " was not declared in this scope");
            $$->dataType = Data::INT;
            qDeleteAll(*$3);
        }
        else if(symbol->type == SymbolTable::VAR)
        {
            currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$1 + " is a variable, cannot use as a function");
            $$->dataType = symbol->dataType;
            qDeleteAll(*$3);
        }
        else
        {
            $$->dataType = symbol->dataType;
            $$->id = *$1;
            if($3->size() != symbol->parameterList.size())
                currentParsedNet->addError(CPNet::SEMANTIC, "Parameter count for " + *$1 + " does not match");
            int i;
            for(i = 0; i < qMin($3->size(), symbol->parameterList.size()); i++)
                $$->expressionList->append(convert(symbol->parameterList.at(i).first, $3->at(i)));
            for(;i < $3->size();i++) //delete unused exprs.
                delete $3->at(i);
        }
        $$->id = *$1;
        delete $1;
        delete $3; // DO NOT qDeleteAll -> exprs. used after being converted
    }
    | ID '=' expression {
        SymbolTable::Symbol *symbol = currentSymbolTable->findSymbol(*$1);
        if(!symbol)
        {
            currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$1 + " was not declared in this scope");
            $$ = unaryOp(Data::INT, Expression::ASSIGN, $3);
        }
        else
        {
            if(symbol->type == SymbolTable::FN)
                currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$1 + " is a function, cannot use as variable");
            $$ = unaryOp(symbol->dataType, Expression::ASSIGN, convert(symbol->dataType, $3));
        }
        if(!currentAssignmentAllowed)
            currentParsedNet->addError(CPNet::SEMANTIC, "Assignment operator is not allowed in this type of inscription");
        $$->id = *$1;
        delete $1;
    }
    ;

expressionData:DATA  {
        $$ = new Expression(Expression::DATA);
        $$->data = $1;
        $$->dataType = $1->type;
    }
    ;

expressionId: ID {
        SymbolTable::Symbol *symbol = currentSymbolTable->findSymbol(*$1);
        $$ = new Expression(Expression::VAR);
        if(!symbol)
        {
            currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$1 + " was not declared in this scope");
            $$->dataType = Data::INT;
        }
        else
        {
            if(symbol->type == SymbolTable::FN)
                currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + *$1 + " is a function, cannot use as variable");
            $$->dataType = symbol->dataType;
        }
        if(!currentIdAllowed)
            currentParsedNet->addError(CPNet::SEMANTIC, "Variables are not allowed in this type of inscription");
        $$->id = *$1;
        delete $1;
    }
    ;

expressionList: /* empty */ { $$ = new ExpressionList(); }
    | expressionListNE { $$ = $1; }
    ;

expressionListNE: expression { $$ = new ExpressionList(); $$->append($1); }
    | expression ',' expressionListNE { $$ = $3; $$->append($1); }
    ;

preset: expressionId {
        Expression *e = new Expression(Expression::DATA);
        e->data = new Data(Data::INT);
        e->dataType = Data::INT;
        e->data->value.i = 1;
        $$ = multiset(e, $1);
    }
    | expressionData {
        Expression *e = new Expression(Expression::DATA);
        e->data = new Data(Data::INT);
        e->dataType = Data::INT;
        e->data->value.i = 1;
        $$ = multiset(e, $1);
    }
    | expressionData '`' expressionId {
        $$ = multiset($1, $3);
    }
    | expressionData '`' expressionData {
        $$ = multiset($1, $3);
    }
    ;


%%

Command *branchCommand(Command::Type type, Expression *e, Command *c)
{
    Command *com = new Command(type);
    com->expression = e;
    com->command = c;
    return com;
}

Expression *binaryOp(Data::Type dataType, Expression::Type type, Expression *e1, Expression *e2)
{
    Expression *e = new Expression(type);
    e->dataType = dataType;
    e->left = e1;
    e->right = e2;
    return e;
}
Expression *unaryOp(Data::Type dataType, Expression::Type type, Expression *e1)
{
    Expression *e = new Expression(type);
    e->dataType = dataType;
    e->left = e1;
    return e;
}

Expression *incDec(Expression::Type type, Id id)
{
    SymbolTable::Symbol *symbol = currentSymbolTable->findSymbol(id);
    if(!symbol)
    {
        currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + id + " was not declared in this scope");
    }
    else
    {
        if(symbol->type == SymbolTable::FN)
            currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + id + " is a function, cannot use as variable");
        else if(symbol->dataType != Data::INT)
            currentParsedNet->addError(CPNet::SEMANTIC, "Symbol " + id + " is not an integer, cannot use ++/--");
    }
    if(!currentAssignmentAllowed)
        currentParsedNet->addError(CPNet::SEMANTIC, "++/-- operators are not allowed in this type of inscription");

    Expression *e = new Expression(type);
    e->dataType = Data::INT;
    e->id = id;
    return e;
}

Expression *convert(Data::Type type, Expression *e)
{
    if(e->dataType == type)
        return e;
    if((isSimpleType(e->dataType) && isSimpleType(type))
        || (e->dataType == Data::UNIT && type == Data::MULTIUNIT)
        || (e->dataType == Data::BOOL && type == Data::MULTIBOOL)
        || (e->dataType == Data::INT && type == Data::MULTIINT))
    {
        Expression *expr = unaryOp(type, Expression::CONVERT, e);
        return expr;
    }
    currentParsedNet->addError(CPNet::SEMANTIC, QString("Invalid conversion to ") + typeNames[type] + " in expression");
    return e;
}

bool isSimpleType(Data::Type type)
{
    return type == Data::UNIT || type == Data::BOOL || type == Data::INT;
}

Expression *plus(Expression *e1, Expression *e2)
{
    if(isSimpleType(e1->dataType) && isSimpleType(e2->dataType))
        return binaryOp(Data::INT, Expression::PLUS, convert(Data::INT, e1), convert(Data::INT, e2));
    if(e1->dataType == e2->dataType)
        return binaryOp(e1->dataType, Expression::PLUS, e1, e2);

    currentParsedNet->addError(CPNet::SEMANTIC, "Invalid conversion in + expression, multiset types do not match");
    return binaryOp(e1->dataType, Expression::PLUS, e1, e2);
}

Expression *minus(Expression *e1, Expression *e2)
{
    if(isSimpleType(e1->dataType) && isSimpleType(e2->dataType))
        return binaryOp(Data::INT, Expression::MINUS, convert(Data::INT, e1), convert(Data::INT, e2));
    if(e1->dataType == e2->dataType)
        return binaryOp(e1->dataType, Expression::MINUS, e1, e2);

    currentParsedNet->addError(CPNet::SEMANTIC, "Invalid conversion in - expression, multiset types do not match");
    return binaryOp(e1->dataType, Expression::PLUS, e1, e2);
}

Expression *mul(Expression *e1, Expression *e2)
{
    if(isSimpleType(e1->dataType) && isSimpleType(e2->dataType))
        return binaryOp(Data::INT, Expression::MUL, convert(Data::INT, e1), convert(Data::INT, e2));
    else if(isSimpleType(e1->dataType))
        return binaryOp(e2->dataType, Expression::MUL, convert(Data::INT, e1), e2);

    currentParsedNet->addError(CPNet::SEMANTIC, "Invalid conversion in * expression, first operand cant be multiset");
    return binaryOp(e2->dataType, Expression::MUL, e1, e2);
}

Expression *multiset(Expression *e1, Expression *e2)
{
    e1 = convert(Data::INT, e1);
    switch(e2->dataType)
    {
    case Data::UNIT:
        return binaryOp(Data::MULTIUNIT, Expression::MULTISET, e1, e2);
    case Data::BOOL:
        return binaryOp(Data::MULTIBOOL, Expression::MULTISET, e1, e2);
    case Data::INT:
        return binaryOp(Data::MULTIINT, Expression::MULTISET, e1, e2);
    default:
        currentParsedNet->addError(CPNet::SEMANTIC, "Invalid conversion in ` expression, right operand cant be multiset");
        return binaryOp(e2->dataType, Expression::MULTISET, e1, e2);
    }
    return NULL;
}

Expression *compare(Expression::Type type, Expression *e1, Expression *e2)
{
    if(isSimpleType(e1->dataType) && isSimpleType(e2->dataType))
        return binaryOp(Data::BOOL, type, convert(Data::INT, e1), convert(Data::INT, e2));
    else if(e1->dataType == e2->dataType)
        return binaryOp(Data::BOOL, type, e1, e2);

    currentParsedNet->addError(CPNet::SEMANTIC, QString("Invalid conversion in comparison expression ") + typeNames[e1->type] + ", " + typeNames[e2->type] );
    return binaryOp(Data::BOOL, type, e1, e2);
}
