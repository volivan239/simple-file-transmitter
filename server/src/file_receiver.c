#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_receiver.h"

int receive_file(int fd, char **filename) {
    int filename_size, file_size;
    if (recv(fd, &filename_size, sizeof(int), 0) != sizeof(int))
        return 1;
    if (recv(fd, &file_size, sizeof(int), 0) != sizeof(int))
        return 1;

    *filename = malloc(filename_size + 2);
    if (recv(fd, *filename, filename_size + 1, 0) != filename_size + 1)
        return 1;

    char *buf = malloc(file_size + 2);
    if (recv(fd, buf, file_size, 0) != file_size) {
        free(buf);
        return 1;
    }

    FILE *file = fopen(*filename, "wb");
    fwrite(buf, file_size, 1, file);
    free(buf);
    fclose(file);
    return 0;
}