#ifndef CAT_H
#define CAT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Veblen.h"

/* The following must be preprocessed with AllocPPx.pl */

/*(alloc-pp __cat_heap)!*/

#define CAT_ALLOC(n) __cat_heap_alloc_notrace(n)
#define CAT_REALLOC(mem, nn) __cat_heap_realloc(mem, nn)
#define CAT_FREE(mem) __cat_heap_free(mem)

#define CAT_Ident CAT_Repr
#define CAT_Variable CAT_Repr
#define CAT_Key CAT_Repr
#define CAT_Item CAT_Variant

struct CAT_Repr;
struct CAT_Type;
struct CAT_Product;
struct CAT_Tuple;
struct CAT_List;
struct CAT_Record;
struct CAT_Sum;
struct CAT_Variant;
struct CAT_Functor;

struct CAT_Repr {
  uint8_t *buffer;
  size_t buf_length;

  struct CAT_Repr *next;
};

struct CAT_Repr *cat_repr_create(uint8_t *buffer, size_t buf_length);
struct CAT_Repr *cat_repr_concat(struct CAT_Repr *repr1,
                                 struct CAT_Repr *repr2);
struct TOKNE_Repr *cat_repr_append(struct CAT_Repr **head,
                                   struct CAT_Repr *append);
void cat_repr_iter(struct CAT_Repr *head, void (*iter_fn)(struct CAT_Repr *));
void cat_repr_print(struct CAT_Repr *repr);
void cat_repr_delete(struct CAT_Repr *repr);

struct CAT_Type {
  struct CAT_Ident *name;

  enum {
    ADT_Sum,
    ADT_Product,
    ADT_Functor,
    ADT_Variable,
  } kind;

  union {
    struct CAT_Sum *v_sum;
    struct CAT_Product *v_product;
    struct CAT_Functor *v_functor;
    struct CAT_Variable *v_variable;
  };

  struct CAT_Type *next;
};

struct CAT_Type *new_cat_type_sum(struct CAT_Sum *v);
struct CAT_Type *new_cat_type_product(struct CAT_Product *v);
struct CAT_Type *new_cat_Type_functor(struct CAT_Functor *v);
struct CAT_Type *new_cat_type_variable(struct CAT_Variable *v);

struct CAT_Type *cat_type_append(struct CAT_Type **head,
                                 struct CAT_Type *append);
void cat_type_iter(struct CAT_Type *head, void (*iter_fn)(struct CAT_Type *));
void cat_type_delete(struct CAT_Type *v);

struct CAT_Product {
  enum {
    PROD_Tuple,
    PROD_List,
    PROD_Set,
    PROD_Record,
  } kind;

  union {
    struct CAT_Tuple *v_tuple;
    struct CAT_List *v_list;
    struct CAT_Set *v_set;
    struct CAT_Record *v_record;
  };
};

struct CAT_Product *cat_product_new_tuple(struct CAT_Tuple *v);
struct CAT_Product *cat_product_new_list(struct CAT_List *v);
struct CAT_Product *cat_product_new_set(struct CAT_Set *v);
struct CAT_Product *cat_product_new_record(struct CAT_Record *v);

struct CAT_Tuple {
  struct CAT_Type *ty1;
  struct CAT_Type *ty2;

  bool optional;
};

struct CAT_Tuple *cat_tuple_new(struct CAT_Type *ty1, struct CAT_Type *ty2,
                                bool optional);

struct CAT_List {
  struct CAT_Type *ls_type;
  bool optional;
};

struct CAT_List *cat_list_new(struct CAT_Type *ls_type, bool optional);

struct CAT_Set {
  size_t arity;
  struct CAT_Type *types;
  bool optional;
};

struct CAT_Set *cat_set_new(size_t arity, struct CAT_Type *types,
                            bool optional);

struct CAT_Record {
  size_t arity;
  struct CAT_Item *items;
  bool optional;
};

struct CAT_Record *cat_record_new(size_t arity, struct CAT_Item *items,
                                  bool optional);

struct CAT_Sum {
  size_t arity;
  struct CAT_Variant *variants;
};

struct CAT_Sum *cat_sum_new(size_t arity, struct CAT_Variant *variants);

struct CAT_Variant {
  struct CAT_Ident *name;
  struct CAT_Type *value;

  struct CAT_Variant *next;
};

struct CAT_Variant *cat_variant_new(struct CAT_Ident *name,
                                    struct CAT_Type *value);
struct CAT_Variant *cat_variant_append(struct CAT_Variant **head,
                                       struct CAT_Variant *append);

struct CAT_Functor {
  size_t num_data;
  struct CAT_Type *data;
  void (*transformer_fn)(void *);
};

struct CAT_Functor *cat_functor_new(size_t num_data, struct CAT_Type *data,
                                    void (*transformer_fn)(void *));

#endif /* Cat.h */
