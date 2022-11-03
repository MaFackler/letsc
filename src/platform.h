#ifndef PLATFORM_H
#define PLATFORM_H

#include <X11/Xlib.h>

typedef struct {
	Display *display;
	Window root;
	int screen;
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

#endif // PLATFORM_H
