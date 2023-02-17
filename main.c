#include "cgentest.h"
#include "cli.h"
#include "logger.h"

int main(int argc, char *argv[]) {
  struct arguments args = parse_args(argc, argv);
  /* print_args(&args); */

  struct log logger = {.level = args.log_level};
  init_logger(&logger);

  generate_test(&args);

  free_args(&args);
  log_info("success generate tests\n");

  return 0;
}
