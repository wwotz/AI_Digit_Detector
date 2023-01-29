#include "../include/mnist_format.h"
#include <arpa/inet.h>
#include <stdlib.h>

/* reads the header into the mnist_header_t structure and returns it */
static mnist_header_t read_header_mnist(FILE *fd)
{
        #define MNIST_HEADER_BUFFER_SIZE 4
        mnist_header_t header;
        unsigned int buffer[MNIST_HEADER_BUFFER_SIZE];

        if (!fd) {
                push_error_debug("Invalid File!");
                return header;
        }

        fread(buffer, sizeof(*buffer), sizeof(buffer)/sizeof(*buffer), fd);
        if (ferror(fd)) {
                push_error_debug("MNIST file had an error whilst reading!");
                return header;
        }

        //converts to correct endianess of the system
        header.magic = ntohl(buffer[0]);
        header.image_count = ntohl(buffer[1]);
        header.row_count = ntohl(buffer[2]);
        header.col_count = ntohl(buffer[3]);

        #undef MNIST_HEADER_BUFFER_SIZE
        return header;
}

/* returns a mnist_file_t structure, this contains the
   file descriptor corresponding to the open mnist file,
   as well as the header of the file. The header contains
   the magic number which helps determine what type of
   data is inside the mnist file, as well as the number of
   images, row count and column count */
mnist_file_t open_file_mnist(const char *file)
{
        mnist_file_t mnist_file;
        FILE *fd;

        fd = fopen(file, "rb");
        if (!fd) {
                push_error_debug("MNIST file does not exist!");
                return mnist_file;
        }

        mnist_file.fd = fd;
        mnist_file.header = read_header_mnist(fd);
        return mnist_file;
}

/* returns the next image in the mnist file, otherwise NULL if no
   such image exists */
char *get_next_image_mnist(mnist_file_t mnist_file, char *buffer, size_t size)
{
        mnist_header_t mnist_header;
        mnist_header = mnist_file.header;

        if (size <= mnist_header.row_count * mnist_header.col_count) {
                size = mnist_header.row_count * mnist_header.col_count;
                buffer = realloc(buffer, size);
        }

        if (!buffer)
                buffer = malloc(size);

        int total_read = 0, nread = 0;
        while (total_read != mnist_header.row_count * mnist_header.col_count) {
                nread = fread(buffer, sizeof(*buffer), mnist_header.row_count * mnist_header.col_count, mnist_file.fd);
                if (nread == 0) {
                        if (feof(mnist_file.fd)) {
                                free(buffer);
                                return NULL;
                        } else {
                                push_error_debug("There was an error reading the file!\n");
                                free(buffer);
                                return NULL;
                        }
                        break;
                }
                total_read += nread;
        }
        return buffer;
}
