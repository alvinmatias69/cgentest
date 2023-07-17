#include "logger.h"
#include "util.h"
#include "writer.h"
#include <errno.h>
#include <jansson.h>
#include <mustach/mustach-jansson.h>
#include <string.h>

json_t *map_json(struct write_result_params *params);

void write_result(struct write_result_params *params) {
  log_info("start mapping to json\n");
  json_t *metadata_json = map_json(params);
  log_info("finish mapping to json\n");
  log_debugf("mapped json: \n%s\n", json_dumps(metadata_json, 0));

  log_debug("start writing result to target\n");
  int write_result =
      mustach_jansson_file(params->template, 0, metadata_json,
                           Mustach_With_AllExtensions, params->target);
  if (write_result != 0)
    throwf("error while writing result: %s\n", strerror(errno));

  log_debug("finish writing result to target\n");
}

// TODO: implement this
json_t *map_json(struct write_result_params *params) {
  json_t *root = json_object();
  return root;
}
