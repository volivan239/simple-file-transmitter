#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "header.h"

typedef struct Settings_t {
    int port;
    const char *dest_foldr;
} Settings;


void parse_settings(int argc, char *argv[], Settings *settings) {
    struct option long_options[] = {
            {"port",      required_argument, 0,  'p' },
            {"folder",    required_argument, 0,  'f' },
    };

    int index = 0, opt;
    while ((opt = getopt_long(argc, argv,"p:f:", long_options, &index)) != -1) {
        switch (opt) {
            case 'p' :
                // TODO: use strtol
                settings->port = atoi(optarg);
                break;
            case 'b' :
                settings->dest_foldr = optarg;
                break;
            default:
                // TODO: handle error
                break;
        }
    }
}

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