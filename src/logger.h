#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>

enum log_level {
  ERROR_LEVEL,
  WARN_LEVEL,
  INFO_LEVEL,
  DEBUG_LEVEL,
};

struct log {
  enum log_level level;
};

void init_logger(struct log *logger);

enum log_level map_from_level(const int level);

void vprint_log(const char *fmt, va_list args, enum log_level level);
void log_error(const char *message);
void log_errorf(const char *fmt, ...);
void log_warn(const char *message);
void log_warnf(const char *fmt, ...);
void log_info(const char *message);
void log_infof(const char *fmt, ...);
void log_debug(const char *message);
void log_debugf(const char *fmt, ...);

#endif // LOGGER_H_
