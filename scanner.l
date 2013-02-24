/* from the flex and bison book do a word count */

%{
int chars=0;
int words=0;
int lines=0;
%}

%%

[a-zA-Z]+ { words++; chars += strlen(yytext); }
\n    { chars++; lines++; }
.     { chars++; }

%%
