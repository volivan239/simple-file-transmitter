#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "file_sender.h"

int send_file(int fd, const char *filename, const char *serv_filename) {
    struct stat st;
    stat(filename, &st);
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
        return 1;
    }
    fclose(file);
    if (send(fd, buf, size, 0) != size) {
        return 2;
    }
    return 0;
}