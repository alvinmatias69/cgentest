#include "logger.h"
#include "util.h"
#include "writer.h"
#include <cjson/cJSON.h>
#include <errno.h>
#include <mustach/mustach-cjson.h>
#include <string.h>

cJSON *map_json(struct write_result_params *params);

void write_result(struct write_result_params *params) {
  log_warn("start writing result using cjson\n");
  log_info("start mapping to json\n");
  cJSON *metadata_json = map_json(params);
  log_info("finish mapping to json\n");
  log_debugf("mapped json:\n%s\n", cJSON_Print(metadata_json));

  log_debug("start writing result to target\n");
  int write_result =
      mustach_cJSON_file(params->template, 0, metadata_json,
                         Mustach_With_AllExtensions, params->target);
  if (write_result != 0)
    throwf("error while writing result: %s\n", strerror(errno));

  log_debug("finish writing result to target\n");
  cJSON_Delete(metadata_json); // free the json root object
}

cJSON *map_json(struct write_result_params *params) {
  cJSON *root = cJSON_CreateObject();

  // add include header
  cJSON_AddItemToObject(root, "use_header",
                        cJSON_CreateBool(params->use_header));
  cJSON_AddItemToObject(root, "source", cJSON_CreateString(params->source));

  cJSON *metadata_list = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "metadata_list", metadata_list);

  for (size_t idx = 0; idx < params->metadata_list->count; idx++) {
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToArray(metadata_list, metadata);

    struct metadata *current = &params->metadata_list->list[idx];
    cJSON_AddItemToObject(metadata, "name", cJSON_CreateString(current->name));
    cJSON_AddItemToObject(metadata, "type",
                          cJSON_CreateString(current->return_type.name));
    cJSON_AddItemToObject(metadata, "is_primitive",
                          cJSON_CreateBool(current->return_type.is_primitive));
    cJSON_AddItemToObject(metadata, "is_void",
                          cJSON_CreateBool(current->return_type.is_void));

    // parse parameters to json object
    cJSON *parameters = cJSON_CreateArray();
    cJSON_AddItemToObject(metadata, "parameters", parameters);

    for (size_t param_idx = 0; param_idx < current->parameter_count;
         param_idx++) {
      cJSON *parameter = cJSON_CreateObject();
      cJSON_AddItemToArray(parameters, parameter);

      struct parameter cur_param = current->params[param_idx];
      cJSON_AddItemToObject(parameter, "name",
                            cJSON_CreateString(cur_param.name));
      cJSON_AddItemToObject(parameter, "type",
                            cJSON_CreateString(cur_param.type));
      cJSON_AddItemToObject(
          parameter, "is_last",
          cJSON_CreateBool(param_idx == current->parameter_count - 1));
    }
  }

  return root;
}
