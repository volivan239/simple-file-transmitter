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

    send(fd, &filename_size, sizeof(int), 0);
    send(fd, &size, sizeof(int), 0);
    send(fd, serv_filename, filename_size + 1, 0);

    char *buf = malloc(size + 1);
    FILE *file = fopen(filename, "rb");
    fread(buf, size, 1, file);
    send(fd, buf, size, 0);

    fclose(file);
    return 0;
}