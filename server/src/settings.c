#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "logger.h"

int parse_settings(int argc, char *argv[], Settings *settings) {
    struct option long_options[] = {
            {"port",      required_argument, 0,  'p' },
            {"folder",    required_argument, 0,  'f' },
    };

    int index = 0, opt;
    while ((opt = getopt_long(argc, argv,"p:f:", long_options, &index)) != -1) {
        switch (opt) {
            case 'p':
                // TODO: use strtol
                settings->port = atoi(optarg);
                break;
            case 'f':
                strcpy(settings->dest_foldr, optarg);
                break;
            default:
                log(ERROR, "Unexpected argument: %s, skipping", opt);
                return -1;
        }
    }
    return 0;
}