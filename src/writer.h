#ifndef WRITER_H_
#define WRITER_H_

#include "entity.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct write_result_params {
  struct metadata_list *metadata_list;
  char *template;
  FILE *target;
  bool use_header;
  char *source;
};

void write_result(struct write_result_params *params);

#endif // WRITER_H_
