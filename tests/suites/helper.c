#include "src/entity.h"
#include <stdlib.h>
#include <string.h>

bool metadataiseq(struct metadata *first, struct metadata *second) {
  if (strncmp(first->name, second->name, 100) != 0)
    return false;

  if (first->return_type.is_primitive != second->return_type.is_primitive)
    return false;

  if (first->return_type.is_void != second->return_type.is_void)
    return false;

  if (strncmp(first->return_type.name, second->return_type.name, 100) != 0)
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

// need to be freed after
char *metadatafmt(struct metadata_list *list) {
  char *result = reallocarray(NULL, sizeof(char *), 512);
  snprintf(result, 32, "count                  : %lu\n", list->count);
  for (size_t idx = 0; idx < list->count; idx++) {
    struct metadata current = list->list[idx];
    char *paramstr = reallocarray(NULL, sizeof(char *), 512);
    for (size_t inner = 0; inner < current.parameter_count; inner++) {
      char *paramcurrent = reallocarray(NULL, sizeof(char *), 512);
      snprintf(paramcurrent, 512,
               "- name : %s\n"
               "  type : %s\n",
               current.params[inner].name, current.params[inner].type);
      strncat(paramstr, paramcurrent, 512);
      free(paramcurrent);
    }

    char *currentstr = reallocarray(NULL, sizeof(char *), 512);
    snprintf(currentstr, 512,
             "name                   : %s\n"
             "return type            : %s\n"
             "return primitive type? : %d\n"
             "void function?         : %d\n"
             "parameters count       : %lu\n"
             "parameters             :\n%s"
             "---\n",
             current.name, current.return_type.name,
             current.return_type.is_primitive, current.return_type.is_void,
             current.parameter_count, paramstr);
    free(paramstr);
    strncat(result, currentstr, 512);
    free(currentstr);
  }

  return result;
}
