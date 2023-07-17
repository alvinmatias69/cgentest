#include "logger.h"
#include "util.h"
#include "writer.h"
#include <errno.h>
#include <json-c/json_object.h>
#include <mustach/mustach-json-c.h>
#include <string.h>

json_object *map_json(struct write_result_params *params);

void write_result(struct write_result_params *params) {
  log_info("start mapping to json\n");
  json_object *metadata_json = map_json(params);
  log_info("finish mapping to json\n");
  log_debugf("mapped json:\n%s\n", json_object_to_json_string(metadata_json));

  log_debug("start writing result to target\n");
  int write_result =
      mustach_json_c_file(params->template, 0, metadata_json,
                          Mustach_With_AllExtensions, params->target);
  if (write_result != 0)
    throwf("error while writing result: %s\n", strerror(errno));

  log_debug("finish writing result to target\n");
}

// TODO: Implement this
json_object *map_json(struct write_result_params *params) {
  json_object *root = json_object_new_object();
  return root;
}
