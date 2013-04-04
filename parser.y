%{

    #include "compiler.h"
    Command *branchCommand(Command::Type type, Expression *e, Command *c);
    Expression *binaryOp(Expression::Type type, Expression *e1, Expression *e2);
    Expression *unaryOp(Expression::Type type, Expression *e1);
    #include <iostream>
    extern int yylineno;
    extern char *yytext;

    int yylex();
    int yyerror(const char *message) { std::cout << message << std::endl; return 0;}

%}

%union{
    Data *data;
    Data::Type dataType;
    IdList *idList;
    Id *id;
    Command * command;
    CommandList * commandList;
    ParameterList *parameterList;
    Expression *expression;
    ExpressionList *expressionList;
    Declaration *declaration;
    DeclarationList *declarationList;
}

%token START_EXPRESSION START_DECLARATION START_MARKING START_PRESET
%token IF ELSE WHILE DO SWITCH CASE RETURN
%token UNIT BOOL INT MULTIUNIT MULTIBOOL MULTIINT
%token ID
%token DATATYPE
%token DATA
%token LEQ GEQ EQ NEQ

%left '='
%left '&' '|'
%left LEQ GEQ EQ NEQ '>' '<'
%left '+' '-'
%left '*' '/' '%'
%left '^'
%nonassoc '!' UMINUS

%type <data> DATA
%type <expression> marking markingNE
%type <expression> preset idOrData
%type <id> ID
%type <dataType> DATATYPE
%type <parameterList> parameterList parameterListNE
%type <idList> idList
%type <expression> expression
%type <expressionList> expressionList expressionListNE
%type <declaration> declaration
%type <declarationList> declarationList
%type <commandList> commandList
%type <command> command

%start start

%%

start: START_DECLARATION declarationList { parsedDeclaration = $2; }
    | START_EXPRESSION expression { parsedExpression = $2; }
    | START_MARKING marking { parsedExpression = $2; }
    | START_PRESET preset { parsedExpression = $2; }
    ;


declarationList: /*empty*/ { $$ = new DeclarationList(); }
    | declaration declarationList { $2->append($1); $$ = $2; }
    ;

