#ifndef ABSYN_H
#define ABSYN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Veblen.h"

/* The following needs to be pre-processed with AllocPPx.pl */

/*!(alloc-pp __absyn_heap)!*/

#define ABSYN_ALLOC(n) __absyn_heap_alloc_notrace(n)
#define ABSYN_REALLOC(mem, nn) __absyn_heap_realloc(mem, nn)
#define ABSYN_FREE(mem) __absyn_heap_free(mem)

#define ABSYN_String ABSYN_Repr
#define ABSYN_Ident ABSYN_Repr
#define ABSYN_Char ABSYN_Repr

struct ABSYN_Repr {
  uint8_t *buffer;
  size_t buf_length;

  struct ABSYN_Repr *next;
};

struct ABSYN_Node {
  enum {
    NODE_Numberic,
    NODE_String,
    NODE_Char,
    NODE_List,
    NODE_Tuple,
    NODE_Name,
    NODE_Expr,
  } kind;

  union {
    struct ABSYN_Numeric *v_numeric;
    struct ABSYN_String *v_string;
    struct ABSYN_Char *v_char;
    struct ABSYN_List *v_list;
    struct ABSYN_Tuple *v_tuple;
    struct ABSYN_Name *v_name;
    struct ABSYN_Expr *v_expr;
  };

  struct ABSYN_Node *next;
};

struct ABSYN_Numeric {
  enum {
    NUMERIC_Integer,
    NUMERIC_Real,
  } kind;

  union {
    intmax_t v_integer;
    long double v_real;
  };
};

struct ABSYN_List {
  enum {
    LIST_Normal,
    LIST_Comprehension,
  } kind;

  size_t num_items;
  struct ABSYN_Node *head;
  struct ABSYN_Node *items;
};

struct ABSYN_Tuple {
  enum {
    TUPLE_Unit,
    TUPLE_Normal,
  } kind;

  struct ABSYN_Node *v1;
  struct ABSYN_Node *v2;
};

struct ABSYN_Name {
  enum {
    NAME_Unbound,
    NAME_Bound,
    NAME_Typebound,
  } kind;

  struct ABSYN_Ident *value;
};

struct ABSYN_Expr {
  enum {
    EXPR_Unary,
    EXPR_Binary,
    EXPR_Cond,
  } kind;
};

#endif /* Absyn.h */
