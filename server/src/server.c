#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "header.h"
#include "settings.h"

int main(int argc, char *argv[]) {

    Settings settings = (Settings) {80, "."};
    parse_settings(argc, argv, &settings);

    struct sockaddr_in addr;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(settings.port);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        // TODO: handle error
        return 1;
    }

    if (bind(fd, (const struct sockaddr*) &addr, sizeof(addr)) == -1) {
        // TODO: handle error
        return 2;
    }

    if (listen(fd, 8) == -1) { // TODO: 8 to constant
        // TODO: handle error
        return 3;
    }

    while (1) {
        struct sockaddr_in client_addr;
        unsigned int addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr*) &client_addr, &addrlen);
        if (connfd == -1) {
            // TODO: handle error
            continue;
        }
        char msg[BUFLEN];
        printf("Accepted connection\n");
        while (1) {
            ssize_t n = recv(connfd, msg, BUFLEN, 0);
            printf("%zd bytes got\n", n);
            printf("Received message %s from port %d\n", msg, ntohs(client_addr.sin_port));
            if (!strcmp(msg, "end"))
                break;
        }
        close(connfd);
    }
    close(fd);
    return 0;
}