#include "src/parser.c"
#include <stdio.h>
#include <stdlib.h>

void regex_match_test() {
  regex_t *simple;
  regcomp(simple, "simple", 0);

  struct {
    char name[100];
    struct {
      const char *string;
      regex_t *regex;
      char *pattern;
    } parameters;
    bool expected;
  } tests[] = {
      {.name = "Success match simple regex",
       .parameters = {.string = "simple", .regex = simple, .pattern = "simple"},
       .expected = true},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running regex_match_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    bool result =
        regex_match(tests[idx].parameters.string, tests[idx].parameters.regex,
                    tests[idx].parameters.pattern);
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
  regex_match_test();
  return 0;
}
