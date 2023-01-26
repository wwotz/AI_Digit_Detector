#ifndef UTILS_H_
#define UTILS_H_

#include "debug.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *load_file(const char *restrict name);
int ends_with(const char *restrict src, const char *restrict end);

#endif // UTILS_H_
