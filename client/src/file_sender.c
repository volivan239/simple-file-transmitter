#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include "file_sender.h"
#include "shared_constants.h"

int send_file(int fd, const char *filename, const char *serv_filename) {
    struct stat st;
    if (stat(filename, &st))
        return 1;
    int size = st.st_size;
    int filename_size = strlen(serv_filename);

    if (send(fd, &filename_size, sizeof(int), 0) != sizeof(int))
        return 2;
    if (send(fd, &size, sizeof(int), 0) != sizeof(int))
        return 2;
    if (send(fd, serv_filename, filename_size + 1, 0) != filename_size + 1)
        return 2;

    char *buf = malloc(size + 1);
    FILE *file = fopen(filename, "rb");
    if (!file || fread(buf, size, 1, file) != 1) {
        fclose(file);
        free(buf);
        return 1;
    }
    fclose(file);

    for (int pos = 0; pos < size; pos += BUFLEN) {
        int len = size - pos;
        if (len > BUFLEN)
            len = BUFLEN;
        if (send(fd, buf + pos, len, 0) != len) {
            free(buf);
            return 2;
        }
    }
    free(buf);
    return 0;
}