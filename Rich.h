#ifndef RICH_H
#define RICH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Veblen.h"

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

struct RICH_Repr *rich_repr_create(uint8_t *buffer, size_t buf_length);
struct RICH_Repr *rich_repr_concat(struct RICH_Repr *repr1,
                                   struct RICH_Repr *repr2);
struct TOKNE_Repr *rich_repr_append(struct RICH_Repr **head,
                                    struct RICH_Repr *append);
void rich_repr_iter(struct RICH_Repr *head,
                    void (*iter_fn)(struct RICH_Repr *));
void rich_repr_print(struct RICH_Repr *repr);
void rich_repr_delete(struct RICH_Repr *repr);

struct RICH_Expr {
  enum {
    RICHXP_Constant,
    RICHXP_Variable,
    RICHXP_Application,
    RICHXP_Abstraction,
    RICHXP_Let,
    RICHXP_Letrec,
    RICHXP_Infix,
    RICHXP_Case,
    RICHXP_Pattern,
  } kind;

  union {
    struct RICH_Const *v_const;
    struct RICH_Variable *v_variable;
    struct RICH_Application *v_application;
    struct RICH_Abstraction *v_abstraction;
    struct RICH_Let *v_let;
    struct RICH_Letrec *v_letrec;
    struct RICH_Infix *v_infix;
    struct RICH_Case *v_case;
    struct RICH_Pattern *v_pattern;
  };

  struct RICH_Expr *next;
};

struct RICH_Expr *new_rich_expr_constant(struct RICH_Const *v);
struct RICH_Expr *new_rich_expr_variable(struct RICH_Variable *v);
struct RICH_Expr *new_rich_expr_application(struct RICH_Application *v);
struct RICH_Expr *new_rich_expr_abstraction(struct RICH_Abstrction *v);
struct RICH_Expr *new_rich_expr_let(struct RICH_Let *v);
struct RICH_Expr *new_rich_expr_letrec(struct RICH_Letrec *v);
struct RICH_Expr *new_rich_expr_infix(struct RICH_Infix *v);
struct RICH_Expr *new_rich_expr_case(struct RICH_Case *v);
struct RICH_Expr *new_rich_expr_pattern(struct RICH_Pattern *v);

struct RICH_Expr *rich_expr_append(struct RICH_Expr **head,
                                   struct RICH_Expr *append);
void rich_expr_iter(struct RICH_Expr *head,
                    void (*iter_fn)(struct RICH_Expr *));
void rich_expr_delete(struct RICH_Expr *v);

struct RICH_Const {
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

  struct RICH_Repr *repr;
};

struct RICH_Const *new_rich_const_number(struct RICH_Repr *repr);
struct RICH_Const *new_rich_const_string(struct RICH_Repr *repr);
struct RICH_Const *new_rich_const_operator(struct RICH_Repr *repr);
struct RICH_Const *new_rich_const_intric(struct RICH_Repr *repr);
struct RICH_Const *new_rich_const_ident(struct RICH_Repr *repr);
struct RICH_Const *new_rich_const_tuple(struct RICH_Repr *repr);
struct RICH_Const *new_rich_const_list(struct RICH_Repr *repr);
struct RICH_Const *new_rich_const_map(struct RICH_Repr *repr);

struct RICH_Variable {
  enum {
    VAR_Function,
    VAR_Constructor,
  } kind;

  struct RICH_Repr *repr;
};

struct RICH_Variable *new_rich_variable_function(struct RICH_Repr *repr);
struct RICH_Variable *new_rich_variable_constructor(struct RICH_Repr *repr);

struct RICH_Application {
  struct RICH_Expr *subj;
  struct RICH_Expr *obj;
};

struct RICH_Application *new_rich_application(struct RICH_Expr *subj,
                                              struct RICH_Expr *obj);

struct RICH_Abstraction {
  struct RICH_Pattern *patt;
  struct RICH_Expr *abs;
};

struct RICH_Abstraction *new_rich_abstraction(struct RICH_Pattern *patt,
                                              struct RICH_Expr *abs);

struct RICH_Let {
  struct RICH_Pattern *rhs;
  struct RICH_Expr *lhs;
  struct RICH_Expr *in;

  struct RICH_Let *next;
};

struct RICH_Let *new_rich_let(struct RICH_Pattern *rhs, struct RICH_Expr *lhs,
                              struct RICH_Expr *in);

struct RICH_Let *rich_let_append(struct RICH_Let **head,
                                 struct RICH_Let *append);
void rich_let_iter(struct RICH_Let *head, void (*iter_fn)(struct RICH_Let *));
void rich_let_delete(struct RICH_Let *v);

struct RICH_Letrec {
  struct RICH_Let *lets;
  size_t num_lets;
  struct RICH_Let *in;
};

struct RICH_Letrect *new_rich_letrch(struct RICH_Let *lets, size_t num_lets,
                                     struct RICH_Lets *in);

struct RICH_Infix {
  struct RICH_Expr *left;
  struct RICH_Expr *right;
  struct RICH_Repr *fatbar;
};

struct RICH_Infix *new_rich_infix(struct RICH_Expr *left,
                                  struct RICH_Expr *ight,
                                  struct RICH_Repr *fatbar);

struct RICH_Case {
  struct RICH_Variable *discrim;
  struct RICH_Pattern *patts;
  size_t num_patts;
};

struct RICH_Case *new_rich_case(struct RICH_Variable *var,
                                struct RICH_Pattern *patt, size_t num_patts);

struct RICH_Pattern {
  enum PATT_Type {
    PATT_Const,
    PATT_Variable,
    PATT_Cons,
  } kind;

  struct RICH_Expr *expr1;
  struct RICH_Expr *expr2;

  struct RICH_Pattern *next;
};

struct RICH_Pattern *new_rich_pattern(enum PATT_Type tyy,
                                      struct RICH_Expr *expr1,
                                      struct RICH_Expr *expr2);

struct RICH_Expr *rich_pattern_append(struct RICH_Expr **head,
                                      struct RICH_Expr *append);
void rich_pattern_iter(struct RICH_Pattern *head,
                       void (*iter_fn)(struct RICH_Pattern));
void rich_pattern_delte(struct RICH_Pattern *v);

#endif /* Rich.h */
