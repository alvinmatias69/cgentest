#include "parser.h"
#include "local_limit.h"
#include "logger.h"
#include "util.h"
#include <errno.h>
#include <readtags.h>
#include <regex.h>
#include <string.h>
#include <time.h>

#define DEFAULT_CTAGS_BIN "ctags"
#define TAGS_FILENAME_FMT "%s/tags%lu"
#define RETURN_INDICATOR "typeref"
#define PARAMETER_INDICATOR "signature"
#define PRIMITIVE_INDICATOR "typename"
#define VOID_INDICATOR "void"
#define VOID_PARAMETER "(void)"
#define DELIMITER ","
#define VARIADIC "..."

void generate_tags(const char *source, bool has_custom_ctags_bin,
                   char *ctags_bin_path, tagFile **tags);
void compile_regex(bool enabled, char *pattern, regex_t *regex);
bool regex_match(const char *string, regex_t *regex, char *pattern);
struct metadata parse_single(tagEntry *entry, bool name_only);
void parse_return_type(const char *typeref, struct metadata *metadata);
void parse_parameters(const char *parameters, struct metadata *metadata);

struct metadata_list *parse(struct parse_arguments *args) {
  struct metadata_list *result =
      reallocarray_with_check(NULL, 1, sizeof(struct metadata_list));
  result->count = 0;

  tagFile *tags;
  log_info("start generating tags\n");
  generate_tags(args->input, args->has_custom_ctags_bin, args->ctags_bin_path,
                &tags);
  log_info("finish generating tags\n");

  tagEntry entry;
  tagResult tag_result = tagsFirst(tags, &entry);

  if (tag_result != TagSuccess) {
    tagsClose(tags);
    throwf("error while opening tags for: %s\n", args->input);
  }

  regex_t only_regex;
  bool apply_only_filter = args->apply_filter && args->has_only;
  compile_regex(apply_only_filter, args->only, &only_regex);

  regex_t excl_regex;
  bool apply_excl_filter = args->apply_filter && args->has_exclude;
  compile_regex(apply_excl_filter, args->exclude, &excl_regex);

  size_t max = FUNCTION_PROTO_BOUND;
  struct metadata *list = reallocarray_with_check(NULL, FUNCTION_PROTO_BOUND,
                                                  sizeof(struct metadata));

  while (tag_result == TagSuccess) {
    // skip parsing if match the filter
    if ((apply_excl_filter &&
         regex_match(entry.name, &excl_regex, args->exclude)) ||
        (apply_only_filter &&
         !regex_match(entry.name, &only_regex, args->only))) {
      tag_result = tagsNext(tags, &entry);
      continue;
    }

    if (result->count + 1 >= max) {
      max += FUNCTION_PROTO_BOUND;
      list = reallocarray_with_check(list, max, sizeof(struct metadata));
    }

    log_infof("start parsing function: %s\n", entry.name);
    list[result->count++] = parse_single(&entry, args->name_only);
    log_infof("finish parsing function: %s\n", entry.name);
    tag_result = tagsNext(tags, &entry);
  }

  tagsClose(tags);
  result->list = list;
  return result;
}

void generate_tags(const char *source, bool has_custom_ctags_bin,
                   char *ctags_bin_path, tagFile **tags) {
  char command[MAX_COMMAND_LENGTH];
  char *ctags_bin = DEFAULT_CTAGS_BIN;
  if (has_custom_ctags_bin) {
    log_warnf("using custom ctags bin path: %s\n", ctags_bin_path);
    ctags_bin = ctags_bin_path;
  }

  char *target_name =
      reallocarray_with_check(NULL, MAX_FILENAME_LENGTH, sizeof(char));
  snprintf(target_name, MAX_FILENAME_LENGTH, TAGS_FILENAME_FMT, P_tmpdir,
           time(NULL));
  log_debugf("tag file: %s\n", target_name);

  snprintf(command, sizeof command,
           "%s --kinds-c=f --_xformat=\"%%N %%t %%S\" -o %s %s", ctags_bin,
           target_name, source);
  log_debugf("ctags generate command: %s\n", command);

  FILE *fp = popen(command, "w");
  if (fp == NULL)
    throwf("error while generating ctags: %s\n", strerror(errno));

  pclose(fp);

  tagFileInfo info;
  *tags = tagsOpen(target_name, &info);
  log_debug("finish open tag\n");
  if (tags == NULL)
    throwf("unable to read tags. Error number %d\n", info.status.error_number);

  remove(target_name);
  free(target_name);
}

void compile_regex(bool enabled, char *pattern, regex_t *regex) {
  if (!enabled)
    return;

  int result = regcomp(regex, pattern, REG_EXTENDED);
  if (result != 0)
    throwf("failure to compile regex: \"%s\"\n", pattern);
}

