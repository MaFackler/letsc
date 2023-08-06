#ifndef SHARED_API_H
#define SHARED_API_H

#include <platform.h>
#include <framebuffer.h>
#include <bitmap.h>
#include <gui.h>

#define RED 0xFFFF0000
#define GREEN 0xFF00FF00
#define BLUE 0xFF0000FF
#define COLOR(c) gui->framebuffer->color = c
#define FILL_RECT(x, y, w, h) framebuffer_fill_rect(gui->framebuffer, x, y, w, h)

typedef struct {
    Platform *platform;
    Framebuffer *framebuffer;
    Gui *gui;
} SharedApi;

typedef void (*update_func)(SharedApi *api);


#endif // SHARED_API_H
