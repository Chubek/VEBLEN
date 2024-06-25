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

#define ORD_Key ORD_Repr
#define ORD_Tag ORD_Repr

struct ORD_Repr {
  uint8_t *buffer;
  size_t buf_length;

  struct ORD_Repr *next;
};

struct ORD_Symtab {
  uint64_t sym_tash;

  struct ORD_Key *key;
  struct ORD_Atom *value;

  struct ORD_Symtab *next;
};

struct ORD_Calc {
  enum {
    CALC_Atom,
    CALC_Abstraction,
  } kind;

  union {
    struct ORD_Atom *v_atom;
    struct ORD_Abstraction *v_abstraction;
  };

  struct ORD_Calc *next;
};

struct ORD_Abstraction {
  struct ORD_Tag *tag;
  struct ORD_Atom *arguments;
  struct ORD_Atom *free_vars;
  struct ORD_Atom *bound_vars;

  struct ORD_Calc *result;
};

struct ORD_Atom {
  enum {
    ATOM_Variable,
    ATOM_Constant,
  } kind;

  union {
    struct ORD_Variable *v_variable;
    struct ORD_Constant *v_constant;
  };

  struct ORD_Atom *next;
};

struct ORD_Variable {
  enum {
    VAR_Argument,
    VAR_Free,
    VAR_Bound,
  } kind;
  struct ORD_Symtab *static_link;

  struct ORD_Repr *repr;
};

struct ORD_Constant {
  enum {
    CONST_Constructor,
    CONST_Operator,
    CONST_Intrinsic,
  } kind;
  struct ORD_Symtab *static_link;

  struct ORD_Repr *repr;
};

#endif
