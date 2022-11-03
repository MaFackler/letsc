#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <test.h>
#include <vector.h>
#include <platform.h>




int main() {
    Platform *p = platform_create();
    int window = platform_window_open(p, 0, 0, 800, 600);

    bool quit = false;
    XEvent e;
    while (!quit) {
        platform_begin(p);

        if (platform_key_released(p, 'q')) {
            quit = true;
        }
        platform_end(p);
    }

    platform_window_close(p, window);
    platform_destroy(p);

}
