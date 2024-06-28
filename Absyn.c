#include "Absyn.h"

struct ABSYN_Node *new_absyn_node_numeric_integer(intmax_t v) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Numberic;
  node->v_numeric = new_absyn_numeric_integer(v);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_numeric_real(long double v) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Numberic;
  node->v_numeric = new_absyn_numeric_real(v);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_string(struct ABSYN_Repr *v) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_String;
  node->v_string = v;
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_char(struct ABSYN_Repr *v) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Char;
  node->v_char = v;
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_list_normal(size_t num_items,
                                              struct ABSYN_Node *head,
                                              struct ABSYN_Node *items) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_List;
  node->v_list = new_absyn_list_normal(num_items, head, items);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_list_comprehension(size_t num_items,
                                                     struct ABSYN_Node *head,
                                                     struct ABSYN_Node *items) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_List;
  node->v_list = new_absyn_list_comprehension(num_items, head, items);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_tuple_unit(struct ABSYN_Node *v1) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Tuple;
  node->v_tuple = new_absyn_tuple_unit(v1);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_tuple_normal(struct ABSYN_Node *v1,
                                               struct ABSYN_Node *v2) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Tuple;
  node->v_tuple = new_absyn_tuple_normal(v1, v2);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_name_terminal(struct ABSYN_Ident *value) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Name;
  node->v_name = new_absyn_name_terminal(value);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_name_constructor(struct ABSYN_Ident *value) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Name;
  node->v_name = new_absyn_name_constructor(value);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_name_typevar(struct ABSYN_Ident *value) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Name;
  node->v_name = new_absyn_name_typevar(value);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_expr_unary(struct ABSYN_UnaryExpr *v) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Expr;
  node->v_expr = new_absyn_expr_unary(v);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_expr_binary(struct ABSYN_BinaryExpr *v) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Expr;
  node->v_expr = new_absyn_expr_binary(v);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_expr_cond(struct ABSYN_CondExpr *v) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Expr;
  node->v_expr = new_absyn_expr_cond(v);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_expr_loop(struct ABSYN_LoopExpr *v) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Expr;
  node->v_expr = new_absyn_expr_loop(v);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_expr_let(bool is_fixed_point,
                                           struct ABSYN_Ident *name,
                                           struct ABSYN_Pattern *lhs_patterns,
                                           struct ABSYN_Expr *rhs_expr) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Expr;
  node->v_expr =
      new_absyn_expr_let(is_fixed_point, name, lhs_patterns, rhs_expr);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_expr_match(struct ABSYN_Ident *discriminant,
                                             struct ABSYN_Expr *body) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Expr;
  node->v_expr = new_absyn_expr_match(discriminant, body);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_expr_func(struct ABSYN_Expr *lhs,
                                            struct ABSYN_Expr *rhs) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Expr;
  node->v_expr = new_absyn_expr_func(lhs, rhs);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *
new_absyn_node_adt_product(struct ABSYN_ProductAdt *v_product,
                           struct ABSYN_Ident *name) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Adt;
  node->v_adt = new_absyn_adt_product(v_product, name);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_adt_sum(struct ABSYN_SumAdt *v_sum,
                                          struct ABSYN_Ident *name) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Adt;
  node->v_adt = new_absyn_adt_sum(v_sum, name);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_pattern(struct ABSYN_Name *constructor,
                                          struct ABSYN_Expr *discriminators) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Pattern;
  node->v_pattern = new_absyn_pattern(constructor, discriminators);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_module(size_t arity, struct ABSYN_Ident *name,
                                         struct ABSYN_Value *decl_values) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Module;
  node->v_module = new_absyn_module(arity, name, decl_values);
  node->next = NULL;

  return node;
}

struct ABSYN_Node *new_absyn_node_signature(size_t arity,
                                            struct ABSYN_Mapping *mappings,
                                            struct ABSYN_Repr *result) {
  struct ABSYN_Node *node = ABSYN_ALLOC(sizeof(struct ABSYN_Node));
  if (node == NULL) {
    return NULL;
  }

  node->kind = NODE_Signature;
  node->v_value = new_absyn_signature(arity, mappings, result);
  node->next = NULL;

  return node;
}

