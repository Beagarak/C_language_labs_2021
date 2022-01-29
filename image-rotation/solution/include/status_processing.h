#ifndef STATUS_PROCESSING_H
#define STATUS_PROCESSING_H

#include "bmp_processing.h"

const char* open_message(enum open_status status);

const char* close_message(enum close_status status);

const char* read_message(enum read_status status);

const char* write_message(enum write_status status);
#endif
