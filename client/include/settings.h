#ifndef FILE_TRANSMITTER_SETTINGS_H
#define FILE_TRANSMITTER_SETTINGS_H

#include <sys/param.h>

typedef struct Settings_t {
    int port;
    const char *host;
    char file_name[MAXPATHLEN];
    char serv_file_name[MAXPATHLEN];
} Settings;

int parse_settings(int argc, char *argv[], Settings *settings);

#endif //FILE_TRANSMITTER_SETTINGS_H
