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
#define ABSYN_Type ABSYN_Repr
#define ABSYN_Operator ABSYN_Repr
#define ABSYN_ProductItem ABSYN_Variant

struct ABSYN_Repr;
struct ABSYN_Node;
struct ABSYN_Numeric;
struct ABSYN_List;
struct ABSYN_Tuple;
struct ABSYN_Name;
struct ABSYN_Expr;
struct ABSYN_UnaryExpr;
struct ABSYN_BinaryExpr;
struct ABSYN_CondExpr;
struct ABSYN_LoopExpr;
struct ABSYN_LetExpr;
struct ABSYN_MatchExpr;
struct ABSYN_FuncExpr;
struct ABSYN_Adt;
struct ABSYN_SumAdt;
struct ABSYN_Variant;
struct ABSYN_ProductAdt;
struct ABSYN_Module;
struct ABSYN_Mapping;
struct ABSYN_Pattern;
struct ABSYN_Signature;
struct ABSYN_Value;

struct ABSYN_Repr {
  uint8_t *buffer;
  size_t buf_length;

  struct ABSYN_Repr *next;
};

struct ABSYN_Repr *absyn_repr_create(uint8_t *buffer, size_t buf_length);
struct ABSYN_Repr *absyn_repr_concat(struct ABSYN_Repr *repr1,
                                     struct ABSYN_Repr *repr2);
struct TOKNE_Repr *absyn_repr_append(struct ABSYN_Repr **head,
                                     struct ABSYN_Repr *append);
void absyn_repr_iter(struct ABSYN_Repr *head, 
				     void (*iter_fn)(struct ABSYN_Repr*));
void absyn_repr_print(struct ABSYN_Repr *repr);
void absyn_repr_delete(struct ABSYN_Repr *repr);

struct ABSYN_Node {
  enum {
    NODE_Numberic,
    NODE_String,
    NODE_Char,
    NODE_List,
    NODE_Tuple,
    NODE_Name,
    NODE_Expr,
    NODE_Adt,
    NODE_Pattern,
    NODE_Module,
    NODE_Signature,
    NODE_Value,
  } kind;

  union {
    struct ABSYN_Numeric *v_numeric;
    struct ABSYN_String *v_string;
    struct ABSYN_Char *v_char;
    struct ABSYN_List *v_list;
    struct ABSYN_Tuple *v_tuple;
    struct ABSYN_Name *v_name;
    struct ABSYN_Expr *v_expr;
    struct ABSYN_Adt *v_adt;
    struct ABSYN_Pattern *v_pattern;
    struct ABSYN_Module *v_module;
    struct ABSYN_Signature *v_signature;
    struct ABSYN_Value *v_value;
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
    NAME_Terminal,
    NAME_Constructor,
    NAME_Typevar,
  } kind;

  struct ABSYN_Ident *value;
  struct ABSYN_Name *next;
};

struct ABSYN_Expr {
  enum {
    EXPR_Unary,
    EXPR_Binary,
    EXPR_Cond,
    EXPR_Loop,
    EXPR_Let,
    EXPR_Match,
    EXPR_Func,
  } kind;

  union {
    struct ABSYN_UnaryExpr *v_unary;
    struct ABSYN_BinaryExpr *v_binary;
    struct ABSYN_CondExpr *v_cond;
    struct ABSYN_LoopExpr *v_loop;
    struct ABSYN_LetExpr *v_let;
    struct ABSYN_MatchExpr *v_match;
    struct ABSYN_FuncExpr *v_func;
  };
};

struct ABSYN_UnaryExpr {
  struct ABSYN_Operator *operation;
  struct ABSYN_Expr *monad;
};

struct ABSYN_BinaryExpr {
  struct ABSYN_Operator *operation;
  struct ABSYN_Expr *left;
  struct ABSYN_Expr *right;
};

struct ABSYN_CondExpr {
  struct ABSYN_Expr *discriminant;
  struct ABSYN_Expr *body;
};

struct ABSYN_LoopExpr {
  struct ABSYN_Repr *subject;
  struct ABSYN_Repr *object;
  struct ABSYN_Expr *body;
};

struct ABSYN_LetExpr {
  bool is_fixed_point;
  struct ABSYN_Ident *name;
  struct ABSYN_Pattern *lhs_patterns;
  struct ABSYN_Expr *rhs_expr;
};

struct ABSYN_MatchExpr {
  struct ABSYN_Ident *dscriminant;
  struct ABSYN_Expr *body;
};

