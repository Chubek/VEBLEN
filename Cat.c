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



