#ifndef FILE_TRANSMITTER_SETTINGS_H
#define FILE_TRANSMITTER_SETTINGS_H

typedef struct Settings_t {
    int port;
    const char *host;
    const char *file_name;
} Settings;

int parse_settings(int argc, char *argv[], Settings *settings);

#endif //FILE_TRANSMITTER_SETTINGS_H
