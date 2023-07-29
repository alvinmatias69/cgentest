#ifndef UTIL_H_
#define UTIL_H_

#include "entity.h"
#include "logger.h"
#include <stdbool.h>
#include <stdlib.h>

char *read_file(char *path);
bool name_in_list(struct metadata_list *list, char *query);
void free_metadata(struct metadata *metadata);
void free_metadata_list(struct metadata_list *list, bool free_member);
void throw(const char *msg);
void throwf(const char *fmt, ...);
void *reallocarray_with_check(void *ptr, size_t nmemb, size_t size);
void print_metadata_list(struct metadata_list *list);
char *strbool(bool var);
char *strloglvl(enum log_level level);

#endif // UTIL_H_
