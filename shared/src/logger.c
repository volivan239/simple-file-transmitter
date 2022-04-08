#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

static const char *message_type_names[] = {"INFO", "WARN", "ERROR"};

void log_msg(message_type t, const char *msg_fmt, ...) {
    time_t now;
    time(&now);


    char msg[256];
    va_list argptr;
    va_start(argptr, msg_fmt);
    vsprintf(msg, msg_fmt, argptr);
    va_end(argptr);

    printf("%s [%s]: %s\n", ctime(&now), message_type_names[t], msg);
}