#include "cgentest.h"
#include "ctags_helper.h"
#include "local_limit.h"
#include "mapper.h"
#include "util.h"
#include <libgen.h>
#include <mustach/mustach-cjson.h>
#include <readtags.h>
#include <regex.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char *generate_target_file(struct arguments *args);
void add_header_to_target(char *input, char *target, bool is_custom_target);
size_t generate_proto(const char *source, struct function_prototype **protos,
                      struct arguments *args, bool name_only, bool apply_regex);

void generate_test(struct arguments *args) {
  if (access(args->input, F_OK) != 0) {
    log_errorf("unable to find file: %s\n", args->input);
    exit(1);
  }

  struct function_prototype *protos =
      malloc(sizeof(struct function_prototype) * FUNCTION_PROTO_BOUND);
  check_malloc(protos);
  size_t protos_count = generate_proto(args->input, &protos, args, false, true);

  if (protos_count == 0) {
    log_warn("no function found, exiting.\n");
    free(protos);
    exit(0);
  }

  // can be disabled for non custom target
  char *target_file_name = generate_target_file(args);
  log_debugf("target file: %s\n", target_file_name);

  struct function_prototype *generated_protos = NULL;
  size_t generated_protos_count = 0;
  if (access(target_file_name, F_OK) == 0 && !args->ignore_target_current) {
    generated_protos =
        malloc(sizeof(struct function_prototype) * FUNCTION_PROTO_BOUND);
    check_malloc(generated_protos);

    generated_protos_count =
        generate_proto(target_file_name, &generated_protos, args, true, false);
  }

  // can be optimized for non custom target
  // handle multiple json lib
  cJSON *root = map_json(&protos, protos_count, &generated_protos,
                         generated_protos_count, args->custom_target);

  char *template;
  if (args->custom_template) {
    if (access(args->template_file, F_OK) != 0) {
      log_errorf("Unable to find custom template file in: %s\n",
                 args->template_file);
      exit(1);
    }
    template = read_file(args->template_file);
  } else if (access(INSTALLED_TEMPLATE_PATH, F_OK) == 0) {
    template = read_file(INSTALLED_TEMPLATE_PATH);
  } else {
    template = read_file(LOCAL_TEMPLATE_PATH);
  } // handle if no default template found

  add_header_to_target(args->input, target_file_name, args->custom_target);
  FILE *target;
  if (args->custom_target) {
    target = fopen(target_file_name, "a");
  } else {
    target = stdout;
  }
  // handle multiple json lib
  int write_result =
      mustach_cJSON_file(template, 0, root, Mustach_With_AllExtensions, target);

  if (!args->custom_target)
    free(target_file_name);

  free(template);
  if (args->custom_target)
    fclose(target);

  // handle multiple json lib
  cJSON_Delete(root);

  for (size_t idx = 0; idx < generated_protos_count; idx++)
    free_proto(&generated_protos[idx]);

  if (generated_protos_count > 0)
    free(generated_protos);

  for (size_t idx = 0; idx < protos_count; idx++)
    free_proto(&protos[idx]);

  free(protos);
}

