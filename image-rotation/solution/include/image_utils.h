#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include "bmp_header.h"
#include "image.h"
#include <stdbool.h>

struct image create_rotated_blank(uint64_t width, uint64_t height);
bool create_image(struct image* img, const struct bmp_header header);

uint8_t get_padding(const uint64_t width);

size_t new_pixel_position(const uint64_t img_height,const size_t cur_height,const size_t cur_width);
size_t current_pixel(const uint64_t width, const size_t cur_height, const size_t cur_width);
#endif
