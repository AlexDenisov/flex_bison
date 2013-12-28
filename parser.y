%{

#include <stdio.h>

extern int yylex(void);
extern int yyerror(char *s);

%}

%token NUMBER
%token ADD SUB MUL DIV 
%token ABS NEGATE
%token EOL
%token OP CP

%%

calclist: /*nothing*/
  | calclist exp EOL { printf("= %d\n", $2); }
  ;

exp: factor
  | exp ADD factor { $$ = $1 + $3; }
  | exp SUB factor { $$ = $1 - $3; }
  ;

factor: term
  | factor MUL term { $$ = $1 * $3; }
  | factor DIV term { $$ = $1 / $3; } 
  ;

term: NUMBER 
  | ABS term { $$ = $2 >= 0 ? $2 : - $2; }
  | NEGATE term { $$ = $2 * -1; }
  | OP exp CP { $$ = $2; }
  ;

%%

int main(int argc, char **argv)
{
  yyparse();
  return 0;
}

int yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
  return 0;
}

