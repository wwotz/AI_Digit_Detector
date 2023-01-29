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

typedef struct mnist_file_t {
        FILE *fd;
        mnist_header_t header;
} mnist_file_t;

mnist_file_t open_file_mnist(const char *file);
char *get_next_image_mnist(mnist_file_t mnist_file, char *buffer, size_t size);

#endif // MNIST_FORMAT_H_
