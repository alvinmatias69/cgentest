#include "src/logger.h"

void init_logger(struct log *logger) {}
void vprint_log(const char *fmt, va_list args, enum log_level level) {}
void log_error(const char *message) {}
void log_errorf(const char *fmt, ...) {}
void log_warn(const char *message) {}
void log_warnf(const char *fmt, ...) {}
void log_info(const char *message) {}
void log_infof(const char *fmt, ...) {}
void log_debug(const char *message) {}
void log_debugf(const char *fmt, ...) {}
