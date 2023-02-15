#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

struct log_mapping {
  enum log_level level;
  const char *str;
};

void init_logger(struct log *log) { logger = log; }

const static struct log_mapping map[] = {
    {.level = ERROR, .str = "error"},
    {.level = WARN, .str = "warn"},
    {.level = INFO, .str = "info"},
    {.level = DEBUG, .str = "debug"},
};

enum log_level map_from_string(const char *str) {
  for (int idx = 0; idx < sizeof(map) / sizeof(map[0]); idx++) {
    if (strncmp(str, map[idx].str, 20) == 0) {
      return map[idx].level;
    }
  }
  return -1;
}

void print_log(const char *message, enum log_level level) {
  if (level <= logger->level) {
    printf("%s", message);
  }
}

void vprint_log(const char *fmt, va_list args, enum log_level level) {
  if (level <= logger->level) {
    vprintf(fmt, args);
  }
}

void log_error(const char *message) { print_log(message, ERROR); }

void log_errorf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, ERROR);
  va_end(args);
}

void log_warn(const char *message) { print_log(message, WARN); }

void log_warnf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, WARN);
  va_end(args);
}

void log_info(const char *message) { print_log(message, INFO); }

void log_infof(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, INFO);
  va_end(args);
}

void log_debug(const char *message) { print_log(message, DEBUG); }

void log_debugf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, DEBUG);
  va_end(args);
}
