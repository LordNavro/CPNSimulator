

%{
    #include "compiler.h"
    #include "parser.parser.hpp"

    int startToken = 0;
%}

al          [a-zA-z]
num         [0-9]
alnum       al|num

%s COMMENT

%option noyywrap nodefault yylineno


%%


%{
    if (startToken)
    {
        int t = startToken;
        startToken = 0;
        return t;
    }
%}

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

unit        {yylval.dataType = Data::UNIT; return DATATYPE;}
bool        {yylval.dataType = Data::BOOL; return DATATYPE;}
int         {yylval.dataType = Data::INT; return DATATYPE;}
multiunit   {yylval.dataType = Data::MULTIUNIT; return DATATYPE;}
multibool   {yylval.dataType = Data::MULTIBOOL; return DATATYPE;}
multiint    {yylval.dataType = Data::MULTIINT; return DATATYPE;}

true        {yylval.data = new Data(Data::BOOL); yylval.data->value.b = true; return DATA;}
false       {yylval.data = new Data(Data::BOOL); yylval.data->value.b = false; return DATA;}

{al}{alnum}*    {yylval.id = new Id(yytext); return ID;}
{num}+          {yylval.data = new Data(Data::BOOL); yylval.data->value.i = atoi(yytext); return DATA;}

">="                {return GEQ;}
"<="                {return LEQ;}
"=="                {return EQ;}
"!="                {return NEQ;}

[-+*/&|!^=,{}()%]   {return yytext[0];}

.               {return 0;}

%%

