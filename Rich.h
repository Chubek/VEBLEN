#ifndef RICH_H
#define RICH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The following needs to be preprocessed with AllocPPx.pl */

/*!(alloc-pp __inter_heap)!*/

#define RICH_ALLOC(n) __inter_heap_alloc_notrace(n)
#define RICH_REALLOC(mem, nn) __inter_heap_realloc(mem, nn)
#define RICH_FREE(mem) __inter_heap_free(mem)

#define RICH_Member RICH_Variant
#define RICH_Ident RICH_Repr

struct RICH_Repr;
struct RICH_Exrp;
struct RICH_Const;
struct RICH_Variable;
struct RICH_Abstraction;
struct RICH_Application;
struct RICH_Let;
struct RICH_Letrec;
struct RICH_Case;
struct RICH_Pattern;

struct RICH_Repr {
  uint8_t *buffer;
  size_t buf_length;

  struct RICH_Repr *next;
};

struct RICH_Expr {
  enum {
    EXP_Constant,
    EXP_Variable,
    EXP_Application,
    EXP_Abstraction,
    EXP_Let,
    EXP_Letrec,
    EXP_Infix,
    EXP_Case,
    EXP_Pattern,
  } kind;

  union {
    struct RICH_Const *v_const;
    struct RICH_Variable *v_variable;
    struct RICH_Application *v_application;
    struct RICH_Abstraction *v_abstraction;
    struct RICH_Let *v_let;
    struct RICH_Letrec *v_letrec;
    struct RICH_LetIn *v_letin;
    struct RICH_Infix *v_infix;
    struct RICH_Case *v_case;
    struct RICH_Pattern *v_pattern;
  };

  struct RICH_Expr *next;
};

struct RICH_Const {
  uint8_t repr[MAX_CONST_REPR + 1];
  struct RICH_Symtab **static_link;

  enum {
    CONST_Number,
    CONST_String,
    CONST_Operator,
    CONST_Intrin,
    CONST_Ident,
    CONST_Tuple,
    CONST_List,
    CONST_Map,
  } kind;
};

struct RICH_Variable {
  uint8_t repr[MAX_VARIABLE_REPR + 1];
  struct RICH_Symtab **static_link;

  enum {
    VAR_Function,
    VAR_Constructor,
  } kind;
};

struct RICH_Symtab {
  uintmax_t key_hash;
  struct RICH_Repr *key;
  struct RICH_Repr *value;

  struct RICH_Symtab *next;
};

struct RICH_Application {
  struct RICH_Expr *subj;
  struct RICH_Expr *obj;

  enum {
    REL_Surjective,
    REL_Injective,
  } rel;
};

struct RICH_Abstraction {
  struct RICH_Pattern *patt;
  struct RICH_Expr *abs;
};

struct RICH_Let {
  struct RICH_Pattern *rhs;
  struct RICH_Expr *lhs;
  struct RICH_Expr *in;

  struct RICH_Let *next;
};

struct RICH_Letrec {
  struct RICH_Let *lets;
  size_t num_lets;
  struct RICH_Let *in;
};

struct RICH_Infix {
  struct RICH_Expr *left;
  struct RICH_Expr *right;
  struct RICH_Repr *fatbar;
};

struct RICH_Case {
  struct RICH_Variable *discrim;
  struct RICH_Pattern *patts;
  size_t num_patts;
};

struct RICH_Pattern {
  enum {
    PATT_Const,
    PATT_Variable,
    PATT_Cons,
  } kind;

  struct RICH_Expr *expr1;
  struct RICH_Expr *expr2;

  struct RICH_Pattern *next;
};

#endif /* Rich.h */
