cmake_minimum_required(VERSION 3.21)
project(solution C)

set(CMAKE_C_STANDARD 99)

add_executable(solution src/main.c include/image.h src/bmp_header.c include/bmp_header.h src/bmp_header.c include/bmp_header.h src/rotate.c include/rotate.h src/image_utils.c include/image_utils.h src/bmp_processing.c include/bmp_processing.h src/status_processing.c include/status_processing.h)
