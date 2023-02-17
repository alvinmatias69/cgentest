#include "mapper.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

#define RETURN_INDICATOR "typeref"
#define PARAMETER_INDICATOR "signature"
#define PRIMITIVE_INDICATOR "typename"
#define VOID_INDICATOR "void"

void parse_return_type(const char *typeref, struct function_prototype *proto);
void parse_parameters(const char *parameters, struct function_prototype *proto);
void print_proto(struct function_prototype *proto);

struct function_prototype map_proto(tagEntry *entry, bool name_only) {
  struct function_prototype proto = {
      .name = strndup(entry->name, 100),
  };

  if (name_only) {
    return proto;
  }

  for (unsigned short idx = 0; idx < entry->fields.count; idx++) {
    if (strncmp(entry->fields.list[idx].key, RETURN_INDICATOR, 10) == 0) {
      parse_return_type(entry->fields.list[idx].value, &proto);
    } else if (strncmp(entry->fields.list[idx].key, PARAMETER_INDICATOR, 10) ==
               0) {
      parse_parameters(entry->fields.list[idx].value, &proto);
    }
  }

  return proto;
}

void print_proto(struct function_prototype *proto) {
  log_debugf("function prototype\n");
  log_debugf("name: %s\n", proto->name);
  log_debugf("return is primitive: %d\n", proto->return_type.is_primitive);
  log_debugf("function is void: %d\n", proto->return_type.is_void);
  log_debugf("return type: %s\n", proto->return_type.name);
  log_debugf("parameters:\n");
  for (size_t idx = 0; idx < proto->parameter_count; idx++) {
    log_debugf("\tname: %s\n", proto->parameters[idx].name);
    log_debugf("\ttype: %s\n", proto->parameters[idx].type);
  }
  log_debugf("\n");
}

void free_proto(struct function_prototype *proto) {
  free(proto->name);
  free(proto->return_type.name);
  for (size_t idx = 0; idx < proto->parameter_count; idx++) {
    free(proto->parameters[idx].name);
    free(proto->parameters[idx].type);
  }
  free(proto->parameters);
}

void parse_return_type(const char *typeref, struct function_prototype *proto) {
  const char *delimiter = ":";
  char *token = strtok(typeref, delimiter);
  bool is_primitive = (strncmp(token, PRIMITIVE_INDICATOR, 100) == 0);
  char *modifier = strndup(token, 100);

  token = strtok(NULL, delimiter);
  bool is_void = (strncmp(token, VOID_INDICATOR, 100) == 0);
  char result[100];
  if (is_primitive) {
    snprintf(result, 100, "%s", token);
  } else {
    snprintf(result, 100, "%s %s", modifier, token);
  }

  proto->return_type.is_primitive = is_primitive;
  proto->return_type.is_void = is_void;
  proto->return_type.name = strndup(result, 100);
  free(modifier);
}

void parse_parameters(const char *parameters,
                      struct function_prototype *proto) {
  size_t sign_length = strnlen(parameters, 100);
  char *raw = malloc(sign_length);

  // substring of parameters, removing parenthesis
  int count = 0; // count is the number of parameters
  for (size_t idx = 0; idx < (sign_length - 2); idx++) {
    if (parameters[idx + 1] == ',') {
      count++;
    }
    raw[idx] = parameters[idx + 1];
  }
  count++;

  const char *delimiter = ",";
  struct function_parameter *result =
      malloc(sizeof(struct function_parameter) * count);
  size_t idx = 0;
  char *token = strtok(raw, delimiter);
  size_t token_length;
  size_t whitespace_loc;
  while (token) {
    token_length = strnlen(token, 100);
    for (size_t token_idx = 0; token_idx < token_length; token_idx++) {
      if (token[token_idx] == ' ') {
        whitespace_loc = token_idx;
      }
    }

    char *name = malloc(sizeof(char *) * (token_length - whitespace_loc));
    char *type = malloc(sizeof(char *) * (whitespace_loc + 1));
    strncpy(name, token + whitespace_loc + 1, token_length - whitespace_loc);
    strncpy(type, token, whitespace_loc);
    type[whitespace_loc] = '\0';
    struct function_parameter parameter = {.name = name, .type = type};
    result[idx++] = parameter;
    token = strtok(NULL, delimiter);
  }

  proto->parameter_count = count;
  proto->parameters = result;
  free(raw);
}

cJSON *map_json(struct function_prototype **protos, size_t count) {
  cJSON *root = cJSON_CreateObject();
  cJSON *protos_json = cJSON_CreateArray();
  cJSON_AddItemToObject(root, "function_prototypes", protos_json);

  for (size_t idx = 0; idx < count; idx++) {
    cJSON *proto_json = cJSON_CreateObject();
    cJSON_AddItemToArray(protos_json, proto_json);
    cJSON_AddItemToObject(proto_json, "name",
                          cJSON_CreateString((*protos)[idx].name));
    cJSON_AddItemToObject(proto_json, "type",
                          cJSON_CreateString((*protos)[idx].return_type.name));
    cJSON_AddItemToObject(
        proto_json, "is_primitive",
        cJSON_CreateBool((*protos)[idx].return_type.is_primitive));
    cJSON_AddItemToObject(proto_json, "is_void",
                          cJSON_CreateBool((*protos)[idx].return_type.is_void));
    cJSON *parameters = cJSON_CreateArray();
    cJSON_AddItemToObject(proto_json, "parameters", parameters);
    for (size_t param_idx = 0; param_idx < (*protos)[idx].parameter_count;
         param_idx++) {
      cJSON *parameter = cJSON_CreateObject();
      cJSON_AddItemToArray(parameters, parameter);

      cJSON_AddItemToObject(
          parameter, "name",
          cJSON_CreateString((*protos)[idx].parameters[param_idx].name));
      cJSON_AddItemToObject(
          parameter, "type",
          cJSON_CreateString((*protos)[idx].parameters[param_idx].type));
      cJSON_AddItemToObject(
          parameter, "is_last",
          cJSON_CreateBool(param_idx == (*protos)[idx].parameter_count - 1));
    }
  }

  return root;
}
