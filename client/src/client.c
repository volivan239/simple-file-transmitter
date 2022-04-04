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
    const char *host;
    const char *file_name;
} Settings;


void parse_settings(int argc, char *argv[], Settings *settings) {
    struct option long_options[] = {
            {"host", required_argument, 0,  'h' },
            {"port", required_argument, 0,  'p' },
    };

    int index = 0, opt;
    while ((opt = getopt_long(argc, argv,"h:p:", long_options, &index)) != -1) {
        switch (opt) {
            case 'p' :
                // TODO: use strtol
                settings->port = atoi(optarg);
                break;
            case 'h' :
                settings->host = optarg;
                break;
            default:
                // TODO: handle error
                break;
        }
    }
    if (optind != argc - 1) {
        // TODO: handle error
        exit(4);
    }
    settings->file_name = argv[optind];
}

int main(int argc, char *argv[]) {

    Settings settings = (Settings) {80, "127.0.0.1", ""};
    parse_settings(argc, argv, &settings);

    struct sockaddr_in serv_addr;
    inet_pton(AF_INET, settings.host, &serv_addr.sin_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(settings.port);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        // TODO: handle error
        return 1;
    }

    char buf[BUFLEN];
    while (1) {
        scanf("%s", buf);
        printf("%d %d\n", strlen(buf), send(fd, buf, strlen(buf) + 1, 0));
        if (!strcmp(buf, "end"))
            break;
    }
    close(fd);
}