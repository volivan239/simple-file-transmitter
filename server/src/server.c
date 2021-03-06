#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "settings.h"
#include "file_receiver.h"
#include "logger.h"
#define BACKLOG 8

void *handle_io(void *arg) {
    int *is_interrupted = (int*) arg;
    char buf[256];
    while (1) {
        scanf("%s", buf);
        if (!strcmp(buf, "shutdown")) {
            *is_interrupted = 1;
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {

    Settings settings = (Settings) {5679, "."};
    if (parse_settings(argc, argv, &settings))
        return -1;

    struct sockaddr_in addr;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(settings.port);

    const char *error_fmt = "Error while trying to launch server: %s returned -1 with errno=%d, terminating";

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        log_msg(ERROR, error_fmt, "socket()", errno);
        return 1;
    }

    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK); // SET socket to nonblocking so that server can react to I/O.
    if (bind(fd, (const struct sockaddr*) &addr, sizeof(addr)) == -1) {
        log_msg(ERROR, error_fmt, "bind()", errno);
        return 1;
    }

    if (listen(fd, BACKLOG) == -1) {
        log_msg(ERROR, error_fmt, "listen()", errno);
        return 1;
    }

    if (chdir(settings.dest_foldr)) {
        log_msg(ERROR, "Can't open directory %s, terminating", settings.dest_foldr);
        return 2;
    }

    log_msg(INFO, "Started listening on port %d", settings.port);

    int is_interrupted = 0;
    pthread_t io_thread;
    pthread_create(&io_thread, NULL, handle_io, &is_interrupted);

    struct sockaddr_in client_addr;
    unsigned int addrlen = sizeof(client_addr);
    char addr_ip[INET_ADDRSTRLEN];
    while (!is_interrupted) {
        int connfd = accept(fd, (struct sockaddr*) &client_addr, &addrlen);
        if (connfd == -1) {
            usleep(20000);
            continue;
        }

        int rc = fork();
        if (rc != 0)
            continue;

        fcntl(connfd, F_SETFL, fcntl(fd, F_GETFL, 0) & (~O_NONBLOCK));
        inet_ntop(AF_INET, &client_addr.sin_addr, addr_ip, INET_ADDRSTRLEN);
        char *filename = NULL;
        log_msg(INFO, "Incoming file from %s", addr_ip);
        if (receive_file(connfd, &filename)) {
            log_msg(WARN, "Receiving file from %s failed!", addr_ip);
        } else {
            log_msg(INFO, "Successfully received file %s from %s", filename, addr_ip);
        }
        free(filename);
        close(connfd);
        return 0;
    }
    log_msg(INFO, "Waiting for all transmissions to be finished");
    while (wait(NULL) > 0);
    close(fd);
    log_msg(INFO, "All transmissions finished, shutting down!");
    return 0;
}