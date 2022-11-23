#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <test.h>
#include <vector.h>
#include <platform.h>
#include <shared.h>
#include <framebuffer.h>

typedef void (*update_func)(Framebuffer *framebuffer);

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

    for (int y = 0; y < res->height; ++y) {
        for (int x = 0; x < res->width; ++x) {
            uint8_t b = *src++;
            uint8_t g = *src++;
            uint8_t r = *src++;
            uint32_t value = (0xFF << 24) | (r << 16) | (g << 8) | (b << 0);
            *dest++ = value; 
        }
        // NOTE: byte alignment
        src++;
        src++;
    }
    free(mem);
    return res;
}

TEST(bitmap_header) {
    char *mem = bitmap__read_file("data/test.bmp");
    BitmapHeader *header = bitmap__get_header(mem);
    CHECK(header->bfType, 0x4D42);
    CHECK(header->bfSize, 154);
    CHECK(header->bfReserved, 0);
    // TODO:
    //UCHECK((int) header->bfOffBits, (int) (sizeof(BitmapHeader) + sizeof(BitmapInfo)));

}

TEST(bitmap_info) {
    char *mem = bitmap__read_file("data/test.bmp");
    BitmapInfo *info = bitmap__get_info(mem);
    CHECK(info->biSize, 124);
    CHECK(info->biWidth, 2);
    CHECK(info->biHeight, 2);
    CHECK(info->biPlanes, 1);
    CHECK(info->biBitCount, 24);
    CHECK(info->biCompression, 0);
    CHECK(info->biSizeImage, 2 * 2 * 4);
}

TEST(bitmap_pixels) {
    Bitmap *b = bitmap_create_from_file("data/test.bmp");

    // NOTE: pixels are bottom up
    CHECK(b->pixels[0], 0xFFFF0000); // red
    CHECK(b->pixels[1], 0xFFFFFFFF); // white
    CHECK(b->pixels[2], 0xFF0000FF); // blue
    CHECK(b->pixels[3], 0xFF00FF00); // green
}

int main() {
    TEST_REGISTER(bitmap_header);
    TEST_REGISTER(bitmap_info);
    TEST_REGISTER(bitmap_pixels);
    TEST_RUN();
#if 0
	SharedLib lib;
	shared_lib_open(&lib, "build/shared_loop.so");

	update_func update = shared_lib_get_function(&lib, "update");


    Platform *p = platform_create();
    PlatformWindow window = platform_window_open(p, 0, 0, 800, 600);

    Framebuffer *framebuffer = framebuffer_create_external(window.pixels, window.width, window.height);

    bool quit = false;
    XEvent e;
    while (!quit) {
        platform_begin(p);

        if (platform_key_released(p, 'q')) {
            quit = true;
        }

		if (shared_lib_check_and_reload(&lib)) {
			update = shared_lib_get_function(&lib, "update");
		}

        // Render
		update(framebuffer);
        platform_window_render(p, &window);
        platform_end(p);
    }

    platform_window_close(p, &window);
    platform_destroy(p);
	shared_lib_close(&lib);
#endif
}
