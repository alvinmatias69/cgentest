#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct required_args {
  bool input;
};

void print_help();
void validate_args(struct required_args *req, struct arguments *args);

struct arguments parse_args(int argc, char *argv[]) {
  int opt = -1;
  struct arguments args = {
      .custom_target = false,
      .input = "",
      .target = "",
      .filter = "",
      .log_level = WARN,
  };
  struct required_args req = {0};

  while ((opt = getopt(argc, argv, "hi:o:l:f:")) != -1) {
    switch (opt) {
    case 'i':
      args.input = strndup(optarg, 100);
      req.input = true;
      break;
    case 'o':
      args.target = strndup(optarg, 100);
      args.custom_target = true;
      break;
    case 'l':
      args.log_level = map_from_string(optarg);
      break;
    case 'f':
      args.filter = strndup(optarg, 100);
      break;
    case 'h':
      print_help();
      exit(0);
      break;
    }
  }

  validate_args(&req, &args);

  return args;
}

void print_args(struct arguments *args) {
  printf("Argument list:\n");
  printf("input file: %s\n", args->input);
  printf("target file: %s\n", args->target);
  printf("is custom target: %d\n", args->custom_target);
  printf("filter: %s\n", args->filter);
  printf("log level: %d\n", args->log_level);
}

void print_help() { printf("USAGE\n"); }

void validate_args(struct required_args *req, struct arguments *args) {
  if (!req->input) {
    printf("input file is required\n");
    exit(1);
  }

  if (args->log_level == -1) {
    printf("invalid log level\n");
    exit(1);
  }
}
