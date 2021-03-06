#include "trie.h"
#include <stdlib.h>
#include <string.h>

int char_to_index(char c) { return (int)c - (int)'a'; }

// init new trie
struct trie_node *new_trie() {
  struct trie_node *node = malloc(sizeof(struct trie_node));
  node->is_end = false;
  for (int idx = 0; idx < ALPHA_SIZE; idx++) {
    node->children[idx] = NULL;
  }

  return node;
}

// insert new keyword to trie with its respective token
void insert(struct trie_node *node, char *key, enum token_type type) {
  int length = strlen(key);
  char ch;
  for (int idx = 0; idx < length; idx++) {
    if ((ch = char_to_index(key[idx])) > ALPHA_SIZE)
      return;

    if (node->children[ch] == NULL)
      node->children[ch] = new_trie();

    node = node->children[ch];
  }

  node->is_end = true;
  node->type = type;
}

// search a keyword in trie. If found will return true and set the type with its
// respective value. Return false otherwise.
bool search(struct trie_node *node, char *key, enum token_type *type) {
  int length = strlen(key);
  char ch;
  for (int idx = 0; idx < length; idx++) {
    if ((ch = char_to_index(key[idx])) > ALPHA_SIZE)
      return false;

    if (node->children[ch] == NULL)
      return false;

    node = node->children[ch];
  }

  if (!node->is_end)
    return false;

  *type = node->type;
  return true;
}
