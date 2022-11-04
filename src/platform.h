#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>

#define PLATFORM_NUM_KEYS 0xFFFF
typedef struct {
    bool pressed;
    bool released;
    bool down;
} KeyState;

typedef struct {
    Display *display;
    Window root;
    Visual *visual;
    GC gc;
    int depth;
    int screen;
    KeyState input[PLATFORM_NUM_KEYS];
} Platform;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t *pixels;
    XShmSegmentInfo info;
    XImage *image;
    Window window;
} PlatformWindow;

Platform* platform_create() {
    Platform *p = malloc(sizeof(Platform));
    p->display = XOpenDisplay(NULL);
    assert(p->display);
    p->screen = XDefaultScreen(p->display);
    p->visual = XDefaultVisual(p->display, p->screen);
    p->gc = XDefaultGC(p->display, p->screen);
    p->depth = XDefaultDepth(p->display, p->screen);
    p->root = XRootWindow(p->display, p->screen);
    return p;
}

void platform_destroy(Platform *p) {
    XCloseDisplay(p->display);
}

PlatformWindow platform_window_open(Platform *p, int x, int y, int w, int h) {
    PlatformWindow res = {.width=w, .height=h};
    res.pixels = malloc(res.width * res.height * sizeof(uint32_t));
    res.window = XCreateSimpleWindow(p->display,
                                     p->root,
                                     x, y,
                                     w, h,
                                     1,
                                     BlackPixel(p->display, p->screen),
                                     WhitePixel(p->display, p->screen));

    XSelectInput(p->display, res.window,
                 ExposureMask | KeyPressMask | KeyReleaseMask);

    XMapWindow(p->display, res.window);
    res.image = XShmCreateImage(p->display,
                                p->visual,
                                p->depth,
                                ZPixmap,
                                (char *) res.pixels,
                                &res.info,
                                res.width, res.height);
    return res;
}

void platform_window_close(Platform *p, PlatformWindow *window) {
    free(window->pixels);
    XUnmapWindow(p->display, window->window);
}

void platform_dispatch_key(Platform *p, XKeyEvent *e, bool down) {
    KeySym sym = XLookupKeysym(e, 0);
    KeyState *state = &p->input[sym];
    state->released = state->down && !down;
    state->pressed = !state->down && down;
    state->down = down;
}

void platform_begin(Platform *p) {

    while (XPending(p->display) > 0) {
        XEvent e;
        XNextEvent(p->display, &e);

        if (e.type == KeyPress) {
            platform_dispatch_key(p, &e.xkey, true);
        } else if (e.type == KeyRelease) {
            platform_dispatch_key(p, &e.xkey, false);
        }
    }
}

void platform_end(Platform *p) {
}

bool platform_key_down(Platform *p, uint32_t keysym) {
    return p->input[keysym].down;
}

bool platform_key_pressed(Platform *p, uint32_t keysym) {
    return p->input[keysym].pressed;
}

bool platform_key_released(Platform *p, uint32_t keysym) {
    return p->input[keysym].released;
}

void platform_window_render(Platform *p, PlatformWindow *window) {
    XPutImage(p->display,
              window->window,
              p->gc,
              window->image,
              0, 0,
              0, 0,
              window->width, window->height);
}


#endif // PLATFORM_H
