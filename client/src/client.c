#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "settings.h"
#include "file_sender.h"

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

    send_file(fd, settings.file_name);
    close(fd);
}