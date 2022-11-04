#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <test.h>
#include <vector.h>
#include <platform.h>




int main() {
    Platform *p = platform_create();
    PlatformWindow window = platform_window_open(p, 0, 0, 800, 600);

    bool quit = false;
    XEvent e;
    while (!quit) {
        platform_begin(p);

        if (platform_key_released(p, 'q')) {
            quit = true;
        }

        // Render
        for (int y = 0; y < window.height; ++y) {
            for (int x = 0; x < window.width; ++x) {
                window.pixels[y * window.width + x] = 0xFFFF0000;
            }
        }
        platform_window_render(p, &window);
        platform_end(p);
    }

    platform_window_close(p, &window);
    platform_destroy(p);

}
