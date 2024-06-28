#include "Rich.h"

struct RICH_Repr *rich_repr_create(uint8_t *buffer, size_t buf_length) {
  struct RICH_Repr *repr = ABSYN_ALLOC(sizeof(struct RICH_Repr));
  if (repr == NULL) {
    return NULL;
  }

  repr->buffer = buffer;
  repr->buf_length = buf_length;
  repr->next = NULL;

  return repr;
}

struct RICH_Repr *rich_repr_concat(struct RICH_Repr *repr1,
                                   struct RICH_Repr *repr2) {
  size_t total_length = repr1->buf_length + repr2->buf_length;
  uint8_t *new_buffer = (uint8_t *)ABSYN_ALLOC(total_length);
  if (new_buffer == NULL) {
    return NULL;
  }

  memcpy(new_buffer, repr1->buffer, repr1->buf_length);
  memcpy(new_buffer + repr1->buf_length, repr2->buffer, repr2->buf_length);

  struct RICH_Repr *concatenated = rich_repr_create(new_buffer, total_length);
  if (concatenated == NULL) {
    ABSYN_FREE(new_buffer);
    return NULL;
  }

  return concatenated;
}

struct RICH_Repr *rich_repr_append(struct RICH_Repr **head,
                                   struct RICH_Repr *append) {
  if (*head == NULL) {
    *head = append;
    return *head;
  }

  struct RICH_Repr *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = append;
  return *head;
}

