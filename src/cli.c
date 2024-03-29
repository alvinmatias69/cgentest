#include "cli.h"
#include "config.h"
#include "local_limit.h"
#include "util.h"
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
    {"version", no_argument, NULL, 'V'},
    {"verbose", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"output", optional_argument, NULL, 'o'},
    {"force", no_argument, NULL, 'F'},
    {"template", optional_argument, NULL, 't'},
    {"exclude", optional_argument, NULL, 'e'},
    {"only", optional_argument, NULL, 'O'},
    {"bin", optional_argument, NULL, 'b'},
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
      .only = "",
      .has_only = false,
      .exclude = "",
      .has_exclude = false,
      .template_file = "",
      .log_level = WARN_LEVEL,
      .ctags_bin_path = "",
      .has_custom_ctags_bin = false,
  };
  struct required_args req = {0};

  int verbosity = 0;
  while ((opt = getopt_long(argc, argv, "Vvho:b:l:e:O:Ft:", long_options,
                            NULL)) != -1) {
    switch (opt) {
    case 'o':
      args.target = strndup(optarg, MAX_INPUT_LENGTH);
      args.custom_target = true;
      break;
    case 'O':
      args.only = strndup(optarg, MAX_INPUT_LENGTH);
      args.has_only = true;
      break;
    case 'e':
      args.exclude = strndup(optarg, MAX_INPUT_LENGTH);
      args.has_exclude = true;
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
    case 'V':
      print_version();
      exit(0);
      break;
    case 'v':
      verbosity++;
      break;
    case 'b':
      args.has_custom_ctags_bin = true;
      args.ctags_bin_path = strndup(optarg, MAX_INPUT_LENGTH);
      break;
    }
  }

  char **positionals = &argv[optind];
  // handle multiple inputs?
  for (; *positionals; positionals++) {
    args.input = strndup(*positionals, MAX_INPUT_LENGTH);
    req.input = true;
  }

  args.log_level = map_from_level(verbosity);
  validate_args(&req, &args);

  return args;
}

void free_args(struct arguments *args) {
  if (strnlen(args->input, MAX_INPUT_LENGTH) > 0)
    free(args->input);
  if (args->custom_target)
    free(args->target);
  if (args->has_only)
    free(args->only);
  if (args->has_exclude)
    free(args->exclude);
  if (strnlen(args->template_file, MAX_INPUT_LENGTH) > 0)
    free(args->template_file);
  if (args->has_custom_ctags_bin)
    free(args->ctags_bin_path);
}

void print_args(struct arguments *args) {
  log_debug("Argument list:\n");
  log_debugf("  input file           : %s\n", args->input);
  log_debugf("  use custom target    : %s\n", strbool(args->custom_target));
  log_debugf("  target file          : %s\n", args->target);
  log_debugf("  use custom template  : %s\n", strbool(args->custom_template));
  log_debugf("  template file        : %s\n", args->template_file);
  log_debugf("  has 'only' filter    : %s\n", strbool(args->has_only));
  log_debugf("  'only' filter        : %s\n", args->only);
  log_debugf("  has 'exclude' filter : %s\n", strbool(args->has_exclude));
  log_debugf("  'exclude' filter     : %s\n", args->exclude);
  log_debugf("  use custom ctags path: %s\n",
             strbool(args->has_custom_ctags_bin));
  log_debugf("  custom ctags path    : %s\n", args->ctags_bin_path);
  log_debugf("  force generate       : %s\n",
             strbool(args->ignore_target_current));
  log_debugf("  log level            : %s\n", strloglvl(args->log_level));
}

void print_help(void) {
  printf("%s\n"
         "\n"
         "Usage:\n"
         "\t%s [-o | --output <output_file>] [-O | --only "
         "<filter_only_regex>] [-e | --exclude <filter_exclude_regex>] [-t | "
         "--template <custom_template_path>] [-b | --bin "
         "<custom_ctags_bin_path>] [-F | --force] [-v | --verbose] "
         "INPUT_FILE\n"
         "\t%s -h | --help\n"
         "\t%s -V | --version\n"
         "\n"
         "Options:\n"
         "\t-o --output\t Path to generated test file. Will print to stdout on "
         "empty. [default: \"\"]\n"
         "\t-v --verbose\t Log verbosity, add more for more verbose log\n"
         "\t-O --only\t regexp. Generate tests for functions that match only. "
         "[default: \"\"]\n"
         "\t-e --exclude\t regexp. Exclude generate tests for function that "
         "match. Takes precedence over --only [default: \"\"]\n"
         "\t-b --bin\t Path to universal ctags binary [default: \"ctags\"]\n"
         "\t-F --force\t Force generate function even if generated function "
         "exists\n"
         "\t-t --template\t Path to custom mustache template [default: \"\"]\n"
         "\t-h --help\t Show this screen\n"
         "\t-V --version\t Show version\n",
         PACKAGE_NAME, PACKAGE_NAME, PACKAGE_NAME, PACKAGE_NAME);
}

void print_version(void) { printf("%s\n", PACKAGE_STRING); }

void validate_args(struct required_args *req, struct arguments *args) {
  if (!req->input) {
    printf("input file is required\n");
    print_help();
    exit(1);
  }
}