struct ABSYN_FuncExpr {
  struct ABSYN_Expr *lhs;
  struct ABSYN_Expr *rhs;
};

struct ABSYN_Adt {
  enum {
    ADT_Product,
    ADT_Sum,
  } kind;

  union {
    struct ABSYN_SumAdt *v_sum;
    struct ABSYN_ProductAdt *v_product;
  };

  struct ABSYN_Ident *name;
};

struct ABSYN_SumAdt {
  size_t arity;
  struct ABSYN_Ident *name;
  struct ABSYN_Variant *variants;
};

struct ABSYN_Variant {
  struct ABSYN_Ident *name;
  struct ABSYN_Name *value;

  struct ABSYN_Variant *next;
};

struct ABSYN_ProductAdt {
  size_t num_item;
  struct ABSYN_ProductItem *items;
};

struct ABSYN_Pattern {
  struct ABSYN_Name *constructor;
  struct ABSYN_Expr *discriminators;
};

struct ABSYN_Module {
  size_t arity;
  struct ABSYN_Ident *name;
  struct ABSYN_Value *decl_values;

  struct ABSYN_Module *next;
};

struct ABSYN_Mapping {
  enum {
    MAP_Iso,
    MAP_Epi,
    MAP_Hom,
    MAP_Functor,
    MAP_Injective,
    MAP_Bijective,
    MAP_Surjective,
    MAP_Curry,
  } kind;

  struct ABSYN_Repr *from;
  struct ABSYN_Repr *to;
};

struct ABSYN_Value {
  struct ABSYN_Name *name;
  struct ABSYN_Signature *signature;
};

struct ABSYN_Signature {
  size_t arity;
  struct ABSYN_Mapping *mappins;
  struct ABSYN_Repr *result;
};

struct ABSYN_Node *new_absyn_node_numeric_integer(intmax_t v);
struct ABSYN_Node *new_absyn_node_numeric_real(long double v);
struct ABSYN_Node *new_absyn_node_string(struct ABSYN_Repr *v);
struct ABSYN_Node *new_absyn_node_char(struct ABSYN_Repr *v);
struct ABSYN_Node *new_absyn_node_list_normal(size_t num_items,
                                              struct ABSYN_Node *head,
                                              struct ABSYN_Node *items);
struct ABSYN_Node *new_absyn_node_list_comprehension(size_t num_items,
                                                     struct ABSYN_Node *head,
                                                     struct ABSYN_Node *items);
struct ABSYN_Node *new_absyn_node_tuple_unit(struct ABSYN_Node *v1);
struct ABSYN_Node *new_absyn_node_tuple_normal(struct ABSYN_Node *v1,
                                               struct ABSYN_Node *v2);
struct ABSYN_Node *new_absyn_node_name_terminal(struct ABSYN_Ident *value);
struct ABSYN_Node *new_absyn_node_name_constructor(struct ABSYN_Ident *value);
struct ABSYN_Node *new_absyn_node_name_typevar(struct ABSYN_Ident *value);
struct ABSYN_Node *new_absyn_node_expr_unary(struct ABSYN_UnaryExpr *v);
struct ABSYN_Node *new_absyn_node_expr_binary(struct ABSYN_BinaryExpr *v);
struct ABSYN_Node *new_absyn_node_expr_cond(struct ABSYN_CondExpr *v);
struct ABSYN_Node *new_absyn_node_expr_loop(struct ABSYN_LoopExpr *v);
struct ABSYN_Node *new_absyn_node_expr_let(bool is_fixed_point,
                                           struct ABSYN_Ident *name,
                                           struct ABSYN_Pattern *lhs_patterns,
                                           struct ABSYN_Expr *rhs_expr);
struct ABSYN_Node *new_absyn_node_expr_match(struct ABSYN_Ident *discriminant,
                                             struct ABSYN_Expr *body);
struct ABSYN_Node *new_absyn_node_expr_func(struct ABSYN_Expr *lhs,
                                            struct ABSYN_Expr *rhs);
struct ABSYN_Node *
new_absyn_node_adt_product(struct ABSYN_ProductAdt *v_product,
                           struct ABSYN_Ident *name);
struct ABSYN_Node *new_absyn_node_adt_sum(struct ABSYN_SumAdt *v_sum,
                                          struct ABSYN_Ident *name);
struct ABSYN_Node *new_absyn_node_pattern(struct ABSYN_Name *constructor,
                                          struct ABSYN_Expr *discriminators);