struct ABSYN_Numeric *new_absyn_numeric_integer(intmax_t v) {
  struct ABSYN_Numeric *numeric = ABSYN_ALLOC(sizeof(struct ABSYN_Numeric));
  if (numeric == NULL) {
    return NULL;
  }

  numeric->kind = NUMERIC_Integer;
  numeric->v_integer = v;

  return numeric;
}

struct ABSYN_Numeric *new_absyn_numeric_real(long double v) {
  struct ABSYN_Numeric *numeric = ABSYN_ALLOC(sizeof(struct ABSYN_Numeric));
  if (numeric == NULL) {
    return NULL;
  }

  numeric->kind = NUMERIC_Real;
  numeric->v_real = v;

  return numeric;
}

struct ABSYN_List *new_absyn_list_normal(size_t num_items,
                                         struct ABSYN_Node *head,
                                         struct ABSYN_Node *items) {
  struct ABSYN_List *list = ABSYN_ALLOC(sizeof(struct ABSYN_List));
  if (list == NULL) {
    return NULL;
  }

  list->kind = LIST_Normal;
  list->num_items = num_items;
  list->head = head;
  list->items = items;

  return list;
}

struct ABSYN_List *new_absyn_list_comprehension(size_t num_items,
                                                struct ABSYN_Node *head,
                                                struct ABSYN_Node *items) {
  struct ABSYN_List *list = ABSYN_ALLOC(sizeof(struct ABSYN_List));
  if (list == NULL) {
    return NULL;
  }

  list->kind = LIST_Comprehension;
  list->num_items = num_items;
  list->head = head;
  list->items = items;

  return list;
}

struct ABSYN_Tuple *new_absyn_tuple_unit(struct ABSYN_Node *v1) {
  struct ABSYN_Tuple *tuple = ABSYN_ALLOC(sizeof(struct ABSYN_Tuple));
  if (tuple == NULL) {
    return NULL;
  }

  tuple->kind = TUPLE_Unit;
  tuple->v1 = v1;
  tuple->v2 = NULL;

  return tuple;
}

struct ABSYN_Tuple *new_absyn_tuple_normal(struct ABSYN_Node *v1,
                                           struct ABSYN_Node *v2) {
  struct ABSYN_Tuple *tuple = ABSYN_ALLOC(sizeof(struct ABSYN_Tuple));
  if (tuple == NULL) {
    return NULL;
  }

  tuple->kind = TUPLE_Normal;
  tuple->v1 = v1;
  tuple->v2 = v2;

  return tuple;
}

struct ABSYN_Name *new_absyn_name_terminal(struct ABSYN_Ident *value) {
  struct ABSYN_Name *name = ABSYN_ALLOC(sizeof(struct ABSYN_Name));
  if (name == NULL) {
    return NULL;
  }

  name->kind = NAME_Terminal;
  name->value = value;
  name->next = NULL;

  return name;
}

struct ABSYN_Name *new_absyn_name_constructor(struct ABSYN_Ident *value) {
  struct ABSYN_Name *name = ABSYN_ALLOC(sizeof(struct ABSYN_Name));
  if (name == NULL) {
    return NULL;
  }

  name->kind = NAME_Constructor;
  name->value = value;
  name->next = NULL;

  return name;
}

struct ABSYN_Name *new_absyn_name_typevar(struct ABSYN_Ident *value) {
  struct ABSYN_Name *name = ABSYN_ALLOC(sizeof(struct ABSYN_Name));
  if (name == NULL) {
    return NULL;
  }

  name->kind = NAME_Typevar;
  name->value = value;
  name->next = NULL;

  return name;
}

struct ABSYN_Expr *new_absyn_expr_unary(struct ABSYN_UnaryExpr *v_unary) {
  struct ABSYN_Expr *expr = ABSYN_ALLOC(sizeof(struct ABSYN_Expr));
  if (expr == NULL) {
    return NULL;
  }

