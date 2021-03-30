#ifndef LEXER
#define LEXER

#include <stdio.h>

enum token_type {
  LEFT_PAREN = '(',
  RIGHT_PAREN = ')',
  LEFT_BRACE = '{',
  RIGHT_BRACE = '}',
  COMMA = ',',
  SEMICOLON = ';',
  AMPERSAND = '&',
  STAR = '*'
};

struct token {
  enum token_type type;
  char *identifier;
  struct token *next;
};

struct token *tokenize(FILE *stream);

#endif
