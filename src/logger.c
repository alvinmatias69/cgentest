#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

struct log *logger;

void init_logger(struct log *log) { logger = log; }

struct log *get_logger() {
  return logger;
};

enum log_level map_from_level(const int level) {
  switch (level) {
  case 0:
    return ERROR;
  case 1:
    return WARN;
  case 2:
    return INFO;
  default:
    return DEBUG;
  }
}

void print_log(const char *message, enum log_level level) {
  if (level <= get_logger()->level) {
    fprintf(stderr, "%s", message);
  }
}

void vprint_log(const char *fmt, va_list args, enum log_level level) {
  if (level <= get_logger()->level) {
    vfprintf(stderr, fmt, args);
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
