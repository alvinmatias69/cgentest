#include "util.h"
#include "lexer.h"

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
