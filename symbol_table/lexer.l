%option noyywrap nodefault yylineno

%{
#include "types.h"
%}

%%

[A-Za-z_]+ { addref(yylineno, curfilename, yytext, 0); }
.|\n {}

%%

