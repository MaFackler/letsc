#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <bitmap.h>
#include <math.h>
#include "font_8x8.h"

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)
#define FRAMEBUFFER_PIXEL(f, x, y) f->pixels[(y) * f->width + (x)] 

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t color;
    uint8_t stencil_value;
    uint32_t *pixels;
    uint8_t *stencil;
    Font8x8 font;
} Framebuffer;

typedef struct {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
} ClipRect;

void cliprect_clip(ClipRect *rect, int x0, int y0, int x1, int y1) {
    rect->xmin = MAX(rect->xmin, x0);
    rect->ymin = MAX(rect->ymin, y0);

    rect->xmax = MIN(rect->xmax, x1);
    rect->ymax = MIN(rect->ymax, y1);
}

bool cliprect_inside(ClipRect *rect, int x, int y) {
    return rect->xmin <= x && x < rect->xmax && rect->ymin <= y && y < rect->ymax;
}


Framebuffer *framebuffer_create_external(uint32_t *pixels, uint32_t width, uint32_t height) {
    Framebuffer *res = malloc(sizeof(Framebuffer));
    res->pixels = pixels;
    res->width = width;
    res->height = height;
    res->stencil = malloc(sizeof(uint8_t) * width * height);
    res->stencil_value = 0xFF;
    memset(res->stencil, res->stencil_value, width * height);
    font8x8_init(&res->font);
    return res;
}

void framebuffer_draw_line(Framebuffer *framebuffer, int x1, int y1, int x2, int y2) {
    ClipRect rect = {0, 0, framebuffer->width, framebuffer->height};
    float m = (float) (y2 - y1) / (float) (x2 - x1);
    if (fabs(m) <= 1.0f) {
        // draw according to x
        for (int x = MIN(x1, x2); x < MAX(x1, x2); ++x) {
            float y = m * (x - x1) + y1;
            if (cliprect_inside(&rect, x, y))
                FRAMEBUFFER_PIXEL(framebuffer, x, (int) y) = framebuffer->color;
        }
    } else {
        // draw according to y
        for (int y = MIN(y1, y2); y < MAX(y1, y2); ++y) {
            float x = (float) (y - y1) / m + x1;
            if (cliprect_inside(&rect, x, y))
                FRAMEBUFFER_PIXEL(framebuffer, (int) x, y) = framebuffer->color;
        }
    }
}

void framebuffer_fill_rect(Framebuffer *framebuffer, int x, int y, int w, int h) {
    ClipRect rect = {x, y, x + w, y + h};
    cliprect_clip(&rect, 0, 0, framebuffer->width, framebuffer->height);
    for (int y = rect.ymin; y < rect.ymax; ++y) {
        for (int x = rect.xmin; x < rect.xmax; ++x) {
            if (framebuffer->stencil[y * framebuffer->width + x] > 0) {
                framebuffer->pixels[y * framebuffer->width + x] = framebuffer->color;
            }
        }
    }
}

void framebuffer_clean(Framebuffer *framebuffer, uint32_t color) {
    framebuffer->color = color;
    framebuffer_fill_rect(framebuffer, 0, 0, framebuffer->width, framebuffer->height);
}


void framebuffer_fill_bitmap_ex(Framebuffer *framebuffer, uint32_t *pixels, int stride, int sx, int sy, int dx, int dy, int w, int h) {
    ClipRect rect = {dx, dy, dx + w, dy + h};
    cliprect_clip(&rect, 0, 0, framebuffer->width, framebuffer->height);
    for (int y = rect.ymin; y < rect.ymax; ++y) {
        uint32_t *row = &pixels[(sy + y - rect.ymin) * stride + sx];
        for (int x = rect.xmin; x < rect.xmax; ++x) {
            if (framebuffer->stencil[y * framebuffer->width + x] > 0) {
                uint32_t *dest = &framebuffer->pixels[y * framebuffer->width + x];
                uint8_t alpha = (0xFF000000 & *row) >> 24;
                // TODO: alpha blending
                if (alpha != 0) {
                    *dest = *row;
                }
                *row++;
            }
        }
    }
}

void framebuffer_fill_bitmap(Framebuffer *framebuffer, uint32_t *pixels, int x, int y, int w, int h) {
    framebuffer_fill_bitmap_ex(framebuffer, pixels, w,
                               0, 0, x, y, w, h);
}

void framebuffer_fill_rect_stencil(Framebuffer *framebuffer, int x, int y, int w, int h) {
    ClipRect rect = {x, y, x + w, y + h};
    cliprect_clip(&rect, 0, 0, framebuffer->width, framebuffer->height);
    for (int y = rect.ymin; y < rect.ymax; ++y) {
        for (int x = rect.xmin; x < rect.xmax; ++x) {
            framebuffer->stencil[y * framebuffer->width + x] = framebuffer->stencil_value;
        }
    }
}

