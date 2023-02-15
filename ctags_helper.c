#include "ctags_helper.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

void generate_tags(char *source, char *target, tagFile **tags) {
  char command[100];
  snprintf(command, sizeof command,
           "ctags --kinds-c=f --_xformat=\"%%N %%t %%S\" -o %s %s", target,
           source);
  log_debugf("ctags generate command: %s\n", command);

  FILE *fp = popen(command, "w");
  if (fp == NULL) {
    log_error("unable to generate ctags\n");
    exit(1);
  }
  pclose(fp);

  tagFileInfo info;
  *tags = tagsOpen(target, &info);
  log_debug("finish open tag\n");
  if (tags == NULL) {
    log_errorf("unable to read tags. Error number %d\n",
               info.status.error_number);
    exit(1);
  }
}
