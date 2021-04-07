#ifndef TRIE
#define TRIE

#include "lexer.h"
#include <stdbool.h>

#define ALPHA_SIZE 26

struct trie_node {
  struct trie_node *children[ALPHA_SIZE];
  bool is_end;
  enum token_type type;
};

struct trie_node *new_trie();
void insert(struct trie_node *root, char *key, enum token_type type);
bool search(struct trie_node *root, char *key, enum token_type *type);

#endif
