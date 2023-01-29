#ifndef MNIST_FORMAT_H_
#define MNIST_FORMAT_H_

#include "debug.h"
#include <stdio.h>

typedef struct mnist_header_t {
        unsigned int magic;
        unsigned int image_count;
        unsigned int row_count;
        unsigned int col_count;
} mnist_header_t;

mnist_header_t read_header_mnist(const char *file);

#endif // MNIST_FORMAT_H_
