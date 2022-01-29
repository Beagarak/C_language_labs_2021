#include "../include/bmp_processing.h"

enum open_status open_bmp(FILE **in, const char *filename, const char *modes){
    *in = fopen(filename, modes);
    if (*in) {return OPEN_OK;}
    return OPEN_ERROR;
}

enum close_status close_bmp(FILE **out){
    if (fclose(*out) != EOF) {return CLOSE_OK;}
    return CLOSE_ERROR;
}

enum read_status valid_header(struct bmp_header header){
    if (header.bfType != BF_TYPE || header.biBitCount != BI_BIT_COUNT 
    || header.biSize != BI_SIZE || header.bOffBits != 54) return READ_INVALID_HEADER;
    return READ_OK;
}

enum read_status from_bmp(FILE* in, struct image* img) {
    struct bmp_header header = {0};
    if(!fread(&header, sizeof(struct bmp_header), 1, in)) return READ_INVALID_HEADER;
    if (valid_header(header) == READ_OK) {
        if (create_image(img,header)){
            uint8_t padding = get_padding(img->width);
            size_t row_counter;
            for (size_t height = 0; height < img->height; height++) {
                row_counter = fread(&(img->data[height * img->width]), sizeof(struct pixel), img->width, in);
                uint64_t check = fseek(in, padding, SEEK_CUR);
                if (check != 0 || img->width != row_counter) return READ_INVALID_BITS;
            }
        }
    }
    return READ_OK;

}

enum write_status to_bmp(FILE* out, struct image const* img) {
    const uint8_t padding = get_padding(img->width);
    struct bmp_header header = make_bmp_header(img);
    uint32_t const pad = 0;
    fwrite(&header, sizeof(struct bmp_header), 1, out);
    for (size_t height = 0; height < img->height; height++) {
        for (size_t width = 0; width < img->width; width++){
            fwrite(img->data + (img->width * height + width), sizeof(struct pixel), 1, out);
        }
        fwrite(&pad, sizeof(uint8_t), padding, out);
    }
    return WRITE_OK;
}