  expr->kind = EXPR_Unary;
  expr->v_unary = v_unary;

  return expr;
}

struct ABSYN_Expr *new_absyn_expr_binary(struct ABSYN_BinaryExpr *v_binary) {
  struct ABSYN_Expr *expr = ABSYN_ALLOC(sizeof(struct ABSYN_Expr));
  if (expr == NULL) {
    return NULL;
  }

  expr->kind = EXPR_Binary;
  expr->v_binary = v_binary;

  return expr;
}

struct ABSYN_Expr *new_absyn_expr_cond(struct ABSYN_CondExpr *v_cond) {
  struct ABSYN_Expr *expr = ABSYN_ALLOC(sizeof(struct ABSYN_Expr));
  if (expr == NULL) {
    return NULL;
  }

  expr->kind = EXPR_Cond;
  expr->v_cond = v_cond;

  return expr;
}

struct ABSYN_Expr *new_absyn_expr_loop(struct ABSYN_LoopExpr *v_loop) {
  struct ABSYN_Expr *expr = ABSYN_ALLOC(sizeof(struct ABSYN_Expr));
  if (expr == NULL) {
    return NULL;
  }

  expr->kind = EXPR_Loop;
  expr->v_loop = v_loop;

  return expr;
}

struct ABSYN_Expr *new_absyn_expr_let(bool is_fixed_point,
                                      struct ABSYN_Ident *name,
                                      struct ABSYN_Pattern *lhs_patterns,
                                      struct ABSYN_Expr *rhs_expr) {
  struct ABSYN_LetExpr *let_expr = ABSYN_ALLOC(sizeof(struct ABSYN_LetExpr));
  if (let_expr == NULL) {
    return NULL;
  }

  let_expr->is_fixed_point = is_fixed_point;
  let_expr->name = name;
  let_expr->lhs_patterns = lhs_patterns;
  let_expr->rhs_expr = rhs_expr;

  struct ABSYN_Expr *expr = ABSYN_ALLOC(sizeof(struct ABSYN_Expr));
  if (expr == NULL) {
    return NULL;
  }

  expr->kind = EXPR_Let;
  expr->v_let = let_expr;

  return expr;
}

struct ABSYN_Expr *new_absyn_expr_match(struct ABSYN_Ident *discriminant,
                                        struct ABSYN_Expr *body) {
  struct ABSYN_MatchExpr *match_expr =
      ABSYN_ALLOC(sizeof(struct ABSYN_MatchExpr));
  if (match_expr == NULL) {
    return NULL;
  }

  match_expr->dscriminant = discriminant;
  match_expr->body = body;

  struct ABSYN_Expr *expr = ABSYN_ALLOC(sizeof(struct ABSYN_Expr));
  if (expr == NULL) {
    return NULL;
  }

  expr->kind = EXPR_Match;
  expr->v_match = match_expr;

  return expr;
}

struct ABSYN_Expr *new_absyn_expr_func(struct ABSYN_Expr *lhs,
                                       struct ABSYN_Expr *rhs) {
  struct ABSYN_FuncExpr *func_expr = ABSYN_ALLOC(sizeof(struct ABSYN_FuncExpr));
  if (func_expr == NULL) {
    return NULL;
  }

  func_expr->lhs = lhs;
  func_expr->rhs = rhs;

  struct ABSYN_Expr *expr = ABSYN_ALLOC(sizeof(struct ABSYN_Expr));
  if (expr == NULL) {
    return NULL;
  }

  expr->kind = EXPR_Func;
  expr->v_func = func_expr;

  return expr;
}

struct ABSYN_Adt *new_absyn_adt_product(struct ABSYN_ProductAdt *v_product,
                                        struct ABSYN_Ident *name) {
  struct ABSYN_Adt *adt = ABSYN_ALLOC(sizeof(struct ABSYN_Adt));
  if (adt == NULL) {
    return NULL;
  }

  adt->kind = ADT_Product;
  adt->v_product = v_product;
  adt->name = name;

  return adt;
}

