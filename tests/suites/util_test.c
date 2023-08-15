#include "src/util.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strbool_test() {
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
      printf("Failure\n");
      fprintf(stderr, "Failed on %s:\n\tExpecting: %s\n\tGot: %s\n",
              tests[idx].name, tests[idx].expected, result);
    }
  }
}

void strloglvl_test() {
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
       .expected = "debugr"},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running strloglvl_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    char *result = strloglvl(tests[idx].parameters.level);
    if (strncmp(tests[idx].expected, result, 100) == 0) {
      printf("Success\n");
    } else {
      printf("Failure\n");
      fprintf(stderr, "Failed on %s:\n\tExpecting: %s\n\tGot: %s\n",
              tests[idx].name, tests[idx].expected, result);
    }
  }
}

int main(int argc, char *argv[]) {
  strbool_test();
  strloglvl_test();

  return 0;
}
