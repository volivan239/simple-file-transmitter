#ifndef FILE_TRANSMITTER_SETTINGS_H
#define FILE_TRANSMITTER_SETTINGS_H

typedef struct Settings_t {
    int port;
    const char *dest_foldr;
} Settings;

int parse_settings(int argc, char *argv[], Settings *settings);

#endif //FILE_TRANSMITTER_SETTINGS_H
