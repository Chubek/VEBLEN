#include "Cat.h"

struct CAT_Repr *cat_repr_create(uint8_t *buffer, size_t buf_length) {
  struct CAT_Repr *repr = ABSYN_ALLOC(sizeof(struct CAT_Repr));
  if (repr == NULL) {
    return NULL;
  }

  repr->buffer = buffer;
  repr->buf_length = buf_length;
  repr->next = NULL;

  return repr;
}

struct CAT_Repr *cat_repr_concat(struct CAT_Repr *repr1,
                                     struct CAT_Repr *repr2) {
  size_t total_length = repr1->buf_length + repr2->buf_length;
  uint8_t *new_buffer = (uint8_t *)ABSYN_ALLOC(total_length);
  if (new_buffer == NULL) {
    return NULL;
  }

  memcpy(new_buffer, repr1->buffer, repr1->buf_length);
  memcpy(new_buffer + repr1->buf_length, repr2->buffer, repr2->buf_length);

  struct CAT_Repr *concatenated = cat_repr_create(new_buffer, total_length);
  if (concatenated == NULL) {
    ABSYN_FREE(new_buffer);
    return NULL;
  }

  return concatenated;
}

struct CAT_Repr *cat_repr_append(struct CAT_Repr **head,
                                     struct CAT_Repr *append) {
  if (*head == NULL) {
    *head = append;
    return *head;
  }

  struct CAT_Repr *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = append;
  return *head;
}

