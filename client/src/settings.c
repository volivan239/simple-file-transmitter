#include <getopt.h>
#include <stdlib.h>
#include "settings.h"

int parse_settings(int argc, char *argv[], Settings *settings) {
    struct option long_options[] = {
            {"host", required_argument, 0,  'h' },
            {"port", required_argument, 0,  'p' },
    };

    int index = 0, opt;
    while ((opt = getopt_long(argc, argv,"h:p:", long_options, &index)) != -1) {
        switch (opt) {
            case 'p':
                // TODO: use strtol
                settings->port = atoi(optarg);
                break;
            case 'h':
                settings->host = optarg;
                break;
            default:
                // TODO: handle error
                break;
        }
    }
    if (optind != argc - 1) {
        // TODO: handle error
        return -1;
    }
    settings->file_name = argv[optind];
}