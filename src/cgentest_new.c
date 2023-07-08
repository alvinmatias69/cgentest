#include "cgentest.h"
#include "local_limit.h"
#include "parser.h"
#include "util.h"
#include "writer.h"
#include <stdbool.h>
#include <unistd.h>

char *get_template(struct arguments *args);
struct metadata_list *filter(struct metadata_list *source,
                             struct metadata_list *target);

void generate_test(struct arguments *args) {
  // check whether the input is available
  if (access(args->input, F_OK) != 0) {
    log_errorf("unable to find file: %s\n", args->input);
    exit(1);
  }

  struct parse_arguments parse_arguments = {
      .input = args->input,
      .only = args->only,
      .has_only = args->has_only,
      .exclude = args->exclude,
      .has_exclude = args->has_exclude,
      .ctags_bin_path = args->ctags_bin_path,
      .has_custom_ctags_bin = args->has_custom_ctags_bin,
      .name_only = false,
      .apply_filter = true,
  };
  struct metadata_list *source_metadata = parse(&parse_arguments);
  if (source_metadata->count == 0) {
    log_warn("no function found, exiting.\n");
    free_metadata_list(source_metadata, false);
    exit(0);
  }

  bool use_header = true;
  FILE *target = stdout;
  struct metadata_list *target_metadata = NULL;
  if (args->custom_target) {
    if (access(args->target, F_OK) == 0) {
      use_header = false;
      if (!args->ignore_target_current) {
        // change arguments for target only
        parse_arguments.name_only = true;
        parse_arguments.apply_filter = false;
        parse_arguments.input = args->target;
        target_metadata = parse(&parse_arguments);
      }
    }
    target = fopen(args->target, "a");
  }

  struct metadata_list *result = source_metadata;

  if (target_metadata != NULL) {
    if (target_metadata->count > 0) {
      result = filter(source_metadata, target_metadata);
    }

    free_metadata_list(target_metadata, true);
  }

  char *template = get_template(args);

  struct write_result_params write_arguments = {
      .metadata_list = result,
      .target = target,
      .template = template,
      .use_header = use_header,
      .source = args->input,
  };
  write_result(&write_arguments);

  free(template);
  free_metadata_list(result, true);
}

char *get_template(struct arguments *args) {
  if (args->custom_template) {
    if (access(args->template_file, F_OK) != 0)
      throwf("Unable to find custom template file in: %s\n",
             args->template_file);

    return read_file(args->template_file);
  }

  if (access(INSTALLED_TEMPLATE_PATH, F_OK) == 0)
    return read_file(INSTALLED_TEMPLATE_PATH);

  if (access(LOCAL_TEMPLATE_PATH, F_OK) == 0)
    return read_file(LOCAL_TEMPLATE_PATH);

  throw("Unable to find any template\n");
}

struct metadata_list *filter(struct metadata_list *source,
                             struct metadata_list *target) {
  struct metadata_list *result =
      reallocarray_with_check(NULL, 1, sizeof(struct metadata_list));
  result->count = 0;
  result->list =
      reallocarray_with_check(NULL, source->count, sizeof(struct metadata));

  for (size_t idx = 0; idx < source->count; idx++) {
    char *name =
        reallocarray_with_check(NULL, MAX_FUNCTION_NAME_LENGTH, sizeof(char));
    snprintf(name, MAX_FUNCTION_NAME_LENGTH, "%s_test", source->list[idx].name);

    if (name_in_list(target, name)) {
      log_debugf("function %s already present in target. Skipping...\n", name);
      free_metadata(&source->list[idx]);
      free(name);
      continue;
    }

    result->list[result->count++] = source->list[idx];
    free(name);
  }

  free_metadata_list(source, false);
  return result;
}
