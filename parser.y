%{
    #include "compiler.h"
    #include <QList>
    #include <QVector>
    #include <QPair>
%}

%union{
    int intVal;
    bool boolVal;
    DataType dataType;
    IdList idList;
    QString id;
    ParameterList parameterList;
}

%token START_EXPRESSION START_DECLARATION
%token IF ELSE WHILE DO SWITCH CASE
%token ID
%token DATATYPE
%token UNITVAL BOOLVAL INTVAL
%token LEQ GEQ EQ

%left '='
%left '^'
%left '&' '|'
%left LEQ GEQ EQ
%left '+' '-'
%left '*' '/'
%nonassoc '!' UMINUS

%type <intVal> INTVAL
%type <boolVal> BOOLVAL
%type <id> ID
%type <dataType> DATATYPE
%type <parameterList> parameterList parameterListNE
%type <idList> idList

%start start

%%

start: START_DECLARATION declarations
    | START_EXPRESSION expression
    ;

declarations: /*empty*/ { $$ = NULL; }
    | declaration declarations { $$ = mergeDeclarations($1, $2); }
    ;

declaration: DATATYPE idList ';' { }
    | DATATYPE ID '(' parameterList ')' '{' commands '}' { }
    ;

idList: ID { $$ = newIDList($1); }
    | ID ',' idList { $$ = addIDList($3,$1); }
    ;

parameterList: /*empty*/ { }
    | DATATYPE ID
    | DATATYPE ID ',' parameterListNE
    ;

parameterListNE: DATATYPE ID
    | DATATYPE ID ',' parameterListNE
    ;


commands: /*empty*/ { }
    | commands command { }
    ;

command: WHILE '(' expression ')' command
    | IF '(' expression ')' command
    | IF '(' expression ')' command ELSE command
    | DO command WHILE '(' expression ')'
    | DATATYPE idList ';'
    | expression
    ;

expression: ID '=' expression
    | expression '^' expression
    | expression '&' expression
    | expression '|' expression
    | expression LEQ expression
    | expression GEQ expression
    | expression EQ expression
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | '(' expression ')'
    | '!' expression
    | '-' expression %prec UMINUS
    | ID '(' expressionList ')'
    ;

expressionList: /* empty */
    | expression
    | expression ',' expressionListNE
    ;

expressionListNE: expression
    | expression ',' expressionListNE
    ;
%%


