#include <stdio.h>
#include <shared_api.h>

void update(SharedApi *api) {
    api->framebuffer->color = 0xFF000000;
    framebuffer_fill_rect(api->framebuffer, 0, 0, api->framebuffer->width, api->framebuffer->height);

    char buf[256] = {0};
    sprintf(&buf[0], "Mouse x=%d, y=%d", api->platform->mouse_x, api->platform->mouse_y);

    //render_char(framebuffer, bitmap, 10, 10, '2');
    framebuffer_render_text(api->framebuffer, api->font, 10, 10, "# MF Code");
    framebuffer_render_text(api->framebuffer, api->font, 10, 20, "---------");
    framebuffer_render_text(api->framebuffer, api->font, 10, 30, &buf[0]);

    //if (p->mouse_left_down) {
    if (gui_render_button(api->gui, 30, 40, "Hello World", 0xFFFF0000)) {
        framebuffer_fill_rect(api->framebuffer, 200, 200, 100, 100);
    }
    //framebuffer_fill_bitmap(framebuffer, bitmap->pixels, 0, 0, bitmap->width, bitmap->height);
}
