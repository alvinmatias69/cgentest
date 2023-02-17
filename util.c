#include "util.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

void check_malloc(void *ptr) {
  if (ptr == NULL) {
    log_error("error while allocating memory.");
    exit(1);
  }
}

char *read_file(char *path) {
  FILE *f = fopen(path, "r");

  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *data = malloc(filesize + 1);
  check_malloc(data);
  fread(data, filesize, 1, f);
  fclose(f);

  data[filesize] = '\0';

  return data;
}
