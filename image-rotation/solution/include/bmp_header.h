#ifndef BMP_HEADER_H
#define BMP_HEADER_H

#include "image.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

//Определяем переменные, постоянные для заголовка файла
#define BF_TYPE 0x4D42
#define BF_RESERVED 0
#define BI_SIZE 40
#define BI_PLANES 1
#define BI_BIT_COUNT 24
#define BI_COMPRESSION 0
#define BI_X_PELS_PER_METER 0
#define BI_Y_PELS_PER_METER 0
#define BI_CLR_USED 0
#define BI_CLR_IMPORTANT 0

#pragma pack(push, 1)
struct bmp_header
{
    uint16_t bfType;             //4D42(little-endian).
    uint32_t bfileSize;          //Размер файла в байтах.
    uint32_t bfReserved;         //Зарезервированное поле, содержит 0
    uint32_t bOffBits;           //Положение пиксельных данных относительно начала данной структуры (в байтах).
    uint32_t biSize;             //Размер структуры в байтах
    uint32_t biWidth;            //Ширина изображения в пикселях.
    uint32_t biHeight;           //Bысота изображения в пикселях
    uint16_t biPlanes;           //Устанавливаем значение 1.
    uint16_t biBitCount;         //Количество бит на пиксель.
    uint32_t biCompression;      //Способ хранения пикселей.
    uint32_t biSizeImage;        //Размер всех пикселей изображения (в байтах).
    uint32_t biXPelsPerMeter;    //Количество пикселей на метр по горизонтали.
    uint32_t biYPelsPerMeter;    //Количество пикселей на метр по вертикали.
    uint32_t biClrUsed;          //Количество используемых цветов.
    uint32_t biClrImportant;     //Количество важных цветов
};
#pragma pack(pop)

struct bmp_header make_bmp_header(struct image const* image);

#endif
