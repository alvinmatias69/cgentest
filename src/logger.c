#include "logger.h"
#include <stdio.h>
#include <string.h>

// logger don't need structure? can be a simple enum
struct log *logger = NULL;
struct log default_logger = {.level = ERROR_LEVEL};

void init_logger(struct log *log) { logger = log; }

// maybe we don't need getter
struct log *get_logger() {
  return logger == NULL ? &default_logger : logger;
};

enum log_level map_from_level(const int level) {
  switch (level) {
  case 0:
    return ERROR_LEVEL;
  case 1:
    return WARN_LEVEL;
  case 2:
    return INFO_LEVEL;
  default:
    return DEBUG_LEVEL;
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

void log_error(const char *message) { print_log(message, ERROR_LEVEL); }

void log_errorf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, ERROR_LEVEL);
  va_end(args);
}

void log_warn(const char *message) { print_log(message, WARN_LEVEL); }

void log_warnf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, WARN_LEVEL);
  va_end(args);
}

void log_info(const char *message) { print_log(message, INFO_LEVEL); }

void log_infof(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, INFO_LEVEL);
  va_end(args);
}

void log_debug(const char *message) { print_log(message, DEBUG_LEVEL); }

void log_debugf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint_log(fmt, args, DEBUG_LEVEL);
  va_end(args);
}
