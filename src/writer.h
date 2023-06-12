#ifndef WRITER_H_
#define WRITER_H_

#include "mapper.h"
#include <stdbool.h>
#include <stdio.h>

struct write_result_params {
  struct function_prototype **protos;
  size_t protos_count;
  struct function_prototype **generated;
  size_t generated_count;
  char *template;
  FILE *target;
};

bool write_result(struct write_result_params *params);

#endif // WRITER_H_
