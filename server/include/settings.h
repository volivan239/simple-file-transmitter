#ifndef FILE_TRANSMITTER_SETTINGS_H
#define FILE_TRANSMITTER_SETTINGS_H

#include <sys/param.h>

typedef struct Settings_t {
    int port;
    char dest_foldr[MAXPATHLEN];
} Settings;

int parse_settings(int argc, char *argv[], Settings *settings);

#endif //FILE_TRANSMITTER_SETTINGS_H
