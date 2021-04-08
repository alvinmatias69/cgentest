#ifndef EXTRACTOR_UTIL
#define EXTRACTOR_UTIL

#include "lexer.h"
#include "trie.h"

struct trie_node *init_trie();
void free_trie(struct trie_node *node);
void free_token(struct token *node);

#endif
