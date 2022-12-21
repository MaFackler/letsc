#include <stdio.h>
#include <framebuffer.h>
#include <bitmap.h>

typedef struct {
    int x;
    int y;
} v2;


void update(Framebuffer *framebuffer, Bitmap *bitmap) {
    framebuffer->color = 0xFF000000;
    framebuffer_fill_rect(framebuffer, 0, 0, framebuffer->width, framebuffer->height);

    framebuffer_fill_bitmap(framebuffer, bitmap->pixels, 500, 100, bitmap->width, bitmap->height);

#if 1 // C-Logo
    framebuffer->stencil_value = 0xFF;
    framebuffer_fill_rect_stencil(framebuffer, 0, 0, framebuffer->width, framebuffer->height);

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
    framebuffer->color = b0; 
    framebuffer_fill_triangle(framebuffer, a.x, a.y, c.x, c.y, b.x, b.y);
    framebuffer_fill_triangle(framebuffer, a.x, a.y, c.x, c.y, f.x, f.y);

    framebuffer->color = b2; 
    framebuffer_fill_triangle(framebuffer, f.x, f.y, e.x, e.y, g.x, g.y);
    framebuffer_fill_triangle(framebuffer, g.x, g.y, e.x, e.y, d.x, d.y);

    framebuffer->color = 0xFFFFFFFF; 
    framebuffer->stencil_value = 0x00;
    framebuffer_fill_circle_stencil(framebuffer, g.x, g.y, 70);
    framebuffer_fill_circle(framebuffer, g.x, g.y, 140);

    framebuffer->color = b1; 
    framebuffer->stencil_value = 0xFF;
    framebuffer_fill_rect_stencil(framebuffer, 0, 0, framebuffer->width, framebuffer->height);
    framebuffer_fill_triangle(framebuffer, c.x, c.y, d.x, d.y, g.x, g.y);

#endif



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
