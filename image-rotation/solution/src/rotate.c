#include "../include/rotate.h"

struct image rotate(struct image const *input) {
    struct image output = create_rotated_blank(input->height, input->width);

    for (size_t height = 0; height < input->height; height++) {
        for (size_t width = 0; width < input->width; width++) {
            size_t new_pix = new_pixel_position(input->height, height, width);
            size_t current_pix = current_pixel(input->width, height, width);
            output.data[new_pix] = input->data[current_pix];
        }
    }
    return output;
}
