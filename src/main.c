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
#include <dict.h>
#include <renderer.h>

#include <dirent.h>
#include <stdarg.h>

static Framebuffer *framebuffer = NULL;

char *string_create(char *fmt, ...) {
    va_list args1;
    va_list args2;
    va_start(args1, fmt);
    va_copy(args2, args1);
    size_t size = vsnprintf(NULL, 0, fmt, args1);
    char *res = malloc(size + 1);
    vsprintf(res, fmt, args2);
    res[size] = 0;
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

void set_color(unsigned int color) {
    framebuffer->color = color;
}

void push_rect(float x, float y, float w, float h, int layer) {
}

void push_text(float x, float y, char *text, size_t n, int layer) {
}

int get_width(void) {
}

int get_height(void) {
}


int main() {

    dict *d = dict_create(256);

    SharedLib lib;
    //shared_lib_open(&lib, "build/shared_c_logo.so");
    //update_func update = shared_lib_get_function(&lib, "update");

    update_func update = NULL;

    Platform *platform = platform_create();
    PlatformWindow window = platform_window_open(platform, 0, 0, 800, 600);

    framebuffer = framebuffer_create_external(window.pixels, window.width, window.height);
    Renderer renderer = {0};
    renderer_init(&renderer, framebuffer->width, framebuffer->height);
    Gui gui = {0};

    SharedApi api = {
        .framebuffer = framebuffer,
        .platform = platform,
        .gui = &gui,
    };
    gui_init(&gui, &renderer);

    int loaded_index = -1;
    const char *varfile = "variables.dat";
    size_t used_index = 1;
    if (access(varfile, F_OK) == 0) {
        FILE *fp = fopen(varfile, "r");
        assert(fp != NULL);
        fscanf(fp, "%llu", &used_index);
        fclose(fp);
    }
    char **shared_libs = get_shared_libs("src", "shared");
    used_index = MIN(used_index, vec_size(shared_libs));
    bool collapsed = false;

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
            FILE *fp = fopen(varfile, "w");
            fprintf(fp, "%llu", used_index);
            fclose(fp);
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
        framebuffer_clean(framebuffer, 0xFF1a1a1a);
        framebuffer_clean_stencil(framebuffer);
        gui_set_mouse(&gui, api.platform->mouse_x, api.platform->mouse_y,
                      api.platform->mouse_left_down,
                      api.platform->mouse_left_released);
        // actual update
        update(&api);
        // after update
        framebuffer_clean_stencil(framebuffer);

        GUI_PANEL(&gui, "shared_libs_panel", 500, 2) {
            gui_combobox(&gui, shared_libs, vec_size(shared_libs), &used_index, "cmb_shared_libs");
        }

#if 0
        gui_push_panel(&gui, "shared_libs_panel", 500, 2);
            gui_combobox(&gui, shared_libs, vec_size(shared_libs), &used_index, "cmb_shared_libs");
        gui_pop(&gui);
#endif


        gui_render(&gui);
        for (int layer_index = 0; layer_index < RENDERER_NUM_LAYERS; ++layer_index) {
            RenderCommand *commands = renderer.layer_commands[layer_index];
            for (int i = 0; i < vec_size(commands); ++i) {
                RenderCommand cmd = commands[i];
                framebuffer->color = cmd.color;
                switch (cmd.type) {
                    case RENDER_COMMAND_RECT:
                        framebuffer_fill_rect(framebuffer,
                                              cmd.cmd_rect.x,
                                              cmd.cmd_rect.y,
                                              cmd.cmd_rect.w,
                                              cmd.cmd_rect.h);
                        break;
                    case RENDER_COMMAND_TEXT:
                        framebuffer_render_textn(framebuffer,
                                                 cmd.cmd_text.x,
                                                 cmd.cmd_text.y,
                                                 cmd.cmd_text.text,
                                                 cmd.cmd_text.n);
                        break;

                }
            }
        }
        renderer_end_frame(&renderer);
        platform_window_render(platform, &window);
        platform_end(platform);
    }

    platform_window_close(platform, &window);
    platform_destroy(platform);
    shared_lib_close(&lib);
}