bool regex_match(const char *string, regex_t *regex, char *pattern) {
  int result = regexec(regex, string, 0, NULL, 0);

  if (result == 0) {
    log_debugf("function \"%s\" match pattern \"%s\".\n", string, pattern);
    return true;
  }

  if (result == REG_NOMATCH) {
    log_debugf("function \"%s\" doesn't match pattern \"%s\".\n", string,
               pattern);
    return false;
  }

  throwf("error while executing regex pattern \"%s\" for \"%s\"\n", pattern,
         string);
}

struct metadata parse_single(tagEntry *entry, bool name_only) {
  struct metadata metadata = {
      .name = strndup(entry->name, MAX_FUNCTION_NAME_LENGTH),
  };

  if (name_only) {
    return metadata;
  }

  for (unsigned short idx = 0; idx < entry->fields.count; idx++) {
    if (strncmp(entry->fields.list[idx].key, RETURN_INDICATOR,
                MAX_TOKEN_LENGTH) == 0) {
      parse_return_type(entry->fields.list[idx].value, &metadata);
    } else if (strncmp(entry->fields.list[idx].key, PARAMETER_INDICATOR,
                       MAX_TOKEN_LENGTH) == 0) {
      parse_parameters(entry->fields.list[idx].value, &metadata);
    }
  }

  return metadata;
}

void parse_return_type(const char *typeref, struct metadata *metadata) {
  log_debugf("raw return type: %s\n", typeref);
  size_t typeref_length = strnlen(typeref, MAX_FUNCTION_TYPE_LENGTH);
  size_t pos = 0;
  for (size_t idx = 0; idx < typeref_length; idx++)
    if (typeref[idx] == ':')
      pos = idx;

  char *modifier = strndup(typeref, pos);
  metadata->return_type.is_primitive =
      strncmp(modifier, PRIMITIVE_INDICATOR, MAX_TOKEN_LENGTH) == 0;

  char *name = strndup(typeref + pos + 1, typeref_length);
  metadata->return_type.is_void =
      strncmp(name, VOID_INDICATOR, MAX_TOKEN_LENGTH) == 0;

  if (metadata->return_type.is_primitive) {
    free(modifier);
    metadata->return_type.name = name;
    return;
  }

  char *tmp_name =
      reallocarray_with_check(NULL, MAX_TOKEN_LENGTH, sizeof(char));

  snprintf(tmp_name, MAX_TOKEN_LENGTH, "%s %s", modifier, name);
  metadata->return_type.name = tmp_name;
  free(name);
  free(modifier);
}

void parse_parameters(const char *parameters, struct metadata *metadata) {
  log_debugf("raw parameters: %s\n", parameters);
  // ignore empty parameters (e.g. int example())
  if (strnlen(parameters, MAX_PARAMS_LENGTH) <= 2)
    return;

  // ignore parameters for void (e.g. int example(void))
  if (strncmp(parameters, VOID_PARAMETER, MAX_PARAMS_LENGTH) == 0)
    return;

  size_t sign_length = strnlen(parameters, MAX_PARAMS_LENGTH);
  char *raw = reallocarray_with_check(NULL, sign_length, sizeof(char));

  // substring of parameters, removing parenthesis
  size_t count = 0; // count is the number of parameters
  for (size_t idx = 0; idx < (sign_length - 2); idx++) {
    if (parameters[idx + 1] == ',')
      count++;

    raw[idx] = parameters[idx + 1];
  }
  raw[sign_length - 2] = '\0';
  count++;

  struct parameter *result =
      reallocarray_with_check(NULL, count, sizeof(struct parameter));

  size_t idx = 0;
  char *token = strtok(raw, DELIMITER);
  size_t token_length;
  size_t whitespace_loc;

  while (token) {
    // stop the iteration on variadic param
    if (strncmp(token, VARIADIC, MAX_PARAMS_LENGTH) == 0) {
      count--; // reduce the count as we left out the variadic
      break;
    }

    token_length = strnlen(token, MAX_TOKEN_LENGTH);
    for (size_t token_idx = 0; token_idx < token_length; token_idx++)
      if (token[token_idx] == ' ')
        whitespace_loc = token_idx;

    char *name = reallocarray_with_check(NULL, token_length - whitespace_loc,
                                         sizeof(char));
    char *type =
        reallocarray_with_check(NULL, whitespace_loc + 1, sizeof(char));

    strncpy(name, token + whitespace_loc + 1, token_length - whitespace_loc);
    strncpy(type, token, whitespace_loc);
    type[whitespace_loc] = '\0';
    struct parameter parameter = {.name = name, .type = type};
    result[idx++] = parameter;
    token = strtok(NULL, DELIMITER);
  }

  metadata->parameter_count = count;
  metadata->params = result;
  free(raw);
}
