#include "../include/status_processing.h"


const char* open_status_messages[] = {
        [OPEN_OK] = "File opened successfully",
        [OPEN_ERROR] = "Error opening the file",
};

const char* open_message(enum open_status status) {
    return open_status_messages[status];
}



const char* close_status_messages[] = {
        [CLOSE_OK] = "File closed successfully",
        [CLOSE_ERROR] = "Error closing the file",
};

const char* close_message(enum close_status status) {
    return close_status_messages[status];
}



static const char *const read_status_messages[] = {
        [READ_OK] = "File has been read successfully",
        [READ_INVALID_BITS] = "Incorrect pixel size",
        [READ_INVALID_HEADER] = "Incorrect file header",
};

const char* read_message(enum read_status status) {
    return read_status_messages[status];
}



static const char *const write_status_messages[] = {
        [WRITE_OK] = "File was successfully written",
        [WRITE_ERROR] = "Error writing file"
};

const char* write_message(enum write_status status) {
    return write_status_messages[status];
}
