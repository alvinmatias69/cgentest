#ifndef LEXER
#define LEXER

#include <stdio.h>

#define MAXIMUM_IDENT_LENGTH 31

enum token_type {
  LEFT_PAREN = '(',
  RIGHT_PAREN = ')',
  LEFT_BRACE = '{',
  RIGHT_BRACE = '}',
  COMMA = ',',
  SEMICOLON = ';',
  AMPERSAND = '&',
  STAR = '*',
  TYPE_SPECIFIER,
  TYPE_QUALIFIER,
  STORAGE_CLASS_SPECIFIER,
  IDENTIFIER
};

struct token {
  enum token_type type;
  char *identifier;
  struct token *next;
};

struct token *tokenize(FILE *stream);

#endif
