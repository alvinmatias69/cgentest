#include "logger.h"
#include "writer.h"
#include <json-c/json_object.h>
#include <mustach/mustach-json-c.h>

json_object *map_json(struct write_result_params *params);

void write_result(struct write_result_params *params) {
  log_info("start mapping to json\n");
  json_object *metadata_json = map_json(params);
  log_info("finish mapping to json\n");
  log_debugf("mapped json:\n%s\n", json_object_to_json_string(metadata_json));

  // handle the result
  log_debug("start writing result to target\n");
  int write_result =
      mustach_json_c_file(params->template, 0, metadata_json,
                          Mustach_With_AllExtensions, params->target);
  log_debug("finish writing result to target\n");
}

// TODO: Implement this
json_object *map_json(struct write_result_params *params) {
  json_object *root = json_object_new_object();
  return root;
}
