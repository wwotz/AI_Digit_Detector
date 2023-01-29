#include "../include/mnist_format.h"

#include <arpa/inet.h>

mnist_header_t read_header_mnist(const char *file)
{
        #define MNIST_HEADER_BUFFER_SIZE 4
        mnist_header_t header;
        unsigned int buffer[MNIST_HEADER_BUFFER_SIZE];
        FILE *fd;

        fd = fopen(file, "rb");
        if (!fd) {
                push_error_debug("MNIST file does not exist!");
                return header;
        }

        fread(buffer, sizeof(*buffer), sizeof(buffer)/sizeof(*buffer), fd);
        if (ferror(fd)) {
                push_error_debug("MNIST file had an error whilst reading!");
                return header;
        }

        //convert from little endian to big endian
        header.magic = htonl(buffer[0]);
        header.image_count = htonl(buffer[1]);
        header.row_count = htonl(buffer[2]);
        header.col_count = htonl(buffer[3]);

        #undef MNIST_HEADER_BUFFER_SIZE
        return header;
}
