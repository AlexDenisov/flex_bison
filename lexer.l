%{
#include "parser.h"
%}

%option noyywrap

%%

"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"|"     { return ABS; }
-?[0-9]+  { yylval = atof(yytext); return NUMBER;  }
\n      { return EOL; }
[ \t]   { /* ignore */ }
.       { /* ignore */ }

%%

