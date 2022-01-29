#ifndef BMP_PROCESSING_H
#define BMP_PROCESSING_H

#include "bmp_header.h"
#include "image.h"
#include "image_utils.h"

enum read_status  {
    READ_OK = 0,
    READ_INVALID_BITS,
    READ_INVALID_HEADER
    /* коды других ошибок  */
};

enum read_status from_bmp( FILE* in, struct image* img );


enum  write_status  {
    WRITE_OK = 0,
    WRITE_ERROR
    /* коды других ошибок  */
};

enum write_status to_bmp( FILE* out, struct image const* img );


enum open_status {
    OPEN_OK = 0,
    OPEN_ERROR = 1
};

enum open_status open_bmp( FILE** in, const char *filename, const char *modes);


enum close_status {
    CLOSE_OK = 0,
    CLOSE_ERROR = 1
};

enum close_status close_bmp( FILE** out);

enum read_status valid_header(struct bmp_header header);

#endif
