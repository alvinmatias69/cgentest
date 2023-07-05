#ifndef ENTITY_H_
#define ENTITY_H_

#include <stdbool.h>
#include <stdio.h>

struct parameter {
  char *name;
  char *type;
};

struct metadata {
  char *name;
  struct {
    char *name;
    bool is_primitive;
    bool is_void;
  } return_type;
  struct parameter *params;
  size_t parameter_count;
};

struct metadata_list {
  struct metadata *list;
  size_t count;
};

#endif // ENTITY_H_
