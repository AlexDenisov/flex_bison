#define NHASH 9997

// types

typedef struct _symbol {
  char *name;
  struct _ref *reflist; 
} symbol;

typedef struct _ref {
  struct _ref *next; 
  char *filename; 
  int flags;
  int lineno;
} ref;

// vars

static symbol symtab[NHASH];
static char *curfilename;

// prototypes

symbol *lookup(char *sym);
void addref(int lineno, char *filename, char *word, int flags);

