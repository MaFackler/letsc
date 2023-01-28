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

#include <dirent.h>
#include <stdarg.h>

char *string_create(char *fmt, ...) {
    va_list args1;
    va_list args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    size_t size = vsnprintf(NULL, 0, fmt, args1);
    char *res = malloc(size);
    vsprintf(res, fmt, args2);
    va_end(args1);
    va_end(args2);
    return res;
}

char **get_shared_libs(const char *path, const char *prefix) {
    char **res = NULL;
    DIR *d;
    struct dirent *dir;
    d = opendir(path);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(prefix, dir->d_name, strlen(prefix)) == 0 &&
                dir->d_name[strlen(dir->d_name) - 1] == 'c') {
                char buf[256] = {0};
                // NOTE: remove .c
                strncpy(&buf[0], dir->d_name, strlen(dir->d_name) - 2);
                vec_push(res, string_create("build/%s.so", &buf[0]));
            }
        }
    }
    return res;
}


int main() {
    Bitmap *bitmap = bitmap_create_from_file("data/font.bmp");
    SharedLib lib;
    //shared_lib_open(&lib, "build/shared_c_logo.so");
    //update_func update = shared_lib_get_function(&lib, "update");

    update_func update = NULL;

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

    int loaded_index = -1;
    int used_index = 1;
    char **shared_libs = get_shared_libs("src", "shared");

    bool quit = false;
    XEvent e;
    while (!quit) {
        if (loaded_index != used_index) {
            if (loaded_index != -1) {
                shared_lib_close(&lib);
            }
            loaded_index = used_index;
            const char *lib_path = shared_libs[loaded_index];
            shared_lib_open(&lib, lib_path);
            update = shared_lib_get_function(&lib, "update");
        }
        assert(update != NULL);

        platform_begin(platform);

        if (platform_key_released(platform, 'q')) {
            quit = true;
        }

        if (platform_key_pressed(platform, 'a')) {
            used_index = MAX(--used_index, 0);
        }

        if (platform_key_pressed(platform, 'd')) {
            used_index = MIN(++used_index, vec_size(shared_libs) - 1);
        }



        if (shared_lib_check_and_reload(&lib)) {
            update = shared_lib_get_function(&lib, "update");
        }

        // Render
        // before update
        framebuffer_clean(framebuffer, 0xFF000000);
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
