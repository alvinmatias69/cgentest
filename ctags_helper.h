#ifndef CTAGS_HELPER_H_
#define CTAGS_HELPER_H_

#include "libs/readtags/readtags.h"
#include <stdlib.h>

void generate_tags(char *source, char *target, tagFile **tags);

#endif // CTAGS_HELPER_H_
