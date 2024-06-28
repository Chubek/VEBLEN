#include "Token.h"

struct TOKEN_Repr *absyn_repr_create(uint8_t *buffer, size_t buf_length) {
  struct TOKEN_Repr *repr = TOKEN_ALLOC(sizeof(struct TOKEN_Repr));
  if (repr == NULL) {
    return NULL;
  }

  repr->buffer = buffer;
  repr->buf_length = buf_length;
  repr->next = NULL;

  return repr;
}

struct TOKEN_Repr *absyn_repr_concat(struct TOKEN_Repr *repr1,
                                     struct TOKEN_Repr *repr2) {
  size_t total_length = repr1->buf_length + repr2->buf_length;
  uint8_t *new_buffer = (uint8_t *)TOKEN_ALLOC(total_length);
  if (new_buffer == NULL) {
    return NULL;
  }

  memcpy(new_buffer, repr1->buffer, repr1->buf_length);
  memcpy(new_buffer + repr1->buf_length, repr2->buffer, repr2->buf_length);

  struct TOKEN_Repr *concatenated = absyn_repr_create(new_buffer, total_length);
  if (concatenated == NULL) {
    TOKEN_FREE(new_buffer);
    return NULL;
  }

  return concatenated;
}

struct TOKEN_Repr *absyn_repr_append(struct TOKEN_Repr **head,
                                     struct TOKEN_Repr *append) {
  if (*head == NULL) {
    *head = append;
    return *head;
  }

  struct TOKEN_Repr *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = append;
  return *head;
}

void absyn_repr_iter(struct TOKEN_Repr *head,
                     void (*iter_fn)(struct TOKEN_Repr *)) {
  struct TOKEN_Repr *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void absyn_repr_print(struct TOKEN_Repr *repr) {
  printf("%.*s", (int)repr->buf_length, repr->buffer);
}

void absyn_repr_delete(struct TOKEN_Repr *repr) {
  struct TOKEN_Repr *current = repr;
  while (current != NULL) {
    struct TOKEN_Repr *next = current->next;
    TOKEN_FREE(current->buffer);
    TOKEN_FREE(current);
    current = next;
  }
}
