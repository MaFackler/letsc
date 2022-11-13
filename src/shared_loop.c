#include <stdio.h>
#include <framebuffer.h>

void update(Framebuffer *framebuffer) {
    framebuffer->color = 0xFFFFFFFF;
    framebuffer_fill_rect(framebuffer, 0, 0, framebuffer->width, framebuffer->height);
    framebuffer->color = 0xFFFF0000;
    framebuffer_fill_rect(framebuffer, 0, 0, 100, 100);

    framebuffer_fill_circle(framebuffer, 200, 200, 50);
    framebuffer_fill_triangle(framebuffer, 400, 400, 500, 400, 500, 500);
}
