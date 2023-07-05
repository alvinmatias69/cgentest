#ifndef PARSER_H_
#define PARSER_H_

#include "entity.h"
#include <stdbool.h>
#include <stdlib.h>

struct parse_arguments {
  char *input;
  char *only;
  char *exclude;
  char *ctags_bin_path;
  bool has_only;
  bool has_exclude;
  bool has_custom_ctags_bin;
  bool name_only;
  bool apply_filter;
};

// Parse file arguments into list of metadata
// Will return size of metadata and set the metadata in the parameters
struct metadata_list *parse(struct parse_arguments *args);

#endif // PARSER_H_
