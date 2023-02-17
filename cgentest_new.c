#include "cgentest.h"
#include "ctags_helper.h"
#include "libs/mustach/mustach-cjson.h"
#include "local_limit.h"
#include "mapper.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

size_t map_tags_to_proto(tagFile *tags, const char *query,
                         struct function_prototype **protos, bool name_only);
char *read_file(char *path);
char *generate_target_file(struct arguments *args);
void add_header_to_target(char *input, char *target);
size_t generate_proto(const char *source, struct function_prototype **protos,
                      const char *query, bool name_only);

void generate_test(struct arguments *args) {
  if (access(args->input, F_OK) != 0) {
    log_errorf("unable to find file: %s\n", args->input);
    exit(1);
  }

  char *target_file_name = generate_target_file(args);
  log_debugf("target file: %s\n", target_file_name);

  struct function_prototype *generated_protos = NULL;
  size_t generated_protos_count = 0;
  if (access(target_file_name, F_OK) == 0 && !args->ignore_target_current) {
    generated_protos =
        malloc(sizeof(struct function_prototype) * FUNCTION_PROTO_BOUND);
    generated_protos_count =
        generate_proto(target_file_name, &generated_protos, "", true);
  }

  add_header_to_target(args->input, target_file_name);

  struct function_prototype *protos =
      malloc(sizeof(struct function_prototype) * FUNCTION_PROTO_BOUND);
  size_t protos_count =
      generate_proto(args->input, &protos, args->filter, false);

  if (protos_count == 0) {
    log_warn("no function found, exiting.\n");
    free(protos);
    exit(0);
  }

  /* for (size_t idx = 0; idx < protos_count; idx++) { */
  /*   print_proto(&protos[idx]); */
  /* } */

  cJSON *root = map_json(&protos, protos_count);
  log_debugf("json result: %s\n", cJSON_Print(root));

  FILE *target = fopen(target_file_name, "a");
  char *template = read_file("template");

  int write_result =
      mustach_cJSON_file(template, 0, root, Mustach_With_AllExtensions, target);

  free(target_file_name);
  free(template);
  fclose(target);

  cJSON_Delete(root);
  free(protos);
}

size_t generate_proto(const char *source, struct function_prototype **protos,
                      const char *query, bool name_only) {
  char *tag_name = malloc(sizeof(char *) * MAX_FILENAME_LENGTH);
  snprintf(tag_name, MAX_FILENAME_LENGTH, "tags%lu", time(NULL));
  log_debugf("tag file: %s\n", tag_name);

  tagFile *tags;
  generate_tags(source, tag_name, &tags);
  log_debug("finish generate tags\n");
  remove(tag_name);
  free(tag_name);

  size_t count = 0;
  tagEntry entry;
  tagResult (*next)(tagFile *const, tagEntry *const);
  tagResult result;

  if (strnlen(query, MAX_QUERY_LENGTH) > 0) {
    next = tagsFindNext;
    result = tagsFind(tags, &entry, query, TAG_PARTIALMATCH | TAG_OBSERVECASE);
  } else {
    next = tagsNext;
    result = tagsFirst(tags, &entry);
  }

  if (result != TagSuccess) {
    tagsClose(tags);
    return count;
  }

  size_t max = FUNCTION_PROTO_BOUND;

  while (result == TagSuccess) {
    if (count + 1 >= max) {
      max += FUNCTION_PROTO_BOUND;
      *protos = realloc(*protos, sizeof(struct function_prototype) * max);
    }
    (*protos)[count++] = map_proto(&entry, name_only);
    result = (*next)(tags, &entry);
  }

  tagsClose(tags);
  return count;
}

size_t map_tags_to_proto(tagFile *tags, const char *query,
                         struct function_prototype **protos, bool name_only) {
  size_t count = 0;
  tagEntry entry;
  tagResult (*next)(tagFile *const, tagEntry *const);
  tagResult result;

  if (strnlen(query, 100) > 0) {
    next = tagsFindNext;
    result = tagsFind(tags, &entry, query, TAG_PARTIALMATCH | TAG_OBSERVECASE);
  } else {
    next = tagsNext;
    result = tagsFirst(tags, &entry);
  }

  if (result != TagSuccess) {
    tagsClose(tags);
    return count;
  }

  while (result == TagSuccess) {
    (*protos)[count++] = map_proto(&entry, name_only);
    result = (*next)(tags, &entry);
  }

  tagsClose(tags);
  return count;
}

char *read_file(char *path) {
  FILE *f = fopen(path, "r");

  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *data = malloc(filesize + 1);
  fread(data, filesize, 1, f);
  fclose(f);

  data[filesize] = '\0';

  return data;
}

char *generate_target_file(struct arguments *args) {
  if (args->custom_target) {
    return args->target;
  }

  size_t pos = 0;
  size_t input_name_length = strnlen(args->input, MAX_FILENAME_LENGTH);
  for (size_t idx; idx < input_name_length; idx++) {
    if (args->input[idx] == '.')
      pos = idx;
  }

  char *input_name = strndup(args->input, pos);
  char *extension = strndup(args->input + pos, input_name_length);

  char *target = malloc(sizeof(char *) * MAX_FILENAME_LENGTH);
  snprintf(target, MAX_FILENAME_LENGTH, "%s_test%s", input_name, extension);
  return target;
}

void add_header_to_target(char *input, char *target) {
  if (access(target, F_OK) == 0) {
    log_debug("target already exist, skipping header generation\n");
    return;
  }

  char *include_header = malloc(sizeof(char *) * MAX_INCLUDE_LENGTH);
  snprintf(include_header, MAX_INCLUDE_LENGTH, "#include \"%s\"\n", input);

  FILE *f = fopen(target, "w");
  fputs(include_header, f);
  fputs("#include <stdlib.h>\n\n", f);
  free(include_header);
  fclose(f);
}
