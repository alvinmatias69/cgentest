#include "src/util.h"

char *read_file(char *path) { return ""; }
bool name_in_list(struct metadata_list *list, char *query) { return false; }
void free_metadata(struct metadata *metadata) {}
void free_metadata_list(struct metadata_list *list, bool free_member) {}
void throw(const char *msg) {}
void throwf(const char *fmt, ...) {}
void *reallocarray_with_check(void *ptr, size_t nmemb, size_t size) {
  return reallocarray(ptr, nmemb, size);
}
void print_metadata_list(struct metadata_list *list) {}
char *strbool(bool var) { return ""; }
char *strloglvl(enum log_level level) { return ""; }
