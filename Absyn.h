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

struct AST_String;
struct AST_Literal;
struct AST_Postfix;
struct AST_Prefix;
struct AST_Infix;
struct AST_Expr;
struct AST_Tag;
struct AST_Binding;

struct AST_String {
  uint8_t *buffer;
  size_t buf_length;

  struct AST_String *next;
};

struct AST_Literal {
  enum {
    LIT_Integer,
    LIT_Real,
    LIT_Char,
    LIT_String,
    LIT_List,
    LIT_Map,
    LIT_TrainCase,
    LIT_PascalCase,
  } kind;

  struct AST_String *values;
  size_t num_values;
};

struct AST_Prefix {
  enum {
    PREFIX_Plus,
    PREFIX_Minus,
    PREFIX_BitwiseNot,
    PREFIX_LogicalNot,
  } kind;

  struct AST_Expr *value;
};

struct AST_Postfix {
  enum {
    POSTFIX_Call,
    POSTFIX_Access,
    POSTFIX_Index,
  } kind;

  struct AST_Expr *value;
};

struct AST_Infix {
  enum {
    INFIX_Add,
    INFIX_Sub,
    INFIX_Mul,
    INFIX_Div,
    INFIX_Mod,
    INFIX_Shr,
    INFIX_Shl,
    INFIX_Eq,
    INFIX_Ne,
    INFIX_Gt,
    INFIX_Ge,
    INFIX_Lt,
    INFIX_Le,
  } kind;

  struct AST_Expr *left;
  struct AST_Expr *right;
};

struct AST_Expr {
  enum {
    EXPR_Postfix,
    EXPR_Prefix,
    EXPR_Infix,
    EXPR_Literal,
  } kind;

  union {
    struct AST_Postfix *v_postfix;
    struct AST_Prefix *v_prefix;
    struct AST_Infix *v_infix;
    struct AST_Literal *v_literal;
  };

  struct AST_Expr *next;
  struct AST_Tag *tag;
};

struct AST_Binding {
  enum {
    BINDING_Func,
    BINDING_Type,
  } kind;

  struct AST_Expr *binding;
};

struct AST_Tag {
  size_t line_num;
  /* TODO: Add control flow, semantics, etc */
};

#endif /* Absyn.h */
