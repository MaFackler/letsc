#ifndef SHARED_API_H
#define SHARED_API_H

#include <platform.h>
#include <framebuffer.h>
#include <bitmap.h>
#include <gui.h>

typedef struct {
    Platform *platform;
    Framebuffer *framebuffer;
    Gui *gui;
} SharedApi;

typedef void (*update_func)(SharedApi *api);


#endif // SHARED_API_H
