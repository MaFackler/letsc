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
#include <shared_api.h>
#include <gui.h>


int main() {
    Bitmap *bitmap = bitmap_create_from_file("data/font.bmp");
	SharedLib lib;
	shared_lib_open(&lib, "build/shared_loop.so");

	update_func update = shared_lib_get_function(&lib, "update");


    Platform *platform = platform_create();
    PlatformWindow window = platform_window_open(platform, 0, 0, 800, 600);

    Framebuffer *framebuffer = framebuffer_create_external(window.pixels, window.width, window.height);
    Gui gui = {0};

    SharedApi api = {
        .framebuffer = framebuffer,
        .platform = platform,
        .font = bitmap,
        .gui = &gui,
    };


    bool quit = false;
    XEvent e;
    while (!quit) {
        platform_begin(platform);

        if (platform_key_released(platform, 'q')) {
            quit = true;
        }

		if (shared_lib_check_and_reload(&lib)) {
			update = shared_lib_get_function(&lib, "update");
		}

        // Render
        // before update
        gui_init(&gui, api.framebuffer, api.font);
        gui_set_mouse(&gui, api.platform->mouse_x, api.platform->mouse_y, api.platform->mouse_left_down);
        // actual update
		update(&api);
        platform_window_render(platform, &window);
        platform_end(platform);
    }

    platform_window_close(platform, &window);
    platform_destroy(platform);
	shared_lib_close(&lib);
}
