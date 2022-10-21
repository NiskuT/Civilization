#ifndef INCLUDES_H
#define INCLUDES_H

/* Includefile computation related stuff (equally applicable to
   Ada with clause computation and Java package use computation)  */

#include "dia2code.h"
#include "decls.h"

extern namelist includes;

extern int have_include (char *name,char* package);
extern void add_include (char *name,char* package);
extern void push_include (umlclassnode *node);
extern void determine_includes (declaration *d, batch *b);
extern void determine_referenced_classes (declaration *d, batch *b);

#endif  /* INCLUDES_H */

