#define free_metadata mock_free_metadata
#define free_metadata_list mock_free_metadata_list

#include "helper.c"
#include "src/cgentest.c"
#include <stdio.h>
#include <stdlib.h>

void mock_free_metadata(struct metadata *metadata) {}
void mock_free_metadata_list(struct metadata_list *list, bool free_member) {}

void filter_test(void) {
  struct {
    char name[100];
    struct {
      struct metadata_list *source;
      struct metadata_list *target;
    } parameters;
    struct metadata_list *expected;
  } tests[] = {
      {.name = "Success on empty both list",
       .parameters = {.source =
                          &(struct metadata_list){.count = 0, .list = NULL},
                      .target =
                          &(struct metadata_list){.count = 0, .list = NULL}},
       .expected = &(struct metadata_list){.count = 0, .list = NULL}},

      {.name = "Success on empty target list",
       .parameters =
           {.source =
                &(struct metadata_list){
                    .count = 1,
                    .list =
                        (struct metadata[]){
                            {.name = "first", .return_type = {.name = ""}},
                        }},
            .target = &(struct metadata_list){.count = 0, .list = NULL}},
       .expected =
           &(struct metadata_list){
               .count = 1,
               .list =
                   (struct metadata[]){
                       {.name = "first", .return_type = {.name = ""}},
                   }}},

      {.name = "Success on found item",
       .parameters =
           {.source =
                &(struct metadata_list){.count = 2,
                                        .list =
                                            (struct metadata[]){
                                                {.name = "first",
                                                 .return_type = {.name = ""}},
                                                {.name = "second",
                                                 .return_type = {.name = ""}},
                                            }},
            .target =
                &(struct metadata_list){
                    .count = 1,
                    .list = (struct metadata[]){{.name = "first_test"}}}},
       .expected =
           &(struct metadata_list){.count = 1,
                                   .list =
                                       (struct metadata[]){
                                           {.name = "second",
                                            .return_type = {.name = ""}},
                                       }}},

      {.name = "Success on not found item",
       .parameters =
           {.source =
                &(struct metadata_list){.count = 2,
                                        .list =
                                            (struct metadata[]){
                                                {.name = "first",
                                                 .return_type = {.name = ""}},
                                                {.name = "second",
                                                 .return_type = {.name = ""}},
                                            }},
            .target =
                &(struct metadata_list){
                    .count = 1,
                    .list = (struct metadata[]){{.name = "third_test"}}}},
       .expected =
           &(struct metadata_list){.count = 2,
                                   .list =
                                       (struct metadata[]){
                                           {.name = "first",
                                            .return_type = {.name = ""}},
                                           {.name = "second",
                                            .return_type = {.name = ""}},
                                       }}},
  };

  size_t length = sizeof(tests) / sizeof(tests[0]);
  printf("Running filter_test\n");
  for (size_t idx = 0; idx < length; idx++) {
    printf(" %s: \t", tests[idx].name);
    struct metadata_list *result =
        filter(tests[idx].parameters.source, tests[idx].parameters.target);
    if (metadatalistiseq(tests[idx].expected, result)) {
      printf("Success\n");
    } else {
      printf("Failed\n");
      fprintf(stderr, "Failed on %s:\n", tests[idx].name);

      char *expectedstr = metadatalistfmt(tests[idx].expected);
      fprintf(stderr, "\tExpecting: \n%s\n", expectedstr);
      free(expectedstr);

      char *gotstr = metadatalistfmt(result);
      fprintf(stderr, "\tGot: \n%s\n", gotstr);
      free(gotstr);
    }
  }
}

int main(void) {
  filter_test();
  return 0;
}
