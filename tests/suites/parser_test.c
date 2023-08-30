#include "helper.c"
#include "src/parser.c"
#include <stdio.h>
#include <stdlib.h>

struct metadata *init_empty(void) {
  struct metadata *result = reallocarray(NULL, sizeof(struct metadata), 1);
  result->name = NULL;
  result->return_type.name = NULL;
  result->return_type.is_primitive = false;
  result->return_type.is_void = false;
  result->parameter_count = 0;
  result->params = NULL;
  return result;
}

void parse_return_type_test(void) {
  struct {
    char name[100];
    struct {
      const char *typeref;
      struct metadata *metadata;
    } parameters;
    struct metadata expected;
  } tests[] = {
      {.name = "Success parse primitive",
       .parameters = {.typeref = "typename:bool",
                      .metadata =
                          reallocarray(NULL, sizeof(struct metadata), 1)},
       .expected = {.return_type = {.name = "bool",
                                    .is_primitive = true,
                                    .is_void = false}}},

      {.name = "Success parse void",
       .parameters = {.typeref = "typename:void",
                      .metadata =
                          reallocarray(NULL, sizeof(struct metadata), 1)},
       .expected = {.return_type = {.name = "void",
                                    .is_primitive = true,
                                    .is_void = true}}},

      {.name = "Success parse struct",
       .parameters = {.typeref = "struct:example *",
                      .metadata =
                          reallocarray(NULL, sizeof(struct metadata), 1)},
       .expected = {.return_type = {.name = "struct example *",
                                    .is_primitive = false,
                                    .is_void = false}}},

      {.name = "Success parse enum",
       .parameters = {.typeref = "enum:example",
                      .metadata =
                          reallocarray(NULL, sizeof(struct metadata), 1)},
       .expected = {.return_type = {.name = "enum example",
                                    .is_primitive = false,
                                    .is_void = false}}},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running parse_return_type_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    parse_return_type(tests[idx].parameters.typeref,
                      tests[idx].parameters.metadata);

    if (metadataiseq(tests[idx].parameters.metadata, &tests[idx].expected)) {
      printf("Success\n");
      free(tests[idx].parameters.metadata);
      continue;
    }

    printf("Failed\n");
    fprintf(stderr, "Failed on %s:\n", tests[idx].name);

    char *expectedstr = metadatafmt(&tests[idx].expected);
    fprintf(stderr, "\tExpecting: \n%s\n", expectedstr);
    free(expectedstr);

    char *gotstr = metadatafmt(tests[idx].parameters.metadata);
    fprintf(stderr, "\tGot: \n%s\n", gotstr);
    free(gotstr);

    free(tests[idx].parameters.metadata);
  }
}

void parse_parameters_test(void) {
  struct {
    char name[100];
    struct {
      const char *parameters;
      struct metadata *metadata;
    } parameters;
    struct metadata expected;
  } tests[] = {
      {.name = "Success parse empty parameters",
       .parameters = {.parameters = "()", .metadata = init_empty()},
       .expected = {.name = NULL, .parameter_count = 0}},

      {.name = "Success parse void parameters",
       .parameters = {.parameters = "(void)", .metadata = init_empty()},
       .expected = {.name = NULL, .parameter_count = 0}},

      {.name = "Success parse single parameter",
       .parameters = {.parameters = "(bool is_active)",
                      .metadata = init_empty()},
       .expected = {.name = NULL,
                    .parameter_count = 1,
                    .params =
                        (struct parameter[]){
                            {.name = "is_active", .type = "bool"},
                        }}},

      {.name = "Success parse multiple parameter",
       .parameters = {.parameters = "(bool is_active,char * path)",
                      .metadata = init_empty()},
       .expected = {.name = NULL,
                    .parameter_count = 2,
                    .params =
                        (struct parameter[]){
                            {.name = "is_active", .type = "bool"},
                            {.name = "path", .type = "char *"},
                        }}},

      {.name = "Success parse multiple parameter with variadic",
       .parameters = {.parameters = "(bool is_active,char * fmt,...)",
                      .metadata = init_empty()},
       .expected = {.name = NULL,
                    .parameter_count = 2,
                    .params =
                        (struct parameter[]){
                            {.name = "is_active", .type = "bool"},
                            {.name = "fmt", .type = "char *"},
                        }}},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running parse_parameters_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    parse_parameters(tests[idx].parameters.parameters,
                     tests[idx].parameters.metadata);

    if (metadataiseq(tests[idx].parameters.metadata, &tests[idx].expected)) {
      printf("Success\n");
      free(tests[idx].parameters.metadata);
      continue;
    }

    printf("Failed\n");
    fprintf(stderr, "Failed on %s:\n", tests[idx].name);

    char *expectedstr = metadatafmt(&tests[idx].expected);
    fprintf(stderr, "\tExpecting: \n%s\n", expectedstr);
    free(expectedstr);

    char *gotstr = metadatafmt(tests[idx].parameters.metadata);
    fprintf(stderr, "\tGot: \n%s\n", gotstr);
    free(gotstr);

    free(tests[idx].parameters.metadata);
  }
}

int main(void) {
  parse_return_type_test();
  parse_parameters_test();
  return 0;
}
