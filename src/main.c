#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <test.h>
#include <vector.h>
#include <platform.h>
#include <shared.h>
#include <framebuffer.h>
#include <bitmap.h>

typedef void (*update_func)(Framebuffer *framebuffer, Bitmap *bitmap);

int main() {
    Bitmap *bitmap = bitmap_create_from_file("data/test64.bmp");
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
		update(framebuffer, bitmap);
        platform_window_render(p, &window);
        platform_end(p);
    }

    platform_window_close(p, &window);
    platform_destroy(p);
	shared_lib_close(&lib);
}
