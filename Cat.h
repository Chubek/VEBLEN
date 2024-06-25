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
struct CAT_Symtab;
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

struct CAT_Symtab {
  uint64_t cat_hash;

  struct CAT_Key *key;
  struct CAT_Type *value;

  struct CAT_Symtab *next;
};

struct CAT_Type {
  struct CAT_Ident *name;
  struct CAT_Symtab **static_link;

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

struct CAT_Tuple {
  struct CAT_Type *ty1;
  struct CAT_Type *ty2;

  bool optional;
};

struct CAT_List {
  struct CAT_Type *ls_type;
  bool optional;
};

struct CAT_Set {
  size_t arity;
  struct CAT_Type *types;
  bool optional;
};

struct CAT_Record {
  size_t arity;
  struct CAT_Item *items;
  bool optional;
};

struct CAT_Sum {
  size_t arity;
  struct CAT_Variant *variants;
};

struct CAT_Variant {
  struct CAT_Ident *name;
  struct CAT_Type *value;

  struct CAT_Variant *next;
};

struct CAT_Functor {
  size_t num_data;
  struct CAT_Type *data;
  void (*transformer_fn)(void *);
};

#endif /* Cat.h */
