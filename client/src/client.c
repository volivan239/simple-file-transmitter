#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "settings.h"
#include "file_sender.h"
#include "logger.h"

int main(int argc, char *argv[]) {

    Settings settings = (Settings) {5679, "127.0.0.1", "", ""};
    if (parse_settings(argc, argv, &settings))
        return -1;

    struct sockaddr_in serv_addr;
    inet_pton(AF_INET, settings.host, &serv_addr.sin_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(settings.port);

    const char *error_fmt = "Error while trying to establish connection with server: %s returned -1 with errno=%d, terminating";

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        log_msg(ERROR, error_fmt, "socket()", errno);
        return 1;
    }
    if (connect(fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        log_msg(ERROR, error_fmt, "connect()", errno);
        close(fd);
        return 1;
    }

    log_msg(INFO, "Successfully connected to server");

    int code = send_file(fd, settings.file_name, settings.serv_file_name);
    switch (code) {
        case 0:
            log_msg(INFO, "Successfully sent file to server!");
            break;
        case 1:
            log_msg(INFO, "Error while reading file");
            break;
        default:
            log_msg(INFO, "Error while sending file");
            break;
    }
    close(fd);
    if (code != 0)
        return 2;
    return 0;
}