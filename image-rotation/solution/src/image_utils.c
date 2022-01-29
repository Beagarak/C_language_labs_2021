#include "../include/image_utils.h"

struct image create_rotated_blank( const uint64_t new_width, const uint64_t new_height) {
    return (struct image) {
        .width = new_width,
        .height = new_height,
        .data = malloc(sizeof(struct pixel) * new_width * new_height)};
}

uint8_t get_padding(const uint64_t width) {
    return width % 4 == 0 ? 0 : 4 - ((width * sizeof(struct pixel)) % 4);
}

size_t new_pixel_position(const uint64_t img_height, const size_t cur_height, const size_t cur_width) {
    return (img_height - 1 - cur_height) + img_height * cur_width;
}

size_t current_pixel(const uint64_t img_width, const size_t cur_height, const size_t cur_width){
    return img_width * cur_height + cur_width;
}

bool create_image(struct image* img, const struct bmp_header header){
    img->width = header.biWidth;
    img->height = header.biHeight;
    img->data = malloc(sizeof(struct pixel) * img->height * img->width);
    return true;
}
