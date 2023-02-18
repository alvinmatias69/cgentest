#ifndef CTAGS_HELPER_H_
#define CTAGS_HELPER_H_

#include "libs/libreadtags/readtags.h"
#include <stdlib.h>

void generate_tags(const char *source, const char *target, tagFile **tags);

#endif // CTAGS_HELPER_H_
