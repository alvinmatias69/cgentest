#include "cgentest.h"
#include "ctags_helper.h"
#include "libs/mustach/mustach-cjson.h"
#include "mapper.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

size_t map_tags_to_proto(tagFile *tags, const char *query,
                         struct function_prototype **protos);
char *read_file(char *path);

void generate_test(struct arguments *args) {
  char source_tag_name[100];
  snprintf(source_tag_name, sizeof source_tag_name, "sourcetags%lu",
           time(NULL));
  log_debugf("source tag file: %s\n", source_tag_name);

  tagFile *source_tag;
  generate_tags(args->input, source_tag_name, &source_tag);
  log_debug("finish generate tags\n");

  struct function_prototype *protos =
      malloc(sizeof(struct function_prototype) * 100);
  size_t protos_count = map_tags_to_proto(source_tag, args->filter, &protos);
  remove(source_tag_name);
  if (protos_count == 0) {
    log_warn("no function found, exiting.\n");
    free(protos);
    exit(0);
  }

  /* for (size_t idx = 0; idx < protos_count; idx++) { */
  /*   print_proto(&protos[idx]); */
  /* } */

  cJSON *root = map_json(&protos, protos_count);
  /* log_debugf("json result: %s\n", cJSON_Print(root)); */

  char *target_file;
  if (args->custom_target) {
    target_file = strndup(args->target, 100);
  } else {
    target_file = malloc(sizeof(char *) * 100);
    snprintf(target_file, 100, "test_%s", args->input);
  }

  FILE *target = fopen(target_file, "a");
  char *template = read_file("template");

  int write_result =
      mustach_cJSON_file(template, 0, root, Mustach_With_AllExtensions, target);

  free(template);
  fclose(target);

  cJSON_Delete(root);
  free(protos);
}

size_t map_tags_to_proto(tagFile *tags, const char *query,
                         struct function_prototype **protos) {
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
    (*protos)[count++] = map_proto(&entry, false);
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
