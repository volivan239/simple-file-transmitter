#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_receiver.h"

int receive_file(int fd) {
    int filename_size, file_size;
    recv(fd, &filename_size, sizeof(int), 0);
    recv(fd, &file_size, sizeof(int), 0);

    char *filename = malloc(filename_size + 2);
    recv(fd, filename, filename_size + 1, 0);
    printf("%s\n", filename);

    FILE *file = fopen(filename, "wb");
    char *buf = malloc(file_size + 2);
    recv(fd, buf, file_size, 0);
    fwrite(buf, file_size, 1, file);

    fclose(file);
    return 0;
}