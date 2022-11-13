#include <stdio.h>
#include <platform.h>

void update(PlatformWindow *window) {
    for (int y = 0; y < window->height; ++y) {
        for (int x = 0; x < window->width; ++x) {
            window->pixels[y * window->width + x] = 0xFF00FFFF;
        }
    }
}
