#include <shared_api.h>

void update(SharedApi *api) {
    Framebuffer *framebuffer = api->framebuffer;
    framebuffer->stencil_value = 0x00;
    framebuffer->color = 0xFFFF0000;
    framebuffer_fill_rect_stencil(framebuffer, 10, 10, 80, 80);
    framebuffer_fill_rect(framebuffer, 0, 0, 100, 100);

    framebuffer_fill_circle_stencil(framebuffer, 200, 200, 40);
    framebuffer_fill_circle(framebuffer, 200, 200, 50);

    framebuffer_fill_triangle_stencil(framebuffer, 420, 410, 490, 410, 490, 480);
    framebuffer_fill_triangle(framebuffer, 400, 400, 500, 400, 500, 500);
}
