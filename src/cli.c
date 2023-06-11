#include "cli.h"
#include "config.h"
#include "local_limit.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct required_args {
  bool input;
};

void print_help(void);
void print_version(void);
void validate_args(struct required_args *req, struct arguments *args);

static struct option long_options[] = {
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"input", required_argument, NULL, 'i'},
    {"output", optional_argument, NULL, 'o'},
    {"log_level", optional_argument, NULL, 'l'},
    {"filter", optional_argument, NULL, 'f'},
    {"force", optional_argument, NULL, 'F'},
    {"template", optional_argument, NULL, 't'},
    {NULL, 0, NULL, 0},
};

struct arguments parse_args(int argc, char *argv[]) {
  int opt = -1;
  struct arguments args = {
      .custom_target = false,
      .custom_template = false,
      .ignore_target_current = false,
      .input = "",
      .target = "",
      .filter = "",
      .template_file = "",
      .log_level = WARN,
  };
  struct required_args req = {0};

  while ((opt = getopt_long(argc, argv, "vhi:o:l:f:Ft:", long_options, NULL)) !=
         -1) {
    switch (opt) {
    case 'i':
      args.input = strndup(optarg, MAX_INPUT_LENGTH);
      req.input = true;
      break;
    case 'o':
      args.target = strndup(optarg, MAX_INPUT_LENGTH);
      args.custom_target = true;
      break;
    case 'l':
      args.log_level = map_from_string(optarg);
      break;
    case 'f':
      args.filter = strndup(optarg, MAX_INPUT_LENGTH);
      break;
    case 'F':
      args.ignore_target_current = true;
      break;
    case 't':
      args.custom_template = true;
      args.template_file = strndup(optarg, MAX_INPUT_LENGTH);
      break;
    case 'h':
      print_help();
      exit(0);
      break;
    case 'v':
      print_version();
      exit(0);
      break;
    }
  }

  validate_args(&req, &args);

  return args;
}

void free_args(struct arguments *args) {
  if (strnlen(args->input, MAX_INPUT_LENGTH) > 0)
    free(args->input);
  if (strnlen(args->target, MAX_INPUT_LENGTH) > 0)
    free(args->target);
  if (strnlen(args->filter, MAX_INPUT_LENGTH) > 0)
    free(args->filter);
  if (strnlen(args->template_file, MAX_INPUT_LENGTH) > 0)
    free(args->template_file);
}

void print_args(struct arguments *args) {
  log_debug("Argument list:\n");
  log_debugf("\tinput file: %s\n", args->input);
  log_debugf("\ttarget file: %s\n", args->target);
  log_debugf("\tis custom target: %d\n", args->custom_target);
  log_debugf("\ttemplate file: %s\n", args->template_file);
  log_debugf("\tis custom template: %d\n", args->custom_template);
  log_debugf("\tfilter: %s\n", args->filter);
  log_debugf("\tlog level: %d\n", args->log_level);
}

void print_help(void) {
  printf(
      "cgentest\n"
      "\n"
      "Usage:\n"
      "\tcgentest -i <input_file> [-o <output_file>] [-l <log_level>] [-f "
      "<filter_query>] [--force]\n"
      "\tcgentest -h | --help\n"
      "\tcgentest -v | --version\n"
      "\n"
      "Options:\n"
      "\t-i --input\t Input file\n"
      "\t-o --output\t Generated test file [default: <input_file>_test.c]\n"
      "\t-l --log_level\t Log level (debug | info | warn | error [default])\n"
      "\t-f --filter\t Query filter for function [default: \"\"]\n"
      "\t-F --force\t Force generate function even if generated function "
      "exists\n"
      "\t-h --help\t Show this screen\n"
      "\t-v --version\t Show version\n");
}

void print_version(void) { printf("%s\n", PACKAGE_STRING); }

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