void rich_repr_iter(struct RICH_Repr *head,
                    void (*iter_fn)(struct RICH_Repr *)) {
  struct RICH_Repr *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void rich_repr_print(struct RICH_Repr *repr) {
  printf("%.*s", (int)repr->buf_length, repr->buffer);
}

void rich_repr_delete(struct RICH_Repr *repr) {
  struct RICH_Repr *current = repr;
  while (current != NULL) {
    struct RICH_Repr *next = current->next;
    ABSYN_FREE(current->buffer);
    ABSYN_FREE(current);
    current = next;
  }
}

struct RICH_Expr *new_rich_expr_constant(struct RICH_Const *v) {
  struct RICH_Expr *expr =
      (struct RICH_Expr *)RICH_ALLOC(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Constant;
    expr->v_const = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *new_rich_expr_variable(struct RICH_Variable *v) {
  struct RICH_Expr *expr = (struct RICH_Expr *)malloc(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Variable;
    expr->v_variable = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *new_rich_expr_application(struct RICH_Application *v) {
  struct RICH_Expr *expr = (struct RICH_Expr *)malloc(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Application;
    expr->v_application = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *new_rich_expr_abstraction(struct RICH_Abstraction *v) {
  struct RICH_Expr *expr = (struct RICH_Expr *)malloc(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Abstraction;
    expr->v_abstraction = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *new_rich_expr_let(struct RICH_Let *v) {
  struct RICH_Expr *expr = (struct RICH_Expr *)malloc(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Let;
    expr->v_let = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *new_rich_expr_letrec(struct RICH_Letrec *v) {
  struct RICH_Expr *expr = (struct RICH_Expr *)malloc(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Letrec;
    expr->v_letrec = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *new_rich_expr_infix(struct RICH_Infix *v) {
  struct RICH_Expr *expr = (struct RICH_Expr *)malloc(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Infix;
    expr->v_infix = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *new_rich_expr_case(struct RICH_Case *v) {
  struct RICH_Expr *expr = (struct RICH_Expr *)malloc(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Case;
    expr->v_case = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *new_rich_expr_pattern(struct RICH_Pattern *v) {
  struct RICH_Expr *expr = (struct RICH_Expr *)malloc(sizeof(struct RICH_Expr));
  if (expr != NULL) {
    expr->kind = RICHXP_Pattern;
    expr->v_pattern = v;
    expr->next = NULL;
  }
  return expr;
}

struct RICH_Expr *rich_expr_append(struct RICH_Expr **head,
                                   struct RICH_Expr *append) {
  if (*head == NULL) {
    *head = append;
  } else {
    struct RICH_Expr *temp = *head;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = append;
  }
  return *head;
}

void rich_expr_iter(struct RICH_Expr *head,
                    void (*iter_fn)(struct RICH_Expr *)) {
  struct RICH_Expr *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void rich_expr_delete(struct RICH_Expr *v) {
  struct RICH_Expr *current = v;
  while (current != NULL) {
    struct RICH_Expr *next = current->next;
    RICH_FREE(current);
    current = next;
  }
}

struct RICH_Const *new_rich_const_number(struct RICH_Repr *repr) {
  struct RICH_Const *c =
      (struct RICH_Const *)RICH_ALLOC(sizeof(struct RICH_Const));
  if (c != NULL) {
    c->kind = CONST_Number;
    c->repr = repr;
  }
  return c;
}

struct RICH_Variable *new_rich_variable_function(struct RICH_Repr *repr) {
  struct RICH_Variable *v =
      (struct RICH_Variable *)RICH_ALLOC(sizeof(struct RICH_Variable));
  if (v != NULL) {
    v->kind = VAR_Function;
    v->repr = repr;
  }
  return v;
}

struct RICH_Application *new_rich_application(struct RICH_Expr *subj,
                                              struct RICH_Expr *obj) {
  struct RICH_Application *app =
      (struct RICH_Application *)RICH_ALLOC(sizeof(struct RICH_Application));
  if (app != NULL) {
    app->subj = subj;
    app->obj = obj;
  }
  return app;
}

struct RICH_Abstraction *new_rich_abstraction(struct RICH_Pattern *patt,
                                              struct RICH_Expr *abs) {
  struct RICH_Abstraction *abs =
      (struct RICH_Abstraction *)RICH_ALLOC(sizeof(struct RICH_Abstraction));
  if (abs != NULL) {
    abs->patt = patt;
    abs->abs = abs;
  }
  return abs;
}

struct RICH_Let *new_rich_let(struct RICH_Pattern *rhs, struct RICH_Expr *lhs,
                              struct RICH_Expr *in) {
  struct RICH_Let *let = (struct RICH_Let *)RICH_ALLOC(sizeof(struct RICH_Let));
  if (let != NULL) {
    let->rhs = rhs;
    let->lhs = lhs;
    let->in = in;
    let->next = NULL;
  }
  return let;
}

struct RICH_Let *rich_let_append(struct RICH_Let **head,
                                 struct RICH_Let *append) {
  if (*head == NULL) {
    *head = append;
  } else {
    struct RICH_Let *temp = *head;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = append;
  }
  return *head;
}

void rich_let_iter(struct RICH_Let *head, void (*iter_fn)(struct RICH_Let *)) {
  struct RICH_Let *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void rich_let_delete(struct RICH_Let *v) {
  struct RICH_Let *current = v;
  while (current != NULL) {
    struct RICH_Let *next = current->next;
    RICH_FREE(current);
    current = next;
  }
}

struct RICH_Letrec *new_rich_letrch(struct RICH_Let *lets, size_t num_lets,
                                    struct RICH_Let *in) {
  struct RICH_Letrec *letrec =
      (struct RICH_Letrec *)RICH_ALLOC(sizeof(struct RICH_Letrec));
  if (letrec != NULL) {
    letrec->lets = lets;
    letrec->num_lets = num_lets;
    letrec->in = in;
  }
  return letrec;
}

struct RICH_Infix *new_rich_infix(struct RICH_Expr *left,
                                  struct RICH_Expr *right,
                                  struct RICH_Repr *fatbar) {
  struct RICH_Infix *infix =
      (struct RICH_Infix *)RICH_ALLOC(sizeof(struct RICH_Infix));
  if (infix != NULL) {
    infix->left = left;
    infix->right = right;
    infix->fatbar = fatbar;
  }
  return infix;
}

struct RICH_Case *new_rich_case(struct RICH_Variable *var,
                                struct RICH_Pattern *patt, size_t num_patts) {
  struct RICH_Case *c =
      (struct RICH_Case *)RICH_ALLOC(sizeof(struct RICH_Case));
  if (c != NULL) {
    c->discrim = var;
    c->patts = patt;
    c->num_patts = num_patts;
  }
  return c;
}

struct RICH_Pattern *new_rich_pattern(enum PATT_Type tyy,
                                      struct RICH_Expr *expr1,
                                      struct RICH_Expr *expr2) {
  struct RICH_Pattern *p =
      (struct RICH_Pattern *)RICH_ALLOC(sizeof(struct RICH_Pattern));
  if (p != NULL) {
    p->kind = tyy;
    p->expr1 = expr1;
    p->expr2 = expr2;
    p->next = NULL;
  }
  return p;
}

struct RICH_Expr *rich_pattern_append(struct RICH_Expr **head,
                                      struct RICH_Expr *append) {
  if (*head == NULL) {
    *head = append;
  } else {
    struct RICH_Expr *temp = *head;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = append;
  }
  return *head;
}

void rich_pattern_iter(struct RICH_Pattern *head,
                       void (*iter_fn)(struct RICH_Pattern *)) {
  struct RICH_Pattern *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void rich_pattern_delete(struct RICH_Pattern *v) {
  struct RICH_Pattern *current = v;
  while (current != NULL) {
    struct RICH_Pattern *next = current->next;
    RICH_FREE(current);
    current = next;
  }
}
