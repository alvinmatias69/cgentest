#include "util.h"
#include <stdlib.h>

struct trie_node *init_trie() {
  struct trie_node *trie = new_trie();

  insert(trie, "auto", STORAGE_CLASS_SPECIFIER);
  insert(trie, "register", STORAGE_CLASS_SPECIFIER);
  insert(trie, "static", STORAGE_CLASS_SPECIFIER);
  insert(trie, "extern", STORAGE_CLASS_SPECIFIER);
  insert(trie, "typedef", STORAGE_CLASS_SPECIFIER);

  insert(trie, "void", TYPE_SPECIFIER);
  insert(trie, "char", TYPE_SPECIFIER);
  insert(trie, "short", TYPE_SPECIFIER);
  insert(trie, "int", TYPE_SPECIFIER);
  insert(trie, "long", TYPE_SPECIFIER);
  insert(trie, "float", TYPE_SPECIFIER);
  insert(trie, "double", TYPE_SPECIFIER);
  insert(trie, "signed", TYPE_SPECIFIER);
  insert(trie, "unsigned", TYPE_SPECIFIER);

  insert(trie, "const", TYPE_QUALIFIER);
  insert(trie, "union", TYPE_QUALIFIER);

  insert(trie, "struct", STRUCT_OR_UNION);
  insert(trie, "union", STRUCT_OR_UNION);

  return trie;
}

void free_trie(struct trie_node *node) {
  if (node == NULL)
    return;

  for (int idx = 0; idx < ALPHA_SIZE; idx++) {
    free_trie(node->children[idx]);
  }

  free(node);
}

void free_token(struct token *node) {
  if (node == NULL)
    return;

  free_token(node->next);

  if (node->type == TYPE_SPECIFIER || node->type == TYPE_QUALIFIER ||
      node->type == STORAGE_CLASS_SPECIFIER || node->type == STRUCT_OR_UNION ||
      node->type == IDENTIFIER)
    free(node->identifier);

  free(node);
}