struct ABSYN_Node *new_absyn_node_module(size_t arity, struct ABSYN_Ident *name,
                                         struct ABSYN_Value *decl_values);
struct ABSYN_Node *new_absyn_node_signature(size_t arity,
                                            struct ABSYN_Mapping *mappings,
                                            struct ABSYN_Repr *result);

struct ABSYN_Numeric *new_absyn_numeric_integer(intmax_t v);
struct ABSYN_Numeric *new_absyn_numeric_real(long double v);
struct ABSYN_List *new_absyn_list_normal(size_t num_items,
                                         struct ABSYN_Node *head,
                                         struct ABSYN_Node *items);
struct ABSYN_List *new_absyn_list_comprehension(size_t num_items,
                                                struct ABSYN_Node *head,
                                                struct ABSYN_Node *items);
struct ABSYN_Tuple *new_absyn_tuple_unit(struct ABSYN_Node *v1);
struct ABSYN_Tuple *new_absyn_tuple_normal(struct ABSYN_Node *v1,
                                           struct ABSYN_Node *v2);
struct ABSYN_Name *new_absyn_name_terminal(struct ABSYN_Ident *value);
struct ABSYN_Name *new_absyn_name_constructor(struct ABSYN_Ident *value);
struct ABSYN_Name *new_absyn_name_typevar(struct ABSYN_Ident *value);
struct ABSYN_Expr *new_absyn_expr_unary(struct ABSYN_UnaryExpr *v);
struct ABSYN_Expr *new_absyn_expr_binary(struct ABSYN_BinaryExpr *v);
struct ABSYN_Expr *new_absyn_expr_cond(struct ABSYN_CondExpr *v);
struct ABSYN_Expr *new_absyn_expr_loop(struct ABSYN_LoopExpr *v);
struct ABSYN_Expr *new_absyn_expr_let(bool is_fixed_point,
                                      struct ABSYN_Ident *name,
                                      struct ABSYN_Pattern *lhs_patterns,
                                      struct ABSYN_Expr *rhs_expr);
struct ABSYN_Expr *new_absyn_expr_match(struct ABSYN_Ident *discriminant,
                                        struct ABSYN_Expr *body);
struct ABSYN_Expr *new_absyn_expr_func(struct ABSYN_Expr *lhs,
                                       struct ABSYN_Expr *rhs);

struct ABSYN_UnaryExpr *new_absyn_unary_expr(struct ABSYN_Operator *operation,
                                             struct ABSYN_Expr *monad);
struct ABSYN_BinaryExpr *new_absyn_binary_expr(struct ABSYN_Operator *operation,
                                               struct ABSYN_Expr *left,
                                               struct ABSYN_Expr *right);
struct ABSYN_CondExpr *new_absyn_cond_expr(struct ABSYN_Expr *discriminant,
                                           struct ABSYN_Expr *body);
struct ABSYN_LoopExpr *new_absyn_loop_expr(struct ABSYN_Repr *subject,
                                           struct ABSYN_Repr *object,
                                           struct ABSYN_Expr *body);
struct ABSYN_LetExpr *new_absyn_let_expr(bool is_fixed_point,
                                         struct ABSYN_Ident *name,
                                         struct ABSYN_Pattern *lhs_patterns,
                                         struct ABSYN_Expr *rhs_expr);

struct ABSYN_Adt *new_absyn_adt_product(struct ABSYN_ProductAdt *v_product,
                                        struct ABSYN_Ident *name);
struct ABSYN_Adt *new_absyn_adt_sum(struct ABSYN_SumAdt *v_sum,
                                    struct ABSYN_Ident *name);

struct ABSYN_SumAdt *new_absyn_sum_adt(size_t arity, struct ABSYN_Ident *name,
                                       struct ABSYN_Variant *variants);

struct ABSYN_ProductAdt *new_absyn_product_adt(size_t num_item,
                                               struct ABSYN_ProductItem *items);

struct ABSYN_Pattern *new_absyn_pattern(struct ABSYN_Name *constructor,
                                        struct ABSYN_Expr *discriminators);

struct ABSYN_Module *new_absyn_module(size_t arity, struct ABSYN_Ident *name,
                                      struct ABSYN_Value *decl_values);

struct ABSYN_Value *new_absyn_value(struct ABSYN_Name *name,
                                    struct ABSYN_Signature *signature);

struct ABSYN_Signature *new_absyn_signature(size_t arity,
                                            struct ABSYN_Mapping *mappings,
                                            struct ABSYN_Repr *result);

#endif /* Absyn.h */
