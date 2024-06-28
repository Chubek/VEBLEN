#include "Token.h"

struct TOKEN_Repr *token_repr_create(uint8_t *buffer, size_t buf_length) {
  struct TOKEN_Repr *repr = TOKEN_ALLOC(sizeof(struct TOKEN_Repr));
  if (repr == NULL) {
    return NULL;
  }

  repr->buffer = buffer;
  repr->buf_length = buf_length;
  repr->next = NULL;

  return repr;
}

struct TOKEN_Repr *token_repr_concat(struct TOKEN_Repr *repr1,
                                     struct TOKEN_Repr *repr2) {
  size_t total_length = repr1->buf_length + repr2->buf_length;
  uint8_t *new_buffer = (uint8_t *)TOKEN_ALLOC(total_length);
  if (new_buffer == NULL) {
    return NULL;
  }

  memcpy(new_buffer, repr1->buffer, repr1->buf_length);
  memcpy(new_buffer + repr1->buf_length, repr2->buffer, repr2->buf_length);

  struct TOKEN_Repr *concatenated = token_repr_create(new_buffer, total_length);
  if (concatenated == NULL) {
    TOKEN_FREE(new_buffer);
    return NULL;
  }

  return concatenated;
}

struct TOKEN_Repr *token_repr_append(struct TOKEN_Repr **head,
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

void token_repr_iter(struct TOKEN_Repr *head,
                     void (*iter_fn)(struct TOKEN_Repr *)) {
  struct TOKEN_Repr *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void token_repr_print(struct TOKEN_Repr *repr) {
  printf("%.*s", (int)repr->buf_length, repr->buffer);
}

void token_repr_delete(struct TOKEN_Repr *repr) {
  struct TOKEN_Repr *current = repr;
  while (current != NULL) {
    struct TOKEN_Repr *next = current->next;
    TOKEN_FREE(current->buffer);
    TOKEN_FREE(current);
    current = next;
  }
}

struct TOKEN_Meta *token_meta_new_dec_integer(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Integer;
  meta->v_integer =
      (struct TOKEN_Integer *)TOKEN_ALLOC(sizeof(struct TOKEN_Integer));
  if (meta->v_integer == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_integer->kind = INT_Decimal;
  meta->v_integer->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_hex_integer(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Integer;
  meta->v_integer =
      (struct TOKEN_Integer *)TOKEN_ALLOC(sizeof(struct TOKEN_Integer));
  if (meta->v_integer == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_integer->kind = INT_Hexadecimal;
  meta->v_integer->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_oct_integer(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Integer;
  meta->v_integer =
      (struct TOKEN_Integer *)TOKEN_ALLOC(sizeof(struct TOKEN_Integer));
  if (meta->v_integer == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_integer->kind = INT_Octal;
  meta->v_integer->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_bin_integer(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Integer;
  meta->v_integer =
      (struct TOKEN_Integer *)TOKEN_ALLOC(sizeof(struct TOKEN_Integer));
  if (meta->v_integer == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_integer->kind = INT_Binary;
  meta->v_integer->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_float_real(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Real;
  meta->v_real = (struct TOKEN_Real *)TOKEN_ALLOC(sizeof(struct TOKEN_Real));
  if (meta->v_real == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_real->kind = REAL_Float;
  meta->v_real->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_rational_real(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Real;
  meta->v_real = (struct TOKEN_Real *)TOKEN_ALLOC(sizeof(struct TOKEN_Real));
  if (meta->v_real == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_real->kind = REAL_Rational;
  meta->v_real->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_char_text(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Text;
  meta->v_text = (struct TOKEN_Text *)TOKEN_ALLOC(sizeof(struct TOKEN_Text));
  if (meta->v_text == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_text->kind = TEXT_Char;
  meta->v_text->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_string_text(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Text;
  meta->v_text = (struct TOKEN_Text *)TOKEN_ALLOC(sizeof(struct TOKEN_Text));
  if (meta->v_text == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_text->kind = TEXT_String;
  meta->v_text->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_utf8_string_text(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Text;
  meta->v_text = (struct TOKEN_Text *)TOKEN_ALLOC(sizeof(struct TOKEN_Text));
  if (meta->v_text == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_text->kind = TEXT_Utf8String;
  meta->v_text->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_apostrofixed_ident(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Ident;
  meta->v_ident = (struct TOKEN_Ident *)TOKEN_ALLOC(sizeof(struct TOKEN_Ident));
  if (meta->v_ident == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_ident->kind = IDENT_ApostroFixed;
  meta->v_ident->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_pascalcase_ident(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Ident;
  meta->v_ident = (struct TOKEN_Ident *)TOKEN_ALLOC(sizeof(struct TOKEN_Ident));
  if (meta->v_ident == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_ident->kind = IDENT_PascalCase;
  meta->v_ident->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_traincase_ident(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Ident;
  meta->v_ident = (struct TOKEN_Ident *)TOKEN_ALLOC(sizeof(struct TOKEN_Ident));
  if (meta->v_ident == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_ident->kind = IDENT_TrainCase;
  meta->v_ident->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_keyword_ident(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Ident;
  meta->v_ident = (struct TOKEN_Ident *)TOKEN_ALLOC(sizeof(struct TOKEN_Ident));
  if (meta->v_ident == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_ident->kind = IDENT_Keyword;
  meta->v_ident->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_long_ident(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Ident;
  meta->v_ident = (struct TOKEN_Ident *)TOKEN_ALLOC(sizeof(struct TOKEN_Ident));
  if (meta->v_ident == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_ident->kind = IDENT_Long;
  meta->v_ident->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_operator_punct(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Punct;
  meta->v_punct = (struct TOKEN_Punct *)TOKEN_ALLOC(sizeof(struct TOKEN_Punct));
  if (meta->v_punct == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_punct->kind = PUNCT_Operator;
  meta->v_punct->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_new_symbol_punct(struct TOKEN_Repr *v) {
  struct TOKEN_Meta *meta = TOKEN_ALLOC(sizeof(struct TOKEN_Meta));
  if (meta == NULL) {
    return NULL;
  }

  meta->kind = TOK_Punct;
  meta->v_punct = (struct TOKEN_Punct *)TOKEN_ALLOC(sizeof(struct TOKEN_Punct));
  if (meta->v_punct == NULL) {
    TOKEN_FREE(meta);
    return NULL;
  }
  meta->v_punct->kind = PUNCT_Symbol;
  meta->v_punct->repr = v;
  meta->line_at = 0; // Example value, adjust as needed
  meta->word_at = 0; // Example value, adjust as needed
  meta->next = NULL;

  return meta;
}

struct TOKEN_Meta *token_meta_append(struct TOKEN_Meta **head,
                                     struct TOKEN_Meta *append) {
  if (*head == NULL) {
    *head = append;
    return *head;
  }

  struct TOKEN_Meta *current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = append;
  return *head;
}

void token_meta_iter(struct TOKEN_Meta *head,
                     void (*iter_fn)(struct TOKEN_Meta *)) {
  struct TOKEN_Meta *current = head;
  while (current != NULL) {
    iter_fn(current);
    current = current->next;
  }
}

void token_meta_delete(struct TOKEN_Meta *v) {
  struct TOKEN_Meta *current = v;
  while (current != NULL) {
    struct TOKEN_Meta *next = current->next;
    TOKEN_FREE(current);
    current = next;
  }
}
