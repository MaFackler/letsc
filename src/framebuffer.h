#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t color;
    uint8_t stencil_value;
    uint32_t *pixels;
    uint8_t *stencil;
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


Framebuffer *framebuffer_create_external(uint32_t *pixels, uint32_t width, uint32_t height) {
    Framebuffer *res = malloc(sizeof(Framebuffer));
    res->pixels = pixels;
    res->width = width;
    res->height = height;
    res->stencil = malloc(sizeof(uint8_t) * width * height);
    res->stencil_value = 0xFF;
    memset(res->stencil, res->stencil_value, width * height);
    return res;
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

void framebuffer_fill_rect_stencil(Framebuffer *framebuffer, int x, int y, int w, int h) {
    ClipRect rect = {x, y, x + w, y + h};
    cliprect_clip(&rect, 0, 0, framebuffer->width, framebuffer->height);
    for (int y = rect.ymin; y < rect.ymax; ++y) {
        for (int x = rect.xmin; x < rect.xmax; ++x) {
            framebuffer->stencil[y * framebuffer->width + x] = framebuffer->stencil_value;
        }
    }
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


#endif // FRAMEBUFFER_H
