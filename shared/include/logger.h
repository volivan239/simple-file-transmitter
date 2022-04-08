#ifndef FILE_TRANSMITTER_LOGGER_H
#define FILE_TRANSMITTER_LOGGER_H

typedef enum message_type_t {
    INFO, WARN, ERROR
} message_type;

void log(message_type t, const char *msg_fmt, ...);

#endif //FILE_TRANSMITTER_LOGGER_H