size_t generate_proto(const char *source, struct function_prototype **protos,
                      struct arguments *args, bool name_only,
                      bool apply_regex) {
  char *tag_name = malloc(sizeof(char *) * MAX_FILENAME_LENGTH);
  check_malloc(tag_name);
  snprintf(tag_name, MAX_FILENAME_LENGTH, "%s/tags%lu", P_tmpdir, time(NULL));
  log_debugf("tag file: %s\n", tag_name);

  tagFile *tags;
  generate_tags(source, tag_name, &tags);
  log_debug("finish generate tags\n");
  remove(tag_name);
  free(tag_name);

  size_t count = 0;
  tagEntry entry;
  tagResult result = tagsFirst(tags, &entry);

  if (result != TagSuccess) {
    tagsClose(tags);
    return count;
  }

  size_t max = FUNCTION_PROTO_BOUND;

  regex_t only_regex;
  regex_t excl_regex;
  if (apply_regex) {
    if (args->has_only) {
      int result = regcomp(&only_regex, args->only, 0);
      if (result != 0) {
        log_errorf("failure to compile only regex: %s\n", args->only);
        exit(1);
      }
    }

    if (args->has_exclude) {
      int result = regcomp(&excl_regex, args->exclude, 0);
      if (result != 0) {
        log_errorf("failure to compile exclude regex: %s\n", args->exclude);
        exit(1);
      }
    }
  }

  while (result == TagSuccess) {
    if (apply_regex) {
      // generalize this if possible
      if (args->has_exclude) {
        int regex_result = regexec(&excl_regex, entry.name, 0, NULL, 0);
        if (regex_result == 0) {
          log_debugf("function \"%s\" match exclude pattern \"%s\". Skipping\n",
                     entry.name, args->exclude);
          result = tagsNext(tags, &entry);
          continue;
        } else if (regex_result == REG_NOMATCH) {
          log_debug("function doesn't match exclude pattern. Continuing\n");
        } else {
          log_error("error while executing exclude regex\n");
          exit(1);
        }
      }

      if (args->has_only) {
        int regex_result = regexec(&only_regex, entry.name, 0, NULL, 0);
        if (regex_result == REG_NOMATCH) {
          log_debugf(
              "function \"%s\" doesn't match only pattern \"%s\". Skipping\n",
              entry.name, args->only);
          result = tagsNext(tags, &entry);
          continue;
        } else if (regex_result == 0) {
          log_debug("function match only pattern. Continuing\n");
        } else {
          log_error("error while executing only regex\n");
          exit(1);
        }
      }
    }

    if (count + 1 >= max) {
      max += FUNCTION_PROTO_BOUND;
      *protos = realloc(*protos, sizeof(struct function_prototype) * max);
      check_malloc(protos);
    }
    (*protos)[count++] = map_proto(&entry, name_only);
    result = tagsNext(tags, &entry);
  }

  tagsClose(tags);
  return count;
}

// TODO: delete this as we default to stdout instead
char *generate_target_file(struct arguments *args) {
  if (args->custom_target) {
    return args->target;
  }

  size_t pos = 0;
  size_t input_name_length = strnlen(args->input, MAX_FILENAME_LENGTH);
  for (size_t idx = 0; idx < input_name_length; idx++) {
    if (args->input[idx] == '.')
      pos = idx;
  }

  char *input_name = strndup(args->input, pos);
  char *extension = strndup(args->input + pos, input_name_length);

  char *target = malloc(sizeof(char *) * MAX_FILENAME_LENGTH);
  check_malloc(target);
  snprintf(target, MAX_FILENAME_LENGTH, "%s_test%s", input_name, extension);
  free(input_name);
  free(extension);
  return target;
}

void add_header_to_target(char *input, char *target, bool is_custom_target) {
  if (is_custom_target && access(target, F_OK) == 0) {
    log_debug("target already exist, skipping header generation\n");
    return;
  }

  char *include_header = malloc(sizeof(char *) * MAX_INCLUDE_LENGTH);
  check_malloc(include_header);

  // clean input name
  char *input_copy = strndup(input, MAX_INPUT_LENGTH);
  char *base_input_name = basename(input_copy);

  snprintf(include_header, MAX_INCLUDE_LENGTH,
           "#include \"%s\" // basefile expected to be in the same directory, "
           "change this as necesarry\n",
           base_input_name);
  free(input_copy);

  // can be optimized by passing the FILE from main function inste
  FILE *f;
  if (is_custom_target) {
    f = fopen(target, "w");
  } else {
    f = stdout;
  }
  fputs(include_header, f);
  fputs("#include <stdlib.h>\n", f);
  fputs("#include <stdio.h>\n\n", f);
  free(include_header);
  if (is_custom_target)
    fclose(f);
}
