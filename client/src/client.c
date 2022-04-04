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