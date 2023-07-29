#include "logger.h"
#include "util.h"
#include "writer.h"
#include <errno.h>
#include <json-c/json_object.h>
#include <mustach/mustach-json-c.h>
#include <string.h>

json_object *map_json(struct write_result_params *params);

void write_result(struct write_result_params *params) {
  log_warn("start writing result using json-c\n");
  log_info("start mapping to json\n");
  json_object *metadata_json = map_json(params);
  log_info("finish mapping to json\n");
  log_debugf("mapped json:\n%s\n", json_object_to_json_string_ext(
                                       metadata_json, JSON_C_TO_STRING_PRETTY));

  log_debug("start writing result to target\n");
  int write_result =
      mustach_json_c_file(params->template, 0, metadata_json,
                          Mustach_With_AllExtensions, params->target);
  if (write_result != 0)
    throwf("error while writing result: %s\n", strerror(errno));

  log_debug("finish writing result to target\n");
  json_object_put(metadata_json); // free the json root object
}

json_object *map_json(struct write_result_params *params) {
  json_object *root = json_object_new_object();

  // add include header
  json_object_object_add(root, "use_header",
                         json_object_new_boolean(params->use_header));
  json_object_object_add(root, "source",
                         json_object_new_string(params->source));

  json_object *metadata_list = json_object_new_array();
  json_object_object_add(root, "metadata_list", metadata_list);

  for (size_t idx = 0; idx < params->metadata_list->count; idx++) {
    json_object *metadata = json_object_new_object();
    json_object_array_add(metadata_list, metadata);

    struct metadata *current = &params->metadata_list->list[idx];
    json_object_object_add(metadata, "name",
                           json_object_new_string(current->name));
    json_object_object_add(metadata, "type",
                           json_object_new_string(current->return_type.name));
    json_object_object_add(
        metadata, "is_primitive",
        json_object_new_boolean(current->return_type.is_primitive));
    json_object_object_add(
        metadata, "is_void",
        json_object_new_boolean(current->return_type.is_void));

    // parse parameters to json object
    json_object *parameters = json_object_new_array();
    json_object_object_add(metadata, "parameters", parameters);

    for (size_t param_idx = 0; param_idx < current->parameter_count;
         param_idx++) {
      json_object *parameter = json_object_new_object();
      json_object_array_add(parameters, parameter);

      struct parameter cur_param = current->params[param_idx];
      json_object_object_add(parameter, "name",
                             json_object_new_string(cur_param.name));
      json_object_object_add(parameter, "type",
                             json_object_new_string(cur_param.type));
      json_object_object_add(
          parameter, "is_last",
          json_object_new_boolean(param_idx == current->parameter_count - 1));
    }
  }

  return root;
}
