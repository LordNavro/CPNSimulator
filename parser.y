%{

    #include "compiler.h"

%}

%union{
    int intVal;
    bool boolVal;
    DataType dataType;
    IdList *idList;
    Id *id;
    ParameterList *parameterList;
    Expression *expression;
    Expressionist *expressionList;
    Declaration *declaration;
    DeclarationList *declarationList;
}

%token START_EXPRESSION START_DECLARATION
%token IF ELSE WHILE DO SWITCH CASE RETURN
%token ID
%token DATATYPE
%token UNITVAL BOOLVAL INTVAL
%token LEQ GEQ EQ

%left '='
%left '^'
%left '&' '|'
%left LEQ GEQ EQ '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc '!' UMINUS

%type <intVal> INTVAL
%type <boolVal> BOOLVAL
%type <id> ID
%type <dataType> DATATYPE
%type <parameterList> parameterList parameterListNE
%type <idList> idList
%type <expression> expression
%type <expressionList> expressionList
%type <declaration> declaration
%type <declarationList> declarationList

%start start

%%

start: START_DECLARATION declarationList
    | START_EXPRESSION expression
    ;

declarationList: /*empty*/ { $$ = NULL; }
    | declaration declarationList { $$ = mergeDeclarations($1, $2); }
    ;

declaration: DATATYPE idList ';' { }
    | DATATYPE ID '(' parameterList ')' command { }
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
    | expression ';'
    | RETURN expression ';'
    | '{' commands '}'
    ;

expression: ID '=' expression
    | expression '^' expression
    | expression '&' expression
    | expression '|' expression
    | expression LEQ expression
    | expression GEQ expression
    | expression EQ expression
    | expression '>' expression
    | expression '<' expression
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '%' expression
    | '(' expression ')'
    | '!' expression
    | '-' expression %prec UMINUS
    | ID '(' expressionList ')'
    | INTVAL
    | BOOLVAL
    | '(' ')'
    ;

expressionList: /* empty */
    | expression
    | expression ',' expressionListNE
    ;

expressionListNE: expression
    | expression ',' expressionListNE
    ;
%%


