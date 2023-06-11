#ifndef LOGGER_H_
#define LOGGER_H_

enum log_level {
  ERROR,
  WARN,
  INFO,
  DEBUG,
};

struct log {
  enum log_level level;
};

void init_logger(struct log *logger);

enum log_level map_from_level(const int level);

void log_error(const char *message);
void log_errorf(const char *fmt, ...);
void log_warn(const char *message);
void log_warnf(const char *fmt, ...);
void log_info(const char *message);
void log_infof(const char *fmt, ...);
void log_debug(const char *message);
void log_debugf(const char *fmt, ...);

#endif // LOGGER_H_
