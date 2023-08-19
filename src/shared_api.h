#ifndef SHARED_API_H
#define SHARED_API_H

#include <platform.h>
#include <framebuffer.h>
#include <bitmap.h>
#include <gui.h>

#define RED 0xFFFF0000
#define GREEN 0xFF00FF00
#define BLUE 0xFF0000FF
#define WHITE 0xFFFFFFFF
#define BLACK 0xFF000000

#define COLOR_EXTRACT_RED(value) ((uint8_t) ((value >> 16) & 0xFF))
#define COLOR_EXTRACT_GREEN(value) ((uint8_t) ((value >> 8) & 0xFF))
#define COLOR_EXTRACT_BLUE(value) ((uint8_t) ((value >> 0) & 0xFF))

#define COLOR(c) api->framebuffer->color = c
#define FILL_RECT(x, y, w, h) framebuffer_fill_rect(api->framebuffer, x, y, w, h)
#define TEXT(x, y, t) framebuffer_render_text(api->framebuffer, x, y, t)

typedef struct {
    Platform *platform;
    Framebuffer *framebuffer;
    Gui *gui;
} SharedApi;

typedef void (*update_func)(SharedApi *api);


#endif // SHARED_API_H
