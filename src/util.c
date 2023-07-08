#include "util.h"
#include "local_limit.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

void check_malloc(void *ptr) {
  if (ptr == NULL) {
    throw("error while allocating memory.\n");
  }
}

char *read_file(char *path) {
  FILE *f = fopen(path, "r");

  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *data = malloc(filesize + 1);
  check_malloc(data);
  fread(data, filesize, 1, f);
  fclose(f);

  data[filesize] = '\0';

  return data;
}

bool name_in_list(struct metadata_list *list, char *query) {
  for (size_t idx = 0; idx < list->count; idx++) {
    if (strncmp(query, list->list[idx].name, MAX_FUNCTION_NAME_LENGTH) == 0)
      return true;
  }

  return false;
}

void free_metadata_list(struct metadata_list *list, bool free_member) {
  if (free_member)
    for (size_t idx = 0; idx < list->count; idx++)
      free_metadata(&list->list[idx]);

  free(list->list);
  free(list);
}

void free_metadata(struct metadata *metadata) {
  free(metadata->name);

  if (metadata->return_type.name != NULL &&
      strnlen(metadata->return_type.name, MAX_FUNCTION_NAME_LENGTH) > 0)
    free(metadata->return_type.name);

  for (size_t idx = 0; idx < metadata->parameter_count; idx++) {
    free(metadata->params[idx].name);
    free(metadata->params[idx].type);
  }

  if (metadata->parameter_count > 0)
    free(metadata->params);
}

void print_metadata_list(struct metadata_list *list) {
  log_infof("count: %d\n", list->count);

  for (size_t idx = 0; idx < list->count; idx++) {
    log_infof("name: %s\n", list->list[idx].name);
  }
}

void throw(const char *msg) {
  log_error(msg);
  exit(1);
}

void throwf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, ERROR_LEVEL);
  va_end(args);
  exit(1);
}