void cat_repr_iter(struct CAT_Repr *head,
  void (*iter_fn)(struct CAT_Repr*)) {
  struct CAT_Repr *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void cat_repr_print(struct CAT_Repr *repr) {
  printf("%.*s", (int)repr->buf_length, repr->buffer);
}

void cat_repr_delete(struct CAT_Repr *repr) {
  struct CAT_Repr *current = repr;
  while (current != NULL) {
    struct CAT_Repr *next = current->next;
    ABSYN_FREE(current->buffer);
    ABSYN_FREE(current);
    current = next;
  }
}

struct CAT_Type *new_cat_type_sum(struct CAT_Sum *v) {
    struct CAT_Type *type = CAT_ALLOC(sizeof(struct CAT_Type));
    if (type == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Type (sum)\n");
        return NULL;
    }
    
    type->name = NULL; 
    type->kind = ADT_Sum;
    type->v_sum = v;
    type->next = NULL;
    
    return type;
}

struct CAT_Type *new_cat_type_product(struct CAT_Product *v) {
    struct CAT_Type *type = CAT_ALLOC(sizeof(struct CAT_Type));
    if (type == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Type (product)\n");
        return NULL;
    }
    
    type->name = NULL; 
    type->kind = ADT_Product;
    type->v_product = v;
    type->next = NULL;
    
    return type;
}

struct CAT_Type *new_cat_type_functor(struct CAT_Functor *v) {
    struct CAT_Type *type = CAT_ALLOC(sizeof(struct CAT_Type));
    if (type == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Type (functor)\n");
        return NULL;
    }
    
    type->name = NULL; 
    type->kind = ADT_Functor;
    type->v_functor = v;
    type->next = NULL;
    
    return type;
}

struct CAT_Type *new_cat_type_variable(struct CAT_Variable *v) {
    struct CAT_Type *type = CAT_ALLOC(sizeof(struct CAT_Type));
    if (type == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Type (variable)\n");
        return NULL;
    }
    
    type->name = NULL; 
    type->kind = ADT_Variable;
    type->v_variable = v;
    type->next = NULL;
    
    return type;
}

struct CAT_Type *cat_type_append(struct CAT_Type **head,
                                     struct CAT_Type *append) {
  if (*head == NULL) {
    *head = append;
    return *head;
  }

  struct CAT_Type *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = append;
  return *head;
}

void cat_type_iter(struct CAT_Type *head,
                     void (*iter_fn)(struct CAT_Type *)) {
  struct CAT_Type *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void cat_type_delete(struct CAT_Type *v) {
  struct CAT_Type *current = v;
  while (current != NULL) {
    struct CAT_Type *next = current->next;
    CAT_FREE(current);
    current = next;
  }
}

struct CAT_Product *cat_product_new_tuple(struct CAT_Tuple *v) {
    struct CAT_Product *product = CAT_ALLOC(sizeof(struct CAT_Product));
    if (product == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Product (tuple)\n");
        return NULL;
    }
    
    product->kind = PROD_Tuple;
    product->v_tuple = v;
    
    return product;
}

struct CAT_Product *cat_product_new_list(struct CAT_List *v) {
    struct CAT_Product *product = CAT_ALLOC(sizeof(struct CAT_Product));
    if (product == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Product (list)\n");
        return NULL;
    }
    
    product->kind = PROD_List;
    product->v_list = v;
    
    return product;
}

struct CAT_Product *cat_product_new_set(struct CAT_Set *v) {
    struct CAT_Product *product = CAT_ALLOC(sizeof(struct CAT_Product));
    if (product == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Product (set)\n");
        return NULL;
    }
    
    product->kind = PROD_Set;
    product->v_set = v;
    
    return product;
}

struct CAT_Product *cat_product_new_record(struct CAT_Record *v) {
    struct CAT_Product *product = CAT_ALLOC(sizeof(struct CAT_Product));
    if (product == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Product (record)\n");
        return NULL;
    }
    
    product->kind = PROD_Record;
    product->v_record = v;
    
    return product;
}

struct CAT_Tuple *cat_tuple_new(struct CAT_Type *ty1, struct CAT_Type *ty2, bool optional) {
    struct CAT_Tuple *tuple = CAT_ALLOC(sizeof(struct CAT_Tuple));
    if (tuple == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Tuple\n");
        return NULL;
    }
    
    tuple->ty1 = ty1;
    tuple->ty2 = ty2;
    tuple->optional = optional;
    
    return tuple;
}

struct CAT_List *cat_list_new(struct CAT_Type *ls_type, bool optional) {
    struct CAT_List *list = CAT_ALLOC(sizeof(struct CAT_List));
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_List\n");
        return NULL;
    }
    
    list->ls_type = ls_type;
    list->optional = optional;
    
    return list;
}

struct CAT_Set *cat_set_new(size_t arity, struct CAT_Type *types, bool optional) {
    struct CAT_Set *set = CAT_ALLOC(sizeof(struct CAT_Set));
    if (set == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Set\n");
        return NULL;
    }
    
    set->arity = arity;
    set->types = types;
    set->optional = optional;
    
    return set;
}

struct CAT_Record *cat_record_new(size_t arity, struct CAT_Item *items, bool optional) {
    struct CAT_Record *record = CAT_ALLOC(sizeof(struct CAT_Record));
    if (record == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Record\n");
        return NULL;
    }
    
    record->arity = arity;
    record->items = items;
    record->optional = optional;
    
    return record;
}

struct CAT_Sum *cat_sum_new(size_t arity, struct CAT_Variant *variants) {
    struct CAT_Sum *sum = CAT_ALLOC(sizeof(struct CAT_Sum));
    if (sum == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Sum\n");
        return NULL;
    }
    
    sum->arity = arity;
    sum->variants = variants;
    
    return sum;
}

struct CAT_Variant *cat_variant_new(struct CAT_Ident *name, struct CAT_Type *value) {
    struct CAT_Variant *variant = CAT_ALLOC(sizeof(struct CAT_Variant));
    if (variant == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Variant\n");
        return NULL;
    }
    
    variant->name = name;
    variant->value = value;
    variant->next = NULL;
    
    return variant;
}

struct CAT_Variant *cat_variant_append(struct CAT_Variant **head, struct CAT_Variant *append) {
    if (*head == NULL) {
        *head = append;
    } else {
        struct CAT_Variant *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = append;
    }
    
    return *head;
}

struct CAT_Functor *cat_functor_new(size_t num_data, struct CAT_Type *data, void (*transformer_fn)(void *)) {
    struct CAT_Functor *functor = CAT_ALLOC(sizeof(struct CAT_Functor));
    if (functor == NULL) {
        fprintf(stderr, "Memory allocation failed for CAT_Functor\n");
        return NULL;
    }
    
    functor->num_data = num_data;
    functor->data = data;
    functor->transformer_fn = transformer_fn;
    
    return functor;
}


