#include "util.h"
#include "local_limit.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

char *read_file(char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL)
    throwf("error while opening %s: %s\n", path, strerror(errno));

  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *data = reallocarray_with_check(NULL, filesize + 1, sizeof(char));
  fread(data, filesize, 1, f);
  fclose(f);

  data[filesize] = '\0';

  return data;
}

bool name_in_list(struct metadata_list *list, char *query) {
  for (size_t idx = 0; idx < list->count; idx++)
    if (strncmp(query, list->list[idx].name, MAX_FUNCTION_NAME_LENGTH) == 0)
      return true;

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
  log_debug("Metadata properties\n");
  log_debugf("count: %d\n", list->count);

  for (size_t idx = 0; idx < list->count; idx++) {
    struct metadata current = list->list[idx];
    log_debugf("name                  : %s\n", current.name);
    log_debugf("return type           : %s\n", current.return_type.name);
    log_debugf("return primitive type?: %s\n",
               strbool(current.return_type.is_primitive));
    log_debugf("void func?            : %s\n",
               strbool(current.return_type.is_void));
    log_debugf("parameter count       : %d\n", current.parameter_count);
    log_debug("parameters             :\n");
    for (size_t inner = 0; inner < current.parameter_count; inner++) {
      log_debugf("  name : %s\n", current.params[inner].name);
      log_debugf("  type : %s\n", current.params[inner].type);
    }
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

void *reallocarray_with_check(void *ptr, size_t nmemb, size_t size) {
  void *result = reallocarray(ptr, nmemb, size);
  if (result == NULL)
    throw("error while allocating memory.\n");

  return result;
}

inline char *strbool(bool var) { return var ? "true" : "false"; }

inline char *strloglvl(enum log_level level) {
  switch (level) {
  case ERROR_LEVEL:
    return "error";
  case WARN_LEVEL:
    return "warn";
  case INFO_LEVEL:
    return "info";
  case DEBUG_LEVEL:
    return "debug";
  }
}
