

%{
    #include "compiler.h"
    #include "parser.tab.h"

    if (startToken)
    {
        int t = start_token;
        start_token = 0;
        return t;
    }
%}

al          [a-zA-z]
num         [0-9]
alnum       al|num

%s COMMENT

%option noyywrap nodefault yylineno


%%

"//".*              {}
"/*"                {BEGIN(COMMENT);}
<COMMENT>"*/"       {BEGIN(INITIAL);}
<COMMENT>.          {}
<COMMENT><<EOF>>    {return 0;}
\t|\n|\r|" "        {if(yytext[0] == '\n') yylineno++;}

if      {return IF;}
else    {return ELSE;}
while   {return WHILE;}
do      {return DO;}
case    {return CASE;}
switch  {return SWITCH;}
return  {return RETURN;}

unit        {yylval.dataType = UNIT; return DATATYPE;}
bool        {yylval.dataType = BOOL; return DATATYPE;}
int         {yylval.dataType = INT; return DATATYPE;}
multiunit   {yylval.dataType = MULTIUNIT; return DATATYPE;}
multibool   {yylval.dataType = MULTIBOOL; return DATATYPE;}
multiint    {yylval.dataType = MULTIINT; return DATATYPE;}

true        {yylval.boolVal = true; return BOOLVAL;}
false       {yylval.boolVal = false; return BOOLVAL;}

{al}{alnum}*    {yylval.id = yytext; return ID;}
{num}+          {yylval.intVal = atoi(yytext); return INTVAL}

"=="                {return EQ;}
">="                {return GEQ;}
"<="                {return LEQ;}
[-+*/&|!^=,{}()%]   {return yytext[0];}

.               {return 0;}

%%

yylex()
