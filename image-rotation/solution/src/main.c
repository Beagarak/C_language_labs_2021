#include <stdio.h>

#include "../include/image.h"
#include "../include/rotate.h"
#include "../include/status_processing.h"


void usage(){
    fprintf(stderr, "Incorrect input of arguments.\n"
                    "Expected format:\n"
                    "./image-transformer <source-image> <transformed-image>\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {usage();}
    if (argc < 2) { fprintf(stderr,"Not enough arguments \n");}
    if (argc > 2) { fprintf(stderr,"Too many arguments \n");}

    struct image image = {0};

    FILE *input_bmp, *output_bmp;

    enum open_status input_opened = open_bmp(&input_bmp, argv[1], "rb");
    if (input_opened != OPEN_OK) fprintf(stderr,"File: %s - %s\n", argv[1], open_message(input_opened));

    enum open_status output_opened = open_bmp(&output_bmp, argv[2], "wb");
    if (output_opened != OPEN_OK) {fprintf(stderr,"File: %s - %s\n", argv[2], open_message(output_opened));}



    enum read_status read_status = from_bmp(input_bmp,&image);
    if (read_status != READ_OK) { fprintf(stderr, "%s\n", read_message(read_status));}

    struct image result = rotate(&image);
    if (result.data == NULL) {fprintf(stderr, "Error allocating memory while rotating.\n");}
    free(image.data);

    enum write_status write_status = to_bmp(output_bmp,&result);
    if (write_status != WRITE_OK) { fprintf(stderr, "%s\n", write_message(write_status));}
    free(result.data);
    
    enum close_status input_closed = close_bmp(&input_bmp);
    if (input_closed != CLOSE_OK) {fprintf(stderr, "%s\n", close_message(input_closed));}
    
    enum close_status output_closed = close_bmp(&output_bmp);
    if (output_closed != CLOSE_OK) {fprintf(stderr, "%s\n", close_message(output_closed));}

    return 0;
}

