#ifndef BITMAP_H
#define BITMAP_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t bfType;
    uint32_t bfSize;
    uint32_t bfReserved;
    uint32_t bfOffBits;
} BitmapHeader;

typedef struct __attribute__((packed)) {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BitmapInfo;

typedef struct {
    int width;
    int height;
    uint32_t *pixels;
} Bitmap;

char *bitmap__read_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *res = malloc(size);
    while (size > 0) {
        size_t bytes_read = fread(res, 1, size, fp);
        size -= bytes_read;
    };
    fclose(fp);
    return res;
}

BitmapHeader* bitmap__get_header(char *mem) {
    return (BitmapHeader*) mem;
}

BitmapInfo* bitmap__get_info(char *mem) {
    return (BitmapInfo*) (mem + sizeof(BitmapHeader));
}

Bitmap* bitmap_create_from_file(const char *filename) {
    Bitmap *res = malloc(sizeof(Bitmap));
    char *mem = bitmap__read_file(filename);
    BitmapHeader *header = bitmap__get_header(mem);
    BitmapInfo *info = bitmap__get_info(mem);
    char *src = mem + header->bfOffBits;
    res->width = info->biWidth;
    res->height = info->biHeight;
    res->pixels = (uint32_t *) malloc(sizeof(uint32_t) * res->width * res->height);

    uint32_t *dest = res->pixels;

    int stride = res->width * 3 + res->width % 4;
    for (int y = 0; y < res->height; ++y) {
        char *src_row = src + (res->height - y - 1) * stride;
        for (int x = 0; x < res->width; ++x) {
            uint8_t b = *src_row++;
            uint8_t g = *src_row++;
            uint8_t r = *src_row++;
            uint32_t value = (0xFF << 24) | (r << 16) | (g << 8) | (b << 0);
            *dest++ = value; 
        }
    }
    free(mem);
    return res;
}


#endif // BITMAP_H
