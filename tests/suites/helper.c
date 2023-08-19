#include "src/entity.h"
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
