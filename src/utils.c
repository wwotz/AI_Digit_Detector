#include "../include/utils.h"

/* loads a file into memory, returns the location in memory
   it has been loaded into. */
char *load_file(const char *restrict name)
{
        FILE *fd;
        char *buffer;
        size_t buffer_len;

        if (!name) {
                push_error_debug("File was null!");
                return NULL;
        }

        fd = fopen(name, "r");
        if (!fd) {
                const char *msg = "File does not exist: ";
                char *err = malloc(strlen(msg) + strlen(name) + 1);

                err = strcpy(err, msg);
                err = strcat(err, name);
                push_error_debug(err);
                free(err);
                return NULL;
        }

        fseek(fd, 0, SEEK_END);
        buffer_len = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        if (!(buffer = malloc(sizeof(*buffer) * (buffer_len + 1)))) {
                fprintf(stderr, "Fatal: ran out of memory!\n");
                exit(EXIT_FAILURE);
        }

        int nread;
        while ((nread = fread(buffer, sizeof(*buffer), buffer_len, fd)) != 0)
                ;

        buffer[buffer_len] = '\0';
        if (ferror(fd)) {
                const char *msg = "Failed to read file: ";
                char *err = malloc(strlen(msg) + strlen(name) + 1);

                err = strcpy(err, msg);
                err = strcat(err, name);
                push_error_debug(err);
                free(err);

                free(buffer);
                buffer = NULL;
        }

        fclose(fd);
        return buffer;
}

/* checks if @src ends with @end, returning 1 on success,
   and 0 otherwise */
int ends_with(const char *restrict src, const char *restrict end)
{
        size_t src_len, end_len;
        if (src == NULL || end == NULL)
                return 0;

        src_len = strlen(src);
        end_len = strlen(end);
        if (src_len >= end_len)
                return strncmp(src+(src_len-end_len), end, end_len) == 0;
        return 0;
}
