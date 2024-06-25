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
#define ORD_Ident ORD_Repr
#define ORD_Item ORD_Atom

struct ORD_Repr {
  uint8_t *buffer;
  size_t buf_length;

  struct ORD_Repr *next;
};

struct ORD_Symtab {
  uint64_t sym_tash;

  struct ORD_Key *key;
  struct ORD_Expr *value;

  struct ORD_Symtab *next;
};

struct ORD_Expr {
  struct ORD_Ident *name;
  struct ORD_Symtab **static_link;

  enum {
    ORDXP_Atom,
    ORDXP_Composite,
    ORDXP_Abstraction,
    ORDXP_Application,
  } kind;

  union {
    struct ORD_Atom *v_atom;
    struct ORD_Composite *v_composite;
    struct ORD_Abstraction *v_abstraction;
    struct ORD_Application *v_application;
  };

  bool is_normal;
  struct ORD_Expr *next;
};

struct ORD_Atom {
  enum {
    ATOM_Number,
    ATOM_Char,
    ATOM_String,
    ATOM_Ident,
  } kind;

  bool is_var;
  bool is_free;

  struct ORD_Repr *repr;
  struct ORD_Atom *next;
};

struct ORD_Composite {
  size_t arity;
  bool is_body;
  struct ORD_Item *items;
};

struct ORD_Application {
  struct ORD_Atom *arguments;
  struct ORD_Expr *body;
};

struct ORD_Abstraction {
  enum {
    ABS_Alpha,
    ABS_Beta,
    ABS_Eta,
  } kind;

  enum {
    REDEX_Normal,
    REDEX_Reduction,
    REDEX_Conversion,
  } state;

  struct ORD_Expr *subject;
  struct ORD_Expr *object;
};

#endif
