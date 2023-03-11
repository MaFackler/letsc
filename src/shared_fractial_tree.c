#include <shared_api.h>
#include <mathematics.h>

void update(SharedApi *api) {
    Framebuffer *framebuffer = api->framebuffer;
    Platform *p = api->platform;

    framebuffer->color = 0xFFFFFFFF;
    framebuffer_draw_line(framebuffer, 400, 400, p->mouse_x, p->mouse_y);
}
