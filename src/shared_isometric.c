#include <shared_api.h>

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

uint8_t world[10][10][10] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 3, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 3, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 3, 3, 3, 3, 3, 3, 3,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 2, 0, 2, 2, 2, 2, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 2, 2, 0, 2, 0, 0, 2, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

};

void update(SharedApi *api) {

    Framebuffer * framebuffer = api->framebuffer;
    static bool initialized = false;
    static Bitmap *bitmaps[] = {
        NULL, // Nothing
        NULL, // Grass
        NULL, // Wood
        NULL, // Sand
    };
    if (!initialized) {
        initialized = true;
        bitmaps[1] = bitmap_create_from_file("data/block.bmp");
        bitmaps[2] = bitmap_create_from_file("data/block_wood.bmp");
        bitmaps[3] = bitmap_create_from_file("data/block_sand.bmp");
    }


    int startx = 400;
    int starty = 300;
    for (int y = 0; y < ARRAY_LENGTH(world); ++y) {
        for (int z = 0; z < ARRAY_LENGTH(world[y]); ++z) {
            for (int x = 0; x < ARRAY_LENGTH(world[y][z]); ++x) {
                uint8_t value = world[y][z][x];
                if (value > 0) {
                    Bitmap *bitmap = bitmaps[value];
                    float xx = x * 0.5f + z * -0.5f;
                    float yy = x * 0.25f + z * 0.25f - y * 0.5f;
                    framebuffer_fill_bitmap(framebuffer, bitmap->pixels,
                                            startx + xx * 32.0f,
                                            starty + yy * 32.0f,
                                            bitmap->width, bitmap->height);
                
                }
            }
        }
    }

}
