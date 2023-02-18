#ifndef CLI_H_
#define CLI_H_

#include "logger.h"
#include <stdbool.h>

struct arguments {
  char *input;
  char *target;
  char *filter;
  char *template_file;
  bool custom_target;
  bool custom_template;
  bool ignore_target_current;
  enum log_level log_level;
};

struct arguments parse_args(int argc, char *argv[]);
void print_args(struct arguments *args);
void free_args(struct arguments *args);

#endif // CLI_H_