declaration: DATATYPE idList ';' {
        $$ = new Declaration(Declaration::VAR);
        $$->dataType = $1;
        $$->idList = *$2;
        delete $2;
    }
    | DATATYPE ID '(' parameterList ')' command {
        $$ = new Declaration(Declaration::FN);
        $$->dataType = $1;
        $$->id = *$2;
        $$->parameterList = *$4;
        $$->command = $6;
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
    | DATATYPE ID {
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

command: WHILE '(' expression ')' command { $$ = branchCommand(Command::WHILE, $3, $5); }
    | IF '(' expression ')' command { $$ = branchCommand(Command::IF, $3, $5); }
    | IF '(' expression ')' command ELSE command { $$ = branchCommand(Command::IFELSE, $3, $5); $$->command2 = $7; }
    | DO command WHILE '(' expression ')' { $$ = branchCommand(Command::DOWHILE, $5, $2); }
    | DATATYPE idList ';' {
        $$ = new Command(Command::DECL);
        $$->dataType = $1;
        $$->idList = *$2;
        delete $2;
    }
    | expression ';' { $$ = new Command(Command::EXPR); $$->expression = $1; }
    | RETURN expression ';' { $$ = new Command(Command::RETURN); $$->expression = $2; }
    | '{' commandList '}' { $$ = new Command(Command::BLOCK); $$->commandList = *$2; delete $2; }
    ;

expression:   ID '=' expression {
        $$ = unaryOp(Expression::ASSIGN, $3);
        $$->id = *$1; delete $1;
    }
    | expression '&' expression {$$ = binaryOp(Expression::AND, $1, $3);}
    | expression '|' expression {$$ = binaryOp(Expression::OR,  $1, $3);}
    | expression LEQ expression {$$ = binaryOp(Expression::LEQ, $1, $3);}
    | expression GEQ expression {$$ = binaryOp(Expression::GEQ, $1, $3);}
    | expression EQ  expression {$$ = binaryOp(Expression::EQ,  $1, $3);}
    | expression NEQ expression {$$ = binaryOp(Expression::NEQ, $1, $3);}
    | expression '>' expression {$$ = binaryOp(Expression::GT,  $1, $3);}
    | expression '<' expression {$$ = binaryOp(Expression::LT,  $1, $3);}
    | expression '+' expression {$$ = binaryOp(Expression::PLUS, $1, $3);}
    | expression '-' expression {$$ = binaryOp(Expression::MINUS, $1, $3);}
    | expression '*' expression {$$ = binaryOp(Expression::MUL, $1, $3);}
    | expression '/' expression {$$ = binaryOp(Expression::DIV, $1, $3);}
    | expression '%' expression {$$ = binaryOp(Expression::MOD, $1, $3);}
    | expression '^' expression {$$ = binaryOp(Expression::MULTISET, $1, $3);}
    | '(' expression ')'            {$$ = $2;}
    | '!' expression                {$$ = unaryOp(Expression::NOT, $2);}
    | '-' expression %prec UMINUS   {$$ = unaryOp(Expression::UMINUS, $2);}
    | ID '(' expressionList ')'     {
        $$ = new Expression(Expression::FN);
        $$->id = *$1;
        delete $1;
        $$->expressionList = $3;
    }
    | DATA  {$$ = new Expression(Expression::DATA); $$->data = $1; }
    | ID {$$ = new Expression(Expression::VAR); $$->id = *$1; delete $1; }
    ;

expressionList: /* empty */ { $$ = new ExpressionList(); }
    | expressionListNE { $$ = $1; }
    ;

expressionListNE: expression { $$ = new ExpressionList(); $$->append($1); }
    | expression ',' expressionListNE { $$ = $3; $$->append($1); }
    ;

marking: /* empty */ { $$ = NULL; }
    | markingNE {$$ = $1;}
    ;

markingNE: DATA '^' DATA {
        Expression *e1 = new Expression(Expression::DATA); e1->data = $1;
        Expression *e2 = new Expression(Expression::DATA); e2->data = $3;
        $$ = binaryOp(Expression::MULTISET, e1, e2);
    }
    | markingNE '+' DATA '^' DATA {
        Expression *e1 = new Expression(Expression::DATA); e1->data = $3;
        Expression *e2 = new Expression(Expression::DATA); e2->data = $5;
        $$ = binaryOp(Expression::PLUS, $1, binaryOp(Expression::MULTISET, e1, e2)); }
    ;

preset: idOrData '^' idOrData { $$ = binaryOp(Expression::MULTISET, $1, $3); }
    | preset '+' idOrData '^' idOrData { $$ = binaryOp(Expression::PLUS, $1, binaryOp(Expression::MULTISET, $3, $5)); }
    | DATA  {$$ = new Expression(Expression::DATA); $$->data = $1;}
    ;

idOrData: DATA  {$$ = new Expression(Expression::DATA); $$->data = $1; }
    | ID {$$ = new Expression(Expression::VAR); $$->id = *$1; delete $1; }
    ;

%%

Command *branchCommand(Command::Type type, Expression *e, Command *c)
{
    Command *com = new Command(type);
    com->expression = e;
    com->command = c;
    return com;
}

Expression *binaryOp(Expression::Type type, Expression *e1, Expression *e2)
{
    Expression *e = new Expression(type);
    e->left = e1;
    e->right = e2;
    return e;
}
Expression *unaryOp(Expression::Type type, Expression *e1)
{
    Expression *e = new Expression(type);
    e->left = e1;
    return e;
}


extern void yyerror(char* msg)
{
    std::cout << " Syntax Error in Line : " << yylineno << " : " << msg << "\n";
}
