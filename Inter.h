#ifndef INTER_H
#define INTER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The following needs to be preprocessed with AllocPPx.pl */

/*!(alloc-pp __inter_heap)!*/

#define INTER_ALLOC(n) __inter_heap_alloc_notrace(n)
#define INTER_REALLOC(mem, nn) __inter_heap_realloc(mem, nn)
#define INTER_FREE(mem) __inter_heap_free(mem)

struct INTER_String {
  uint8_t *buffer;
  size_t buf_length;

  struct INTER_String *next;
};

struct INTER_Expr {
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
    struct INTER_Const *v_const;
    struct INTER_Variable *v_variable;
    struct INTER_Application *v_application;
    struct INTER_Abstraction *v_abstraction;
    struct INTER_Let *v_let;
    struct INTER_LetRec *v_letrec;
    struct INTER_LetIn *v_letin;
    struct INTER_Infix *v_infix;
    struct INTER_Case *v_case;
    struct INTER_Pattern *v_pattern;
    struct INTER_Cons *v_cons;
  };

  struct INTER_Expr *next;
};

struct INTER_Const {
  uint8_t repr[MAX_CONST_REPR + 1];
  struct INTER_Symtab **static_link;

  enum {
    CNR_Number,
    CNR_String,
    CNR_Operator,
    CNR_Intrin,
    CNR_Ident,
    /* TODO: Add more */
  } kind;
};

struct INTER_Variable {
  uint8_t repr[MAX_VARIABLE_REPR + 1];
  struct INTER_Symtab **static_link;

  enum {
    VAR_Function,
    VAR_Constructor,
  } kind;
};

struct INTER_Symtab {
  uintmax_t key_hash;
  struct INTER_String *key;
  struct INTER_String *value;

  struct INTER_Symtab *next;
};

struct INTER_Application {
  struct INTER_Expr *subj;
  struct INTER_Expr *obj;

  enum {
    REL_Surjective,
    REL_Injective,
  } rel;
};

struct INTER_Abstraction {
  struct INTER_Pattern *patt;
  struct INTER_Expr *abs;
};

struct INTER_Let {
  struct INTER_Pattern *rhs;
  struct INTER_Expr *lhs;
  struct INTER_Expr *in;

  struct INTER_Let *next;
};

struct INTER_LetRec {
  struct INTER_Let *lets;
  size_t num_lets;
  struct INTER_Let *in;
};

struct INTER_Infix {
  struct INTER_Expr *left;
  struct INTER_Expr *right;
  struct INTER_String *fatbar;
};

struct INTER_Case {
  struct INTER_Variable *discrim;
  struct INTER_Pattern *patts;
  size_t num_patts;
};

struct INTER_Pattern {
  enum {
    PATT_Const,
    PATT_Variable,
    PATT_Cons,
  } kind;

  struct INTER_Expr *expr1;
  struct INTER_Expr *expr2;

  struct INTER_Pattern *next;
};

#endif
