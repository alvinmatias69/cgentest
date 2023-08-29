#include "src/logger.c"
#include <stdio.h>
#include <stdlib.h>

void map_from_level_test() {
  struct {
    char name[100];
    struct {
      const int level;
    } parameters;
    enum log_level expected;
  } tests[] = {
      {.name = "Success map to ERROR",
       .parameters = {.level = 0},
       .expected = ERROR_LEVEL},

      {.name = "Success map to WARN",
       .parameters = {.level = 1},
       .expected = WARN_LEVEL},

      {.name = "Success map to INFO",
       .parameters = {.level = 2},
       .expected = INFO_LEVEL},

      {.name = "Success map to DEBUG",
       .parameters = {.level = 3},
       .expected = DEBUG_LEVEL},

      {.name = "Success default to DEBUG",
       .parameters = {.level = 69},
       .expected = DEBUG_LEVEL},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running map_from_level_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    enum log_level result = map_from_level(tests[idx].parameters.level);
    if (tests[idx].expected == result) {
      printf("Success\n");
    } else {
      printf("Failed\n");
      fprintf(stderr, "Failed on %s:\n\tExpecting: %d\n\tGot: %d\n",
              tests[idx].name, tests[idx].expected, result);
    }
  }
}

int main(void) {
  map_from_level_test();
  return 0;
}
