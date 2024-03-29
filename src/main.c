#include "cgentest.h"
#include "cli.h"
#include "logger.h"

int main(int argc, char *argv[]) {
  struct arguments args = parse_args(argc, argv);

  // init logger in main level so we don't need to malloc and free
  struct log logger = {.level = args.log_level};
  init_logger(&logger);

  print_args(&args);

  generate_test(&args);

  free_args(&args);
  log_info("success generate tests\n");

  return 0;
}