void framebuffer_clean_stencil(Framebuffer *framebuffer) {
    int value_old = framebuffer->stencil_value;
    framebuffer->stencil_value = 0xFF;
    framebuffer_fill_rect_stencil(framebuffer, 0, 0, framebuffer->width, framebuffer->height);
    framebuffer->stencil_value = value_old;
}

void framebuffer_fill_circle(Framebuffer *framebuffer, int x0, int y0, int r) {
    ClipRect rect = {x0 - r, y0 - r, x0 + 2 * r, y0 + 2 * r};
    cliprect_clip(&rect, 0, 0, framebuffer->width, framebuffer->height);
    float rf = r * r;
    for (int y = rect.ymin; y < rect.ymax; ++y) {
        for (int x = rect.xmin; x < rect.xmax; ++x) {
            float xf = (x - x0);
            float yf = (y - y0);
            if (xf * xf + yf * yf < rf) {
                if (framebuffer->stencil[y * framebuffer->width + x] > 0) {
                    framebuffer->pixels[y * framebuffer->width + x] = framebuffer->color;
                }
            }

        }
    }
}

void framebuffer_fill_circle_stencil(Framebuffer *framebuffer, int x0, int y0, int r) {
    ClipRect rect = {x0 - r, y0 - r, x0 + 2 * r, y0 + 2 * r};
    cliprect_clip(&rect, 0, 0, framebuffer->width, framebuffer->height);
    float rf = r * r;
    for (int y = rect.ymin; y < rect.ymax; ++y) {
        for (int x = rect.xmin; x < rect.xmax; ++x) {
            float xf = (x - x0);
            float yf = (y - y0);
            if (xf * xf + yf * yf < rf) {
                framebuffer->stencil[y * framebuffer->width + x] = framebuffer->stencil_value;
            }

        }
    }
}

void framebuffer_fill_triangle(Framebuffer *framebuffer, int ax, int ay, int bx, int by, int cx, int cy) {
    int xstart = MIN(MIN(ax, bx), cx);
    int ystart = MIN(MIN(ay, by), cy);
    int xend = MAX(MAX(ax, bx), cx);
    int yend = MAX(MAX(ay, by), cy);
    
    ClipRect rect = {xstart, ystart, xend, yend};
    cliprect_clip(&rect, 0, 0, framebuffer->width, framebuffer->height);

    float p1 = cy - ay;
    float p2 = cx - ax;
    float p3 = by - ay;

    for (int y = rect.ymin; y < rect.ymax; ++y) {
        for (int x = rect.xmin; x < rect.xmax; ++x) {
            float p4 = y - ay;

            float u = (ax * p1 + p4 * p2 - x * p1) / (p3 * p2 - (bx - ax) * p1);
            float v = (p4 - u * p3) / p1;
            if (u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f) {
                if (framebuffer->stencil[y * framebuffer->width + x] > 0) {
                    framebuffer->pixels[y * framebuffer->width + x] = framebuffer->color;
                }
            }
        }
    }
}

void framebuffer_fill_triangle_stencil(Framebuffer *framebuffer, int ax, int ay, int bx, int by, int cx, int cy) {
    int xstart = MIN(MIN(ax, bx), cx);
    int ystart = MIN(MIN(ay, by), cy);
    int xend = MAX(MAX(ax, bx), cx);
    int yend = MAX(MAX(ay, by), cy);
    
    ClipRect rect = {xstart, ystart, xend, yend};
    cliprect_clip(&rect, 0, 0, framebuffer->width, framebuffer->height);

    float p1 = cy - ay;
    float p2 = cx - ax;
    float p3 = by - ay;

    for (int y = rect.ymin; y < rect.ymax; ++y) {
        for (int x = rect.xmin; x < rect.xmax; ++x) {
            float p4 = y - ay;

            float u = (ax * p1 + p4 * p2 - x * p1) / (p3 * p2 - (bx - ax) * p1);
            float v = (p4 - u * p3) / p1;
            if (u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f) {
                framebuffer->stencil[y * framebuffer->width + x] = framebuffer->stencil_value;
            }
        }
    }
}

void framebuffer_render_char(Framebuffer *framebuffer, int *x, int y, char c) {
    const int cw = 8;
    const int ch = 8;
    uint8_t *data = &framebuffer->font.data[(c - ' ') * 8];
    for (int dy = 0; dy < ch; dy++) {
        for (int dx = 0; dx < cw; dx++) {
            if (0x01 << (cw - 1 - dx) & data[dy]) {
                FRAMEBUFFER_PIXEL(framebuffer, *x + dx, y + dy) = framebuffer->color;
            }
        }
    }
    *x += cw;
}

void framebuffer_render_text(Framebuffer *framebuffer, int x, int y, char *text) {
    char c = 0;
    while (c = *text++) {
        framebuffer_render_char(framebuffer, &x, y, c);
    }
}



#endif // FRAMEBUFFER_H
