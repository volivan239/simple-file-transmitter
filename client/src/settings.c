#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/param.h>
#include "settings.h"

int parse_settings(int argc, char *argv[], Settings *settings) {
    struct option long_options[] = {
            {"host",     required_argument, 0,  'h' },
            {"port",     required_argument, 0,  'p' },
            {"filename", required_argument, 0,  'f' },
    };

    int index = 0, opt;
    while ((opt = getopt_long(argc, argv,"h:p:n:", long_options, &index)) != -1) {
        switch (opt) {
            case 'p':
                // TODO: use strtol
                settings->port = atoi(optarg);
                break;
            case 'h':
                settings->host = optarg;
                break;
            case 'f':
                strcpy(settings->serv_file_name, optarg);
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

    strcpy(settings->file_name, argv[optind]);
    if (!strcmp(settings->serv_file_name, ""))
        basename_r(settings->file_name, settings->serv_file_name);

    return 0;
}