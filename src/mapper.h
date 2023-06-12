#ifndef MAPPER
#define MAPPER

#include <cjson/cJSON.h>
#include <readtags.h>
#include <stdbool.h>
#include <stdlib.h>

struct function_parameter {
  char *name;
  char *type;
};

struct function_prototype {
  char *name;
  struct {
    char *name;
    bool is_primitive;
    bool is_void;
  } return_type;
  struct function_parameter *parameters;
  size_t parameter_count;
};

struct function_prototype map_proto(tagEntry *entry, bool name_only);
cJSON *map_json(struct function_prototype **protos, size_t count,
                struct function_prototype **filter_out, size_t filter_out_count,
                bool is_custom_target);
void print_proto(struct function_prototype *proto);
void free_proto(struct function_prototype *proto);

#endif
