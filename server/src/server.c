#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "settings.h"
#include "file_receiver.h"
#include "logger.h"
#define BACKLOG 8

int main(int argc, char *argv[]) {

    Settings settings = (Settings) {5679, "."};
    if (parse_settings(argc, argv, &settings))
        return -1;

    struct sockaddr_in addr;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(settings.port);

    const char *error_fmt = "Error while trying to launch server: %s returned -1 with errno=%d, terminating\n";

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        log_msg(ERROR, error_fmt, "socket()", errno);
        return 1;
    }

    if (bind(fd, (const struct sockaddr*) &addr, sizeof(addr)) == -1) {
        log_msg(ERROR, error_fmt, "bind()", errno);
        return 1;
    }

    if (listen(fd, BACKLOG) == -1) {
        log_msg(ERROR, error_fmt, "listen()", errno);
        return 1;
    }

    if (chdir(settings.dest_foldr)) {
        log_msg(ERROR, "Can't open directory %s, terminating\n", settings.dest_foldr);
        return 2;
    }

    log_msg(INFO, "Stared listening on port %d", settings.port);
    struct sockaddr_in client_addr;
    unsigned int addrlen = sizeof(client_addr);
    char addr_ip[INET_ADDRSTRLEN];
    while (1) {
        int connfd = accept(fd, (struct sockaddr*) &client_addr, &addrlen);
        if (connfd == -1)
            continue;

        inet_ntop(AF_INET, &client_addr.sin_addr, addr_ip, INET_ADDRSTRLEN);
        char *filename = NULL;
        if (receive_file(connfd, &filename)) {
            log_msg(WARN, "Receiving file from %s failed!\n", addr_ip);
        } else {
            log_msg(INFO, "Successfully received file %s from %s\n", filename, addr_ip);
        }
        free(filename);
        close(connfd);
    }
    close(fd);
    return 0;
}