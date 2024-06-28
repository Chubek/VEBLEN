#include "Ord.h"

struct ORD_Repr *ord_repr_create(uint8_t *buffer, size_t buf_length) {
  struct ORD_Repr *repr = ORD_ALLOC(sizeof(struct ORD_Repr));
  if (repr == NULL) {
    return NULL;
  }

  repr->buffer = buffer;
  repr->buf_length = buf_length;
  repr->next = NULL;

  return repr;
}

struct ORD_Repr *ord_repr_concat(struct ORD_Repr *repr1,
                                 struct ORD_Repr *repr2) {
  size_t total_length = repr1->buf_length + repr2->buf_length;
  uint8_t *new_buffer = (uint8_t *)ORD_ALLOC(total_length);
  if (new_buffer == NULL) {
    return NULL;
  }

  memcpy(new_buffer, repr1->buffer, repr1->buf_length);
  memcpy(new_buffer + repr1->buf_length, repr2->buffer, repr2->buf_length);

  struct ORD_Repr *concatenated = ord_repr_create(new_buffer, total_length);
  if (concatenated == NULL) {
    ORD_FREE(new_buffer);
    return NULL;
  }

  return concatenated;
}

struct ORD_Repr *ord_repr_append(struct ORD_Repr **head,
                                 struct ORD_Repr *append) {
  if (*head == NULL) {
    *head = append;
    return *head;
  }

  struct ORD_Repr *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = append;
  return *head;
}

void ord_repr_iter(struct ORD_Repr *head, void (*iter_fn)(struct ORD_Repr *)) {
  struct ORD_Repr *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void ord_repr_print(struct ORD_Repr *repr) {
  printf("%.*s", (int)repr->buf_length, repr->buffer);
}

void ord_repr_delete(struct ORD_Repr *repr) {
  struct ORD_Repr *current = repr;
  while (current != NULL) {
    struct ORD_Repr *next = current->next;
    ORD_FREE(current->buffer);
    ORD_FREE(current);
    current = next;
  }
}

struct ORD_Expr *new_ord_expr_atom(struct ORD_Atom *v) {
  struct ORD_Expr *expr = (struct ORD_Expr *)ORD_ALLOC(sizeof(struct ORD_Expr));
  if (!expr) {
    fprintf(stderr, "Memory allocation failed for new ORD_Expr (Atom)\n");
    return NULL;
  }

  expr->name = NULL;
  expr->kind = ORDXP_Atom;
  expr->v_atom = v;
  expr->is_normal = true;
  expr->next = NULL;

  return expr;
}

struct ORD_Expr *new_ord_expr_composite(struct ORD_Composite *v) {
  struct ORD_Expr *expr = (struct ORD_Expr *)ORD_ALLOC(sizeof(struct ORD_Expr));
  if (!expr) {
    fprintf(stderr, "Memory allocation failed for new ORD_Expr (Composite)\n");
    return NULL;
  }

  expr->name = NULL;
  expr->kind = ORDXP_Composite;
  expr->v_composite = v;
  expr->is_normal = true;
  expr->next = NULL;

  return expr;
}

struct ORD_Expr *new_ord_expr_abstraction(struct ORD_Abstraction *v) {
  struct ORD_Expr *expr = (struct ORD_Expr *)ORD_ALLOC(sizeof(struct ORD_Expr));
  if (!expr) {
    fprintf(stderr,
            "Memory allocation failed for new ORD_Expr (Abstraction)\n");
    return NULL;
  }

  expr->name = NULL;
  expr->kind = ORDXP_Abstraction;
  expr->v_abstraction = v;
  expr->is_normal = true;
  expr->next = NULL;

  return expr;
}

struct ORD_Expr *new_ord_expr_application(struct ORD_Application *v) {
  struct ORD_Expr *expr = (struct ORD_Expr *)ORD_ALLOC(sizeof(struct ORD_Expr));
  if (!expr) {
    fprintf(stderr,
            "Memory allocation failed for new ORD_Expr (Application)\n");
    return NULL;
  }

  expr->name = NULL;
  expr->kind = ORDXP_Application;
  expr->v_application = v;
  expr->is_normal = true;
  expr->next = NULL;

  return expr;
}

struct ORD_Expr *ord_expr_append(struct ORD_Expr **head,
                                 struct ORD_Expr *append) {
  if (*head == NULL) {
    *head = append;
  } else {
    struct ORD_Expr *current = *head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = append;
  }
  return *head;
}

void ord_expr_iter(struct ORD_Expr *head, void (*iter_fn)(struct ORD_Expr *)) {
  struct ORD_Expr *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void ord_expr_delete(struct ORD_Expr *v) {
  struct ORD_Expr *current = v;
  while (current != NULL) {
    struct ORD_Expr *next = current->next;
    ORD_FREE(current);
    current = next;
  }
}

struct ORD_Atom *ord_atom_new_number(struct ORD_Repr *v) {
  struct ORD_Atom *atom = (struct ORD_Atom *)ORD_ALLOC(sizeof(struct ORD_Atom));
  if (!atom) {
    fprintf(stderr, "Memory allocation failed for new ORD_Atom (Number)\n");
    return NULL;
  }

  atom->kind = ATOM_Number;
  atom->is_var = false;
  atom->is_free = false;
  atom->repr = v;
  atom->next = NULL;

  return atom;
}

struct ORD_Atom *ord_atom_new_char(struct ORD_Repr *v) {
  struct ORD_Atom *atom = (struct ORD_Atom *)ORD_ALLOC(sizeof(struct ORD_Atom));
  if (!atom) {
    fprintf(stderr, "Memory allocation failed for new ORD_Atom (Char)\n");
    return NULL;
  }

  atom->kind = ATOM_Char;
  atom->is_var = false;
  atom->is_free = false;
  atom->repr = v;
  atom->next = NULL;

  return atom;
}

struct ORD_Atom *ord_atom_new_string(struct ORD_Repr *v) {
  struct ORD_Atom *atom = (struct ORD_Atom *)ORD_ALLOC(sizeof(struct ORD_Atom));
  if (!atom) {
    fprintf(stderr, "Memory allocation failed for new ORD_Atom (String)\n");
    return NULL;
  }

  atom->kind = ATOM_String;
  atom->is_var = false;
  atom->is_free = false;
  atom->repr = v;
  atom->next = NULL;

  return atom;
}

struct ORD_Atom *ord_atom_new_ident(struct ORD_Repr *v) {
  struct ORD_Atom *atom = (struct ORD_Atom *)ORD_ALLOC(sizeof(struct ORD_Atom));
  if (!atom) {
    fprintf(stderr, "Memory allocation failed for new ORD_Atom (Ident)\n");
    return NULL;
  }

  atom->kind = ATOM_Ident;
  atom->is_var = false;
  atom->is_free = false;
  atom->repr = v;
  atom->next = NULL;

  return atom;
}

struct ORD_Atom *ord_atom_append(struct ORD_Atom **head,
                                 struct ORD_Atom *append) {
  if (*head == NULL) {
    *head = append;
  } else {
    struct ORD_Atom *current = *head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = append;
  }
  return *head;
}

void ord_atom_iter(struct ORD_Atom *head, void (*iter_fn)(struct ORD_Atom *)) {
  struct ORD_Atom *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void ord_atom_delete(struct ORD_Atom *v) {
  struct ORD_Atom *current = v;
  while (current != NULL) {
    struct ORD_Atom *next = current->next;
    ORD_FREE(current);
    current = next;
  }
}

struct ORD_Composite *ord_composite_new(size_t arity, bool is_body,
                                        struct ORD_Item *items) {
  struct ORD_Composite *composite =
      (struct ORD_Composite *)ORD_ALLOC(sizeof(struct ORD_Composite));
  if (!composite) {
    fprintf(stderr, "Memory allocation failed for new ORD_Composite\n");
    return NULL;
  }

  composite->arity = arity;
  composite->is_body = is_body;
  composite->items = items;

  return composite;
}

struct ORD_Application *ord_application_new(struct ORD_Atom *arguments,
                                            struct ORD_Expr *body) {
  struct ORD_Application *application =
      (struct ORD_Application *)ORD_ALLOC(sizeof(struct ORD_Application));
  if (!application) {
    fprintf(stderr, "Memory allocation failed for new ORD_Application\n");
    return NULL;
  }

  application->arguments = arguments;
  application->body = body;

  return application;
}

struct ORD_Abstraction *ord_abstraction_new(enum ABS_Type kind,
                                            enum REDEX_Type state,
                                            struct ORD_Expr *subject,
                                            struct ORD_Expr *object) {
  struct ORD_Abstraction *abstraction =
      (struct ORD_Abstraction *)ORD_ALLOC(sizeof(struct ORD_Abstraction));
  if (!abstraction) {
    fprintf(stderr, "Memory allocation failed for new ORD_Abstraction\n");
    return NULL;
  }

  abstraction->kind = kind;
  abstraction->state = state;
  abstraction->subject = subject;
  abstraction->object = object;

  return abstraction;
}
