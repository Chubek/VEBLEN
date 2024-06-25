#ifndef ORD_H
#define ORD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Veblen.h"

/* The following needs to be preprocessed with AllocPPx.pl */

/*!(alloc-pp __ord_heap)!*/

#define ORD_ALLOC(n) __ord_heap_alloc_notrace(n)
#define ORD_REALLOC(mem, nn) __ord_heap_realloc(mem, nn)
#define ORD_FREE(mem) __ord_heap_free(mem)

#define ORD_Item ORD_Atom

struct ORD_Sexpr {
  enum {
    SEXP_Atom,
    SEXP_List,
  } kind;

  union {
    struct ORD_Atom *v_atom;
    struct ORD_List *v_list;
  };
};

struct ORD_Value {
  enum {
    OVAL_Ident,
    OVAL_String,
    OVAL_Number,
    OVAL_Integer,
    OVAL_Operator,
    OVAL_Char,
  } kind;

  uint8_t value_repr[MAX_VAL_REPR + 1];
};

struct ORD_Atom {
  struct ORD_Value *v_repr;
  struct ORD_Atom *next;
};

struct ORD_List {
  size_t num_item;
  struct ORD_Item *items;
};

#endif
