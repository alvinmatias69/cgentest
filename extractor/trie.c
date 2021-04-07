#include "trie.h"
#include <stdlib.h>
#include <string.h>

int char_to_index(char c) { return (int)c - (int)'a'; }

struct trie_node *new_trie() {
  struct trie_node *node = malloc(sizeof(struct trie_node));
  node->is_end = false;
  for (int idx = 0; idx < ALPHA_SIZE; idx++) {
    node->children[idx] = NULL;
  }

  return node;
}

void insert(struct trie_node *root, char *key, enum token_type type) {
  int length = strlen(key);
  char ch;
  struct trie_node *node = root;
  for (int idx = 0; idx < length; idx++) {
    if ((ch = char_to_index(key[idx])) > ALPHA_SIZE)
      return;

    if (node->children[ch] == NULL)
      node->children[ch] = new ();

    node = node->children[ch];
  }

  node->is_end = true;
  node->type = type;
}

bool search(struct trie_node *root, char *key, enum token_type *type) {
  bool found = false;
  int length = strlen(key);
  char ch;
  struct trie_node *node = root;
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
