#include <stdio.h>
#include <framebuffer.h>
#include <bitmap.h>
#include <platform.h>
#include <gui.h>
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




#if 0

    framebuffer->stencil_value = 0x00;
    framebuffer->color = 0xFFFF0000;
    framebuffer_fill_rect_stencil(framebuffer, 10, 10, 80, 80);
    framebuffer_fill_rect(framebuffer, 0, 0, 100, 100);

    framebuffer_fill_circle_stencil(framebuffer, 200, 200, 40);
    framebuffer_fill_circle(framebuffer, 200, 200, 50);

    framebuffer_fill_triangle_stencil(framebuffer, 420, 410, 490, 410, 490, 480);
    framebuffer_fill_triangle(framebuffer, 400, 400, 500, 400, 500, 500);
#endif
}
