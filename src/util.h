#ifndef UTIL_H_
#define UTIL_H_

#include "entity.h"
#include <stdbool.h>
#include <stdlib.h>

void check_malloc(void *ptr);
char *read_file(char *path);
bool name_in_list(struct metadata_list *list, char *query);
void free_metadata(struct metadata *metadata);
void free_metadata_list(struct metadata_list *list, bool free_member);

#endif // UTIL_H_
