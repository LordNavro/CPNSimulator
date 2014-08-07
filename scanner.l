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
    #include "parser.parser.hpp"
    #include <QtGui>
    #define DEBUG(token) //qDebug() << yytext << "as" << token

    extern int yyparse(void);
%}


%x COMMENT

%option noyywrap
%option nodefault
%option yylineno


%%


%{
    if (startSymbol)
    {
        int t = startSymbol;
        startSymbol = 0;
        return t;
    }
%}

"//".*              {DEBUG("lineComment");}
"/*"                {DEBUG("blockCommentBegin"); BEGIN(COMMENT);}
<COMMENT>"*/"       {DEBUG("blockCommentEnd"); BEGIN(INITIAL);}
<COMMENT>\n         {}
<COMMENT>\r         {}
<COMMENT>.          {}
<COMMENT><<EOF>>    {DEBUG("blockCommentEOF");currentParsedNet->addError(CPNet::LEXICAL, QString("Unfinished comment")); BEGIN(INITIAL);}
[\t\n\r]            {}
" "                 {}

if      {DEBUG("if"); return IF;}
else    {DEBUG("else"); return ELSE;}
while   {DEBUG("while"); return WHILE;}
do      {DEBUG("do"); return DO;}
for     {DEBUG("for"); return FOR;}
return  {DEBUG("return"); return RETURN;}

bool        {DEBUG("bool"); yylval.dataType = Data::BOOL; return DATATYPE;}
int         {DEBUG("int"); yylval.dataType = Data::INT; return DATATYPE;}
multiunit   {DEBUG("multiunit"); yylval.dataType = Data::MULTIUNIT; return DATATYPE;}
multibool   {DEBUG("multibool"); yylval.dataType = Data::MULTIBOOL; return DATATYPE;}
multiint    {DEBUG("multiint"); yylval.dataType = Data::MULTIINT; return DATATYPE;}

unit        {DEBUG("unitdata"); yylval.data = new Data(Data::UNIT); yylval.data->value.b = true; return DATA;}
nounit      {DEBUG("unitdata"); yylval.data = new Data(Data::UNIT); yylval.data->value.b = false; return DATA;}

true        {DEBUG("booldata"); yylval.data = new Data(Data::BOOL); yylval.data->value.b = true; return DATA;}
false       {DEBUG("booldata"); yylval.data = new Data(Data::BOOL); yylval.data->value.b = false; return DATA;}

[0-9]+                  {DEBUG("intdata"); yylval.data = new Data(Data::INT); yylval.data->value.i = atoi(yytext); return DATA;}

[a-zA-Z][a-zA-Z0-9_]*   {DEBUG("id"); yylval.id = new Id(yytext); return ID;}

">="                {DEBUG("geq"); return GEQ;}
"<="                {DEBUG("leq"); return LEQ;}
"=="                {DEBUG("eq"); return EQ;}
"!="                {DEBUG("neq"); return NEQ;}
"&&"                {DEBUG("and"); return AND;}
"||"                {DEBUG("or"); return OR;}
"--"                {DEBUG("--"); return DMINUS;}
"++"                {DEBUG("++"); return DPLUS;}

[-+*/!`=,{}<>()%;?:]    {DEBUG("implicit"); return yytext[0];}

.               {DEBUG("unknown"); currentParsedNet->addError(CPNet::LEXICAL, QString("Unknown symbol ") + yytext);}

%%


int parseQString(QString input, int symbol)
{
    yylineno = 0;
    startSymbol = symbol;
    YY_BUFFER_STATE buffer = yy_scan_string (input.toAscii().data());
    int result  = yyparse();
    yy_delete_buffer(buffer);
    return result;
}
