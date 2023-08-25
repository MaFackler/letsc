#include <test.h>
#include <bitmap.h>

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

TEST(bitmap_pixels_not_aligned) {
    Bitmap *b = bitmap_create_from_file("data/test.bmp");

    // NOTE: pixels are bottom up
    CHECK(b->pixels[0], 0xFF0000FF); // blue
    CHECK(b->pixels[1], 0xFF00FF00); // green
    CHECK(b->pixels[2], 0xFFFF0000); // red
    CHECK(b->pixels[3], 0xFFFFFFFF); // white
}

TEST(bitmap_pixels_aligned) {
    Bitmap *b = bitmap_create_from_file("data/test64.bmp");

    // NOTE: pixels are bottom up
    CHECK(b->pixels[0], 0xFF0000FF); // blue
    CHECK(b->pixels[32], 0xFF00FF00); // green
    CHECK(b->pixels[32 * b->width], 0xFFFF0000); // red
    CHECK(b->pixels[32 * b->width + 32], 0xFFFFFFFF); // white
}

TEST(bitmap_header_with_alpha) {
    char *mem = bitmap__read_file("data/testalpha.bmp");
    BitmapHeader *header = bitmap__get_header(mem);
    CHECK(header->bfType, 0x4D42);
    CHECK(header->bfReserved, 0);

    BitmapInfo *info = bitmap__get_info(mem);
    CHECK(info->biCompression, 3);
    CHECK(info->biBitCount, 32);

    uint32_t *mask_start = (uint32_t *) (info + 1);
    uint32_t red_mask = *mask_start++;
    uint32_t green_mask = *mask_start++;
    uint32_t blue_mask = *mask_start++;
    uint32_t alpha_mask = *mask_start++;
    CHECK(red_mask, 0x00FF0000);
    CHECK(green_mask, 0x0000FF00);
    CHECK(blue_mask, 0x000000FF);
    CHECK(alpha_mask, 0xFF000000);
    free(mem);
}

TEST(bitmap_pixels_with_alpha) {
    Bitmap *b = bitmap_create_from_file("data/testalpha.bmp");
    CHECK(b->width, 4);
    CHECK(b->height, 2);

    CHECK(b->pixels[0], 0xFF0000FF);
    CHECK(b->pixels[1], 0xFF00FF00);
    CHECK(b->pixels[2], 0xFFFF0000);
    CHECK(b->pixels[3], 0xFFFFFFFF);

    CHECK(b->pixels[4], 0x7F0000FF);
    CHECK(b->pixels[5], 0x7F00FF00);
    CHECK(b->pixels[6], 0x7FFF0000);
    CHECK(b->pixels[7], 0x7FFFFFFF);
}

TEST_MAIN() {
    TEST_REGISTER(bitmap_header);
    TEST_REGISTER(bitmap_info);
    TEST_REGISTER(bitmap_pixels_not_aligned);
    TEST_REGISTER(bitmap_pixels_aligned);
    TEST_REGISTER(bitmap_header_with_alpha);
    TEST_REGISTER(bitmap_pixels_with_alpha);
    TEST_RUN();
}
