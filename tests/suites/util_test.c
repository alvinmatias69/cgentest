#include "src/util.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strbool_test(void) {
  struct {
    char name[100];
    struct {
      bool var;
    } parameters;
    char *expected;
  } tests[] = {
      {.name = "Success parse true",
       .parameters = {.var = true},
       .expected = "true"},

      {.name = "Success parse false",
       .parameters = {.var = false},
       .expected = "false"},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running strbool_tests\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    char *result = strbool(tests[idx].parameters.var);
    if (strncmp(tests[idx].expected, result, 100) == 0) {
      printf("Success\n");
    } else {
      printf("Failed\n");
      fprintf(stderr, "Failed on %s:\n\tExpecting: %s\n\tGot: %s\n",
              tests[idx].name, tests[idx].expected, result);
    }
  }
}

void strloglvl_test(void) {
  struct {
    char name[100];
    struct {
      enum log_level level;
    } parameters;
    char *expected;
  } tests[] = {
      {.name = "Success parse level ERROR",
       .parameters = {.level = ERROR_LEVEL},
       .expected = "error"},

      {.name = "Success parse level WARN",
       .parameters = {.level = WARN_LEVEL},
       .expected = "warn"},

      {.name = "Success parse level INFO",
       .parameters = {.level = INFO_LEVEL},
       .expected = "info"},

      {.name = "Success parse level DEBUG",
       .parameters = {.level = DEBUG_LEVEL},
       .expected = "debug"},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running strloglvl_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    char *result = strloglvl(tests[idx].parameters.level);
    if (strncmp(tests[idx].expected, result, 100) == 0) {
      printf("Success\n");
    } else {
      printf("Failed\n");
      fprintf(stderr, "Failed on %s:\n\tExpecting: %s\n\tGot: %s\n",
              tests[idx].name, tests[idx].expected, result);
    }
  }
}

void name_in_list_test(void) {
  struct metadata member = {.name = "needle"};
  struct metadata_list list = {.list = &member, .count = 1};

  struct {
    char name[100];
    struct {
      struct metadata_list *list;
      char *query;
    } parameters;
    bool expected;
  } tests[] = {
      {.name = "Success on available name",
       .parameters = {.list = &list, .query = "needle"},
       .expected = true},
      {.name = "Failed on unavailable name",
       .parameters = {.list = &list, .query = "needl"},
       .expected = false},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running name_in_list_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    bool result =
        name_in_list(tests[idx].parameters.list, tests[idx].parameters.query);
    if (tests[idx].expected == result) {
      printf("Success\n");
    } else {
      printf("Failed\n");
      fprintf(stderr, "Failed on %s:\n\tExpecting: %s\n\tGot: %s\n",
              tests[idx].name, strbool(tests[idx].expected), strbool(result));
    }
  }
}

void read_file_test(void) {
  char path[100];
  snprintf(&path, 100, "%s/util_read_test", FIXTURE_PATH);
  struct {
    char name[100];
    struct {
      char *path;
    } parameters;
    char *expected;
  } tests[] = {
      {.name = "Success read file",
       .parameters = {.path = path},
       .expected = "success read\n"},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running read_file_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    char *result = read_file(tests[idx].parameters.path);
    if (strncmp(tests[idx].expected, result, 100) == 0) {
      printf("Success\n");
    } else {
      printf("Failed\n");
      fprintf(stderr, "Failed on %s:\n\tExpecting: %s\n\tGot: %s\n",
              tests[idx].name, tests[idx].expected, result);
    }
  }
}

int main(void) {
  strbool_test();
  strloglvl_test();
  name_in_list_test();
  read_file_test();

  return 0;
}
