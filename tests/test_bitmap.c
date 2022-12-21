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

int main() {
    TEST_REGISTER(bitmap_header);
    TEST_REGISTER(bitmap_info);
    TEST_REGISTER(bitmap_pixels_not_aligned);
    TEST_REGISTER(bitmap_pixels_aligned);
    TEST_RUN();
}
