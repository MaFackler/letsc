#include <stdio.h>
#include <framebuffer.h>
#include <bitmap.h>
#include <platform.h>

typedef struct {
    int x;
    int y;
} v2;

v2 get_text_dim(char *text) {
    const int cw = 7;
    const int ch = 9;
    return (v2) {cw * strlen(text), ch};
}


void render_char(Framebuffer *framebuffer, Bitmap *bitmap, int *x, int y, char c) {
    int xindex = (c - ' ') % 18;
    int yindex = (c - ' ') / 18;
    const int cw = 7;
    const int ch = 9;
    framebuffer_fill_bitmap_ex(framebuffer, bitmap->pixels, bitmap->width,
                               xindex * cw, yindex * ch, *x, y, cw, ch);
    *x += cw;
}

void render_text(Framebuffer *framebuffer, Bitmap *bitmap, int x, int y, char *text) {
    char c = 0;
    while (c = *text++) {
        render_char(framebuffer, bitmap, &x, y, c);
    }
}


void render_button(Framebuffer *framebuffer, Bitmap *font, int x, int y, char *text, unsigned int color) {
    framebuffer->color = color;
    v2 dim = get_text_dim(text);
    int margin = 2;
    dim.x += 2 * margin;
    dim.y += 2 * margin;
    v2 pos = {30, 30};
    framebuffer_fill_rect(framebuffer, pos.x, pos.y, dim.x, dim.y);
    render_text(framebuffer, font, pos.x + margin, pos.y + margin, text);
}

 

void update(Platform *p, Framebuffer *framebuffer, Bitmap *bitmap) {
    framebuffer->color = 0xFF000000;
    framebuffer_fill_rect(framebuffer, 0, 0, framebuffer->width, framebuffer->height);

    //render_char(framebuffer, bitmap, 10, 10, '2');
    render_text(framebuffer, bitmap, 10, 10, "# MF Code");
    render_text(framebuffer, bitmap, 10, 20, "---------");

    //if (p->mouse_left_down) {
    render_button(framebuffer, bitmap, 30, 30, "Hello World", 0xFFFF0000);
    //framebuffer_fill_bitmap(framebuffer, bitmap->pixels, 0, 0, bitmap->width, bitmap->height);

#if 0 // C-Logo
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
