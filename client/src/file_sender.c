#include <stdio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "file_sender.h"

int send_file(int fd, const char *file_name) {
    struct stat st;
    stat(file_name, &st);
    int size = st.st_size;

    send(fd, &size, sizeof(int), 0);

    char *buf = malloc(size + 1);
    FILE *file = fopen(file_name, "rb");
    send(fd, buf, size, 0);

    fclose(file);
    return 0;
}