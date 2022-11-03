#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <X11/Xlib.h>

#define PLATFORM_NUM_KEYS 0xFFFF
typedef struct {
    bool pressed;
    bool released;
    bool down;
} KeyState;

typedef struct {
	Display *display;
	Window root;
	int screen;
    KeyState input[PLATFORM_NUM_KEYS];
} Platform;

Platform* platform_create() {
	Platform *p = malloc(sizeof(Platform));
    p->display = XOpenDisplay(NULL);
    assert(p->display);
    p->screen = XDefaultScreen(p->display);
    p->root = XRootWindow(p->display, p->screen);
	return p;
}

void platform_destroy(Platform *p) {
    XCloseDisplay(p->display);
}

int platform_window_open(Platform *p, int x, int y, int w, int h) {
    Window window = XCreateSimpleWindow(p->display,
                                        p->root,
                                        x, y,
                                        w, h,
                                        1,
                                        BlackPixel(p->display, p->screen),
                                        WhitePixel(p->display, p->screen));

    XSelectInput(p->display, window,
                 ExposureMask | KeyPressMask | KeyReleaseMask);

    XMapWindow(p->display, window);
	return window;
}

void platform_window_close(Platform *p, int window) {
    XUnmapWindow(p->display, window);
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


#endif // PLATFORM_H
