#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <test.h>
#include <vector.h>
#include <platform.h>
#include <shared.h>

typedef void (*update_func)(PlatformWindow *window);

int main() {
	SharedLib lib;
	shared_lib_open(&lib, "build/shared_loop.so");

	update_func update = shared_lib_get_function(&lib, "update");


    Platform *p = platform_create();
    PlatformWindow window = platform_window_open(p, 0, 0, 800, 600);

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
		update(&window);
        platform_window_render(p, &window);
        platform_end(p);
    }

    platform_window_close(p, &window);
    platform_destroy(p);
	shared_lib_close(&lib);
}
