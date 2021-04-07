#ifndef LEXER
#define LEXER

#include "token.h"
#include "trie.h"
#include <stdio.h>

#define MAXIMUM_IDENT_LENGTH 31

struct token {
  enum token_type type;
  char *identifier;
  struct token *next;
};

struct token *tokenize(FILE *stream, struct trie_node *trie);

#endif
