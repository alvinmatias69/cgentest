#include "logger.h"
#include "util.h"
#include "writer.h"
#include <errno.h>
#include <jansson.h>
#include <mustach/mustach-jansson.h>
#include <string.h>

json_t *map_json(struct write_result_params *params);

void write_result(struct write_result_params *params) {
  log_warn("start writing result using jansson\n");
  log_info("start mapping to json\n");
  json_t *metadata_json = map_json(params);
  log_info("finish mapping to json\n");
  log_debugf("mapped json: \n%s\n", json_dumps(metadata_json, JSON_INDENT(2)));

  log_debug("start writing result to target\n");
  int write_result =
      mustach_jansson_file(params->template, 0, metadata_json,
                           Mustach_With_AllExtensions, params->target);
  if (write_result != 0)
    throwf("error while writing result: %s\n", strerror(errno));

  log_debug("finish writing result to target\n");
  json_object_clear(metadata_json);
}

// TODO: Fix error while writing to mustach
json_t *map_json(struct write_result_params *params) {
  json_t *root = json_object();

  // add include header
  json_object_set(root, "use_header", json_boolean(params->use_header));
  json_object_set(root, "source", json_string(params->source));

  json_t *metadata_list = json_array();
  json_object_set(root, "metadata_list", metadata_list);

  for (size_t idx = 0; idx < params->metadata_list->count; idx++) {
    json_t *metadata = json_object();
    json_array_append(metadata_list, metadata);

    struct metadata *current = &params->metadata_list->list[idx];
    json_object_set(metadata, "name", json_string(current->name));
    json_object_set(metadata, "type", json_string(current->return_type.name));
    json_object_set(metadata, "is_primitive",
                    json_boolean(current->return_type.is_primitive));
    json_object_set(metadata, "is_void",
                    json_boolean(current->return_type.is_void));

    // parse parameters to json object
    json_t *parameters = json_array();
    json_object_set(metadata, "parameters", parameters);

    for (size_t param_idx = 0; param_idx < current->parameter_count;
         param_idx++) {
      json_t *parameter = json_object();
      json_array_append(parameters, parameter);

      struct parameter cur_param = current->params[param_idx];
      json_object_set(parameter, "name", json_string(cur_param.name));
      json_object_set(parameter, "type", json_string(cur_param.type));
      json_object_set(parameter, "is_last",
                      json_boolean(param_idx == current->parameter_count - 1));
    }
  }

  return root;
}
