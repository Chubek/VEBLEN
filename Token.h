#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Veblen.h"

/* The following must be preprocessed with AllocPPx.pl */

/*!(alloc-pp __token_heap)!*/

#define TOKEN_ALLOC(n) __token_heap_alloc_notrace(n)
#define TOKEN_REALLOC(mem, nn) __token_heap_realloc(mem, nn)
#define TOKEN_FREE(mem) __token_heap_free(mem)

struct TOKEN_Repr;
struct TOKEN_Meta;
struct TOKEN_Integer;
struct TOKEN_Real;
struct TOKEN_Text;
struct TOKEN_Ident;
struct TOKEN_Punct;

struct TOKEN_Meta {
  enum {
    TOK_Integer,
    TOK_Real,
    TOK_Text,
    TOK_Ident,
    TOK_Punct,
  } kind;

  union {
    struct TOKEN_Integer *v_integer;
    struct TOKEN_Real *v_real;
    struct TOKEN_Text *v_text;
    struct TOKEN_Ident *v_ident;
    struct TOKEN_Punct *v_punct;
  };

  size_t line_at;
  size_t word_at;
  struct TOKEN_Meta *next;
};

struct TOKEN_Repr {
  uint8_t *buffer;
  size_t buf_length;

  struct TOKEN_Repr *next;
};

struct TOKEN_Integer {
  enum {
    INT_Decimal,
    INT_Hexadecimal,
    INT_Octal,
    INT_Binary,
  } kind;

  struct TOKEN_Repr *repr;
};

struct TOKEN_Real {
  enum {
    REAL_Float,
    REAL_Rational,
  } kind;

  struct TOKEN_Repr *repr;
};

struct TOKEN_Text {
  enum {
    TEXT_Char,
    TEXT_String,
    TEXT_Utf8String,
  } kind;

  struct TOKEN_Repr *repr;
};

struct TOKEN_Ident {
  enum {
    IDENT_ApostroFixed,
    IDENT_PascalCase,
    IDENT_TrainCase,
    IDENT_Keyword,
    IDENT_Long,
  } kind;

  struct TOKEN_Repr *repr;
};

struct TOKEN_Punct {
  enum {
    PUNCT_Operator,
    PUNCT_Symbol,
  } kind;

  struct TOKEN_Repr *repr;
};

#endif
