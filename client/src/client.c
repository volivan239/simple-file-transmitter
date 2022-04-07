#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "settings.h"
#include "file_sender.h"

int main(int argc, char *argv[]) {

    Settings settings = (Settings) {5679, "127.0.0.1", "", ""};
    parse_settings(argc, argv, &settings);

    struct sockaddr_in serv_addr;
    inet_pton(AF_INET, settings.host, &serv_addr.sin_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(settings.port);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        // TODO: handle error
        return 1;
    }
    if (connect(fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        // TODO: handle error
        close(fd);
        return 2;
    }

    int code = send_file(fd, settings.file_name, settings.serv_file_name);
    switch (code) {
        case 0:
            printf("Successfully sent file to server!\n");
            break;
        case 1:
            printf("Error while reading file\n");
            break;
        default:
            printf("Error while sending file\n");
            break;
    }
    close(fd);
    if (code != 0)
        return 3;
    return 0;
}