
%%

%token START_EXPRESSION START_DECLARATION
%token IF ELSE WHILE DO SWITCH CASE

%start start

start: START_EXPRESSION expression
    | START_DECLARATION declaration;



%%