struct ABSYN_Adt *new_absyn_adt_sum(struct ABSYN_SumAdt *v_sum,
                                    struct ABSYN_Ident *name) {
  struct ABSYN_Adt *adt = ABSYN_ALLOC(sizeof(struct ABSYN_Adt));
  if (adt == NULL) {
    return NULL;
  }

  adt->kind = ADT_Sum;
  adt->v_sum = v_sum;
  adt->name = name;

  return adt;
}

struct ABSYN_SumAdt *new_absyn_sum_adt(size_t arity, struct ABSYN_Ident *name,
                                       struct ABSYN_Variant *variants) {
  struct ABSYN_SumAdt *sum_adt = ABSYN_ALLOC(sizeof(struct ABSYN_SumAdt));
  if (sum_adt == NULL) {
    return NULL;
  }

  sum_adt->arity = arity;
  sum_adt->name = name;
  sum_adt->variants = variants;

  return sum_adt;
}

struct ABSYN_Variant *new_absyn_variant(struct ABSYN_Ident *name,
                                        struct ABSYN_Name *value) {
  struct ABSYN_Variant *variant = ABSYN_ALLOC(sizeof(struct ABSYN_Variant));
  if (variant == NULL) {
    return NULL;
  }

  variant->name = name;
  variant->value = value;
  variant->next = NULL;

  return variant;
}

struct ABSYN_ProductAdt *
new_absyn_product_adt(size_t num_item, struct ABSYN_ProductItem *items) {
  struct ABSYN_ProductAdt *product_adt =
      ABSYN_ALLOC(sizeof(struct ABSYN_ProductAdt));
  if (product_adt == NULL) {
    return NULL;
  }

  product_adt->num_item = num_item;
  product_adt->items = items;

  return product_adt;
}

struct ABSYN_Pattern *new_absyn_pattern(struct ABSYN_Name *constructor,
                                        struct ABSYN_Expr *discriminators) {
  struct ABSYN_Pattern *pattern = ABSYN_ALLOC(sizeof(struct ABSYN_Pattern));
  if (pattern == NULL) {
    return NULL;
  }

  pattern->constructor = constructor;
  pattern->discriminators = discriminators;

  return pattern;
}

struct ABSYN_Module *new_absyn_module(size_t arity, struct ABSYN_Ident *name,
                                      struct ABSYN_Value *decl_values) {
  struct ABSYN_Module *module = ABSYN_ALLOC(sizeof(struct ABSYN_Module));
  if (module == NULL) {
    return NULL;
  }

  module->arity = arity;
  module->name = name;
  module->decl_values = decl_values;
  module->next = NULL;

  return module;
}

struct ABSYN_Mapping *new_absyn_mapping(enum ABSYN_MappingKind kind,
                                        struct ABSYN_Repr *from,
                                        struct ABSYN_Repr *to) {
  struct ABSYN_Mapping *mapping = ABSYN_ALLOC(sizeof(struct ABSYN_Mapping));
  if (mapping == NULL) {
    return NULL;
  }

  mapping->kind = kind;
  mapping->from = from;
  mapping->to = to;

  return mapping;
}

struct ABSYN_Signature *new_absyn_signature(size_t arity,
                                            struct ABSYN_Mapping *mappings,
                                            struct ABSYN_Repr *result) {
  struct ABSYN_Signature *signature =
      ABSYN_ALLOC(sizeof(struct ABSYN_Signature));
  if (signature == NULL) {
    return NULL;
  }

  signature->arity = arity;
  signature->mappings = mappings;
  signature->result = result;

  return signature;
}

struct ABSYN_Value *new_absyn_value(struct ABSYN_Name *name,
                                    struct ABSYN_Signature *signature) {
  struct ABSYN_Value *value = ABSYN_ALLOC(sizeof(struct ABSYN_Value));
  if (value == NULL) {
    return NULL;
  }

  value->name = name;
  value->signature = signature;

  return value;
}
