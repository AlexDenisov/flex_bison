#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

extern int yylex(void);
extern int yyrestart(FILE *);
extern int yylineno;

// prototypes

void scanStdin();
void scanFiles(char **names, int count);
unsigned symhash(char *sym);
int symcompare(const void *xa, const void *xb);
void printrefs();

// driver

int main(int argc, char **argv) 
{
  if ( argc < 2 ) {
    scanStdin();
  } else {
    scanFiles(argv, argc);  
  }

  printrefs();
}

// implementation

void scanStdin()
{
  curfilename = "(stdin)";
  yylineno = 1;
  yylex();
}

void scanFiles(char **names, int count)
{
  for(int i = 1; i < count; i++) {
    FILE *f = fopen(names[i], "r");
    if(!f) { 
      perror(names[i]);
      exit(1);
    }
    curfilename = names[i];
    yyrestart(f); 
    yylineno = 1; 
    yylex(); 
    fclose(f);
  }
}

unsigned symhash(char *sym)
{
  unsigned int hash = 0; 
  unsigned c;
  while ( (c = *sym++) ) {
    hash = hash * 9 ^ c;
  }

  return hash; 
}

symbol *lookup(char* sym)
{
  symbol *sp = &symtab[ symhash(sym) % NHASH ];
  int scount = NHASH;
  while (--scount >= 0) {
    if(sp->name && !strcmp(sp->name, sym)) {
      return sp;
    }

    if(!sp->name) { 
      sp->name = strdup(sym);
      sp->reflist = 0;
      return sp;
    }

    if(++sp >= symtab+NHASH) {
      sp = symtab;
    }
  }
  fputs("symbol table overflow\n", stderr); 
  abort();
}

void addref(int lineno, char *filename, char *word, int flags)
{
  ref *r;
  symbol *sp = lookup(word);
  if( sp->reflist && 
      sp->reflist->lineno == lineno && 
      sp->reflist->filename == filename) 
  {
    return;
  }

  r = malloc(sizeof(ref));
  if(!r) {
    fputs("out of space\n", stderr); 
    abort(); 
  } 
  r->next = sp->reflist;
  r->filename = filename;
  r->lineno = lineno;
  r->flags = flags;
  sp->reflist = r;
}

int symcompare(const void *xa, const void *xb)
{
  const symbol *a = xa; 
  const symbol *b = xb;

  if(!a->name) {
    if(!b->name) {
      return 0;
    }
    
    return 1;
  }

  if(!b->name) {
    return -1;
  }

  return strcmp(a->name, b->name); 
}

void printrefs()
{
  symbol *sp;
  qsort(symtab, NHASH, sizeof(symbol), symcompare); 
  
  for(sp = symtab; sp->name && sp < symtab + NHASH; sp++) {
    char *prevfn = NULL; 

    ref *rp = sp->reflist;
    ref *rpp = 0; /* previous ref */
    ref *rpn;
    do {
      rpn = rp->next; rp->next = rpp; rpp = rp;
      rp = rpn;
    } while(rp);

    printf("%10s", sp->name);

    for(rp = rpp; rp; rp = rp->next) {
      if(rp->filename == prevfn) { 
        printf(" %d", rp->lineno);
      } else {
        printf(" %s:%d", rp->filename, rp->lineno);
        prevfn = rp->filename; 
      }
    }

    printf("\n");
  }

}


