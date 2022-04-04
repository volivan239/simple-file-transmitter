#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_receiver.h"

int receive_file(int fd, const char *file_name) {
    int size;
    recv(fd, &size, sizeof(int), 0);
    printf("%d\n", size);

    char *buf = malloc(size + 1);
    recv(fd, buf, size, 0);

    FILE *file = fopen(file_name, "wb");
    fwrite(buf, size, 1, file);

    fclose(file);
}