#ifndef MNIST_FORMAT_H_
#define MNIST_FORMAT_H_

typedef struct mnist_header_t {
        uint32_t magic;
        uint32_t image_count;
        uint32_t row_count;
        uint32_t col_count;
} mnist_header_t;

#endif // MNIST_FORMAT_H_
