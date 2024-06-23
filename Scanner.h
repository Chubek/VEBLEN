#ifndef SCANNER_H
#define SCANNER_H

#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "Strapper.h"

/* The following needs to be pre-processed with AllocPPx.pl */

/*!(alloc-pp __lexer_heap)!*/

#define LEXER_ALLOC(n) __lexer_heap_alloc_notrace(n)
#define LEXER_REALLOC(mem, nn) __lexer_heap_realloc(mem, nn)
#define LEXER_FREE(mem) __lexer_heap_free(mem)

struct LEX_String;
struct LEX_Lexeme;
struct LEX_Sentence;
struct LEX_Token;
struct LEX_Ident;
struct LEX_Operator;
struct LEX_Literal;
struct LEX_Delimiter;
struct LEX_Separator;
struct LEX_Input;
struct LEX_File;
struct LEX_Repl;
struct LEX_State;
struct LEX_Rule;

struct LEX_String {
  uint8_t *buffer;
  size_t buf_size;

  struct LEX_String *next;
};

struct LEX_Lexeme {
  struct LEX_Strig *value;
  struct LEX_Sentence *source_sent;

  struct LEX_Lexeme *next;
};

struct LEX_Sentence {
  struct LEX_Lexeme *lexemes;
  struct LEX_Lexeme **root_lexeme;
  struct LEX_Lexeme **top_lexeme;
  size_t num_lexemes;

  struct LEX_Sentence *next;
};

struct LEX_Token {
  enum {
    TOK_Ident,
    TOK_Operator,
    TOK_Literal,
    TOK_Delimiter,
    TOK_Separator,
  } kind;

  union {
    struct LEX_Ident *v_ident;
    struct LEX_Operator *v_op;
    struct LEX_Literal *v_lit;
    struct LEX_Delimiter *v_delim;
    struct LEX_Separator *v_sep;
  };

  struct LEX_Lexeme *source;
  struct LEX_Token *next;
};

struct LEX_Ident {
  enum {
    IDENT_PascalCase,
    IDENT_TrainCase,
  } kind;

  bool is_keyword;
  struct LEX_String *value;
};

struct LEX_Operator {
  bool is_intrinsic;
  struct LEX_String *value;
};

struct LEX_Literal {
  enum {
    LIT_Numeric,
    LIT_String,
    LIT_Char,
  } kind;

  struct LEX_String *value;
};

struct LEX_Delimiter {
  enum {
    DELIM_Left,
    DELIM_Right,
  } side;

  enum {
    DELIM_Curly,
    DELIM_Paren,
    DELIM_Square,
    DELIM_Angle,
  } sym;
};

struct LEX_Separator {
  enum {
    SEP_Comma,
    SEP_Semi,
    SEP_Colon,
  } kind;

  bool at_tail;
};

struct LEX_Input {
  enum {
    SRCIN_File,
    SRCIN_Repl,
    SRCIN_String,
  } kind;

  union {
    struct LEX_File *v_file;
    struct LEX_Repl *v_repl;
    struct LEX_String *v_string;
  };

  struct LEX_Sentence *read_sents;
  struct LEX_Sentence **root_sent;
  struct LEX_Sentence **top_sent;
  size_t num_sents;

  struct LEX_Input *next;
};

struct LEX_File {
  bool is_stdin;
  FILE *stream;
  DIR *parent;
  char path[MAX_FILE_PATH + 1];
};

struct LEX_Repl {
  struct LEX_String *history;
  struct LEX_String **hist_root;
  struct LEX_String **hist_top;
  size_t num_hist;
  int term_fd;
  struct termios term_info;
};

struct LEX_State {
  struct LEX_Input *inputs;
  size_t num_inputs;

  struct LEX_Token *token_stream;
  size_t num_tokens;

  struct LEX_Rule *installed_rules;
  size_t num_rules;

  uint8_t *last_consumed;
};

struct LEX_Rule {
  uint8_t accept_buffer[MAX_ACCEPT_BUFFER + 1];
  uint8_t consume_buffer[MAX_CONSUME_BUFFER + 1];

  void (*accept_fn)(uint8_t *);
  void (*reject_fn)(void);

  struct LEX_Rule *next;
};

#endif /* Scanner.h */
