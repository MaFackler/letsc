#include <shared_api.h>

void update(SharedApi *api) {
    api->framebuffer->stencil_value = 0xFF;
    framebuffer_fill_rect_stencil(api->framebuffer, 0, 0, api->framebuffer->width, api->framebuffer->height);

    unsigned int b0 = 0x659AD2;
    unsigned int b1 = 0x00599C;
    unsigned int b2 = 0x004482;

    v2 a = {0, 100};
    v2 b = {190, 0};
    v2 c = {380, 100};
    v2 d = {380, 320};
    v2 e = {190, 420};
    v2 f = {0, 320};
    v2 g = {190, 210};
    api->framebuffer->color = b0; 
    framebuffer_fill_triangle(api->framebuffer, a.x, a.y, c.x, c.y, b.x, b.y);
    framebuffer_fill_triangle(api->framebuffer, a.x, a.y, c.x, c.y, f.x, f.y);

    api->framebuffer->color = b2; 
    framebuffer_fill_triangle(api->framebuffer, f.x, f.y, e.x, e.y, g.x, g.y);
    framebuffer_fill_triangle(api->framebuffer, g.x, g.y, e.x, e.y, d.x, d.y);

    api->framebuffer->color = 0xFFFFFFFF; 
    api->framebuffer->stencil_value = 0x00;
    framebuffer_fill_circle_stencil(api->framebuffer, g.x, g.y, 70);
    framebuffer_fill_circle(api->framebuffer, g.x, g.y, 140);

    api->framebuffer->color = b1; 
    api->framebuffer->stencil_value = 0xFF;
    framebuffer_fill_rect_stencil(api->framebuffer, 0, 0, api->framebuffer->width, api->framebuffer->height);
    framebuffer_fill_triangle(api->framebuffer, c.x, c.y, d.x, d.y, g.x, g.y);
}
