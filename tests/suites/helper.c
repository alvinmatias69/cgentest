#include "src/entity.h"
#include <stdlib.h>
#include <string.h>

bool metadataiseq(struct metadata *first, struct metadata *second) {
  if ((!first->name != !second->name) || // xor
      (first->name && second->name &&
       strncmp(first->name, second->name, 100) != 0))
    return false;

  if (first->return_type.is_primitive != second->return_type.is_primitive)
    return false;

  if (first->return_type.is_void != second->return_type.is_void)
    return false;

  if ((!first->return_type.name != !second->return_type.name) || // xor
      (first->return_type.name && second->return_type.name &&
       strncmp(first->return_type.name, second->return_type.name, 100) != 0))
    return false;

  if (first->parameter_count != second->parameter_count)
    return false;

  for (size_t idx = 0; idx < first->parameter_count; idx++) {
    if (strncmp(first->params[idx].name, second->params[idx].name, 100) != 0)
      return false;

    if (strncmp(first->params[idx].type, second->params[idx].type, 100) != 0)
      return false;
  }

  return true;
}

bool metadatalistiseq(struct metadata_list *first,
                      struct metadata_list *second) {
  if (first->count != second->count)
    return false;

  for (size_t idx = 0; idx < first->count; idx++)
    if (!metadataiseq(&first->list[idx], &second->list[idx]))
      return false;

  return true;
}

char *metadatafmt(struct metadata *metadata) {
  char *paramstr = reallocarray(NULL, sizeof(char *), 512);
  for (size_t inner = 0; inner < metadata->parameter_count; inner++) {
    char *paramcurrent = reallocarray(NULL, sizeof(char *), 512);
    snprintf(paramcurrent, 512,
             "- name : %s\n"
             "  type : %s\n",
             metadata->params[inner].name, metadata->params[inner].type);
    strncat(paramstr, paramcurrent, 512);
    free(paramcurrent);
  }

  char *result = reallocarray(NULL, sizeof(char *), 512);
  snprintf(result, 512,
           "name                   : %s\n"
           "return type            : %s\n"
           "return primitive type? : %d\n"
           "void function?         : %d\n"
           "parameters count       : %lu\n"
           "parameters             :\n%s"
           "---\n",
           metadata->name, metadata->return_type.name,
           metadata->return_type.is_primitive, metadata->return_type.is_void,
           metadata->parameter_count, paramstr);
  free(paramstr);

  return result;
}

// need to be freed after
char *metadatalistfmt(struct metadata_list *list) {
  char *result = reallocarray(NULL, sizeof(char *), 512);
  snprintf(result, 32, "count                  : %lu\n", list->count);
  for (size_t idx = 0; idx < list->count; idx++) {
    char *current = metadatafmt(&list->list[idx]);
    strncat(result, current, 512);
    free(current);
  }

  return result;
}
