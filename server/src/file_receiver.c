#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_receiver.h"
#include "shared_constants.h"

int receive_file(int fd, char **filename) {
    int filename_size, file_size;
    if (recv(fd, &filename_size, sizeof(int), 0) != sizeof(int))
        return 1;
    if (recv(fd, &file_size, sizeof(int), 0) != sizeof(int))
        return 1;

    *filename = malloc(filename_size + 2);
    if (recv(fd, *filename, filename_size + 1, 0) != filename_size + 1)
        return 1;

    char buf[BUFLEN];
    FILE *file = fopen(*filename, "wb");
    for (int pos = 0; pos < file_size; pos += BUFLEN) {
        int len = file_size - pos;
        if (len > BUFLEN)
            len = BUFLEN;
        if (recv(fd, buf, len, 0) != len) {
            fclose(file);
            return 1;
        }
        fwrite(buf, len, 1, file);
    }

    fclose(file);
    return 0;
}