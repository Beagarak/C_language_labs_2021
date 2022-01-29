#include "../include/bmp_header.h"
#include "../include/image.h"
#include "../include/image_utils.h"
#include "../include/status_processing.h"

struct bmp_header make_bmp_header(struct image const* image) {
    struct bmp_header header = {
            .bfType = BF_TYPE,
            .bfileSize = sizeof(struct bmp_header) + image->height * get_padding(image->width),
            .bfReserved = BF_RESERVED,
            .bOffBits = sizeof(struct bmp_header),
            .biSize = BI_SIZE,
            .biWidth = image->width,
            .biHeight = image->height,
            .biPlanes = BI_PLANES,
            .biBitCount = BI_BIT_COUNT,
            .biCompression = BI_COMPRESSION,
            .biSizeImage = image->height * (image->width + get_padding(image->width)),
            .biXPelsPerMeter = BI_X_PELS_PER_METER,
            .biYPelsPerMeter = BI_Y_PELS_PER_METER,
            .biClrUsed = BI_CLR_USED,
            .biClrImportant = BI_CLR_IMPORTANT
    };
    return  header;
}
