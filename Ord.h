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

struct ORD_Repr *ord_repr_create(uint8_t *buffer, size_t buf_length);
struct ORD_Repr *ord_repr_concat(struct ORD_Repr *repr1,
                                 struct ORD_Repr *repr2);
struct TOKNE_Repr *ord_repr_append(struct ORD_Repr **head,
                                   struct ORD_Repr *append);
void ord_repr_iter(struct ORD_Repr *head, void (*iter_fn)(struct ORD_Repr *));
void ord_repr_print(struct ORD_Repr *repr);
void ord_repr_delete(struct ORD_Repr *repr);

struct ORD_Expr {
  struct ORD_Ident *name;

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

struct ORD_Expr *new_ord_expr_atom(struct ORD_Atom *v);
struct ORD_Expr *new_ord_expr_composite(struct ORD_Composite *v);
struct ORD_Expr *new_ord_expr_abstrction(struct ORD_Abstrction *v);
struct ORD_Expr *new_ord_expr_application(struct ORD_Application *v);

struct ORD_Expr *ord_expr_append(struct ORD_Expr **head,
                                 struct ORD_Expr *append);
void ord_expr_iter(struct ORD_Expr *head, void (*iter_fn)(struct ORD_Expr *));
void ord_expr_delete(struct ORD_Expr *v);

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

struct ORD_Atom *ord_atom_new_number(struct ORD_Repr *v);
struct ORD_Atom *ord_atom_new_char(struct ORD_Repr *v);
struct ORD_Atom *ord_atom_new_string(struct ORD_Repr *v);
struct ORD_Atom *ord_atom_new_ident(struct ORD_Repr *v);

struct ORD_Atom *ord_atom_append(struct ORD_Atom **head,
                                 struct ORD_Atom *append);
void ord_atom_iter(struct ORD_Atom *head, void (*iter_fn)(struct ORD_Atom *));
void ord_atom_delete(struct ORD_Atom *v);

struct ORD_Composite {
  size_t arity;
  bool is_body;
  struct ORD_Item *items;
};

struct ORD_Composite *ord_composite_new(size_t arity, bool is_body,
                                        struct ORD_Item *items);

struct ORD_Application {
  struct ORD_Atom *arguments;
  struct ORD_Expr *body;
};

struct ORD_Application *ord_application_new(struct ORD_Atom *arguments,
                                            struct ORD_Expr *body);

struct ORD_Abstraction {
  enum ABS_Type {
    ABS_Alpha,
    ABS_Beta,
    ABS_Eta,
  } kind;

  enum REDEX_Type {
    REDEX_Normal,
    REDEX_Reduction,
    REDEX_Conversion,
  } state;

  struct ORD_Expr *subject;
  struct ORD_Expr *object;
};

struct ORD_Abstraction *ord_abstraction_new(enum ABS_Type kind,
                                            enum REDEX_Type state,
                                            struct ORD_Expr *subject,
                                            struct ORD_Expr *object);

#endif /* Ord.h */
