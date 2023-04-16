#include <shared_api.h>

void update(SharedApi *api) {

    Framebuffer * framebuffer = api->framebuffer;
    static bool initialized = false;
    static Bitmap *bitmap = NULL;
    if (!initialized) {
        initialized = true;
        bitmap = bitmap_create_from_file("data/block.bmp");
    }


    int startx = 400;
    int starty = 300;
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            float xx = x * 0.5f + y * -0.5f;
            float yy = x * 0.25f + y * 0.25f;
            framebuffer_fill_bitmap(framebuffer, bitmap->pixels,
                                    startx + xx * 32.0f,
                                    starty + yy * 32.0f,
                                    bitmap->width, bitmap->height);
        }
    }

}
