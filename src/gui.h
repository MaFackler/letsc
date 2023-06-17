#ifndef GUI_H
#define GUI_H
#include <stdbool.h>
#include <framebuffer.h>

typedef struct {
    int x;
    int y;
} v2;

unsigned int gui__colors[] = {
    0xFF1C4E80,
    0xFF0091D5,
    0xFF4CB5F5,
};

typedef enum GuiWidgetState {
    GUI_WIDGET_STATE_NOT_HOVERED,
    GUI_WIDGET_STATE_HOVERED,
    GUI_WIDGET_STATE_ACTIVE,
} GuiWidgetState;

typedef struct {
    Framebuffer *framebuffer;

    bool mouse_down;
    bool mouse_released;
    int mouse_x;
    int mouse_y;
    int margin;
} Gui;

v2 gui__get_text_dim(Gui *gui, char *text) {
    int x = font8x8_get_text_width(&gui->framebuffer->font, text);
    return (v2) {x, 8};
}

v2 gui__get_texts_dim(Gui *gui, char *texts[], size_t n) {
    assert(n > 0);
    v2 dim = {0, 0};
    for (int i = 0; i < n; ++i) {
        v2 d = gui__get_text_dim(gui, texts[i]);
        dim.x = MAX(dim.x, d.x);
        dim.y = MAX(dim.y, d.y);
    }
    return dim;
}

v2 gui__advance_margin(Gui *gui, v2 dim) {
    return (v2) {dim.x + 2 * gui->margin, dim.y + 2 * gui->margin};
}

void gui_init(Gui *gui, Framebuffer *framebuffer) {
    gui->framebuffer = framebuffer;
    gui->margin = 2;
}

void gui_set_mouse(Gui *gui, int x, int y, bool down, bool released) {
    gui->mouse_x = x;
    gui->mouse_y = y;
    gui->mouse_down = down;
    gui->mouse_released = released;
}


GuiWidgetState gui__check_rect(Gui *gui, int xmin, int xmax, int ymin, int ymax) {
    bool is_in_rect = xmin <= gui->mouse_x && gui->mouse_x < xmax && ymin <= gui->mouse_y && gui->mouse_y < ymax;
    GuiWidgetState res = GUI_WIDGET_STATE_NOT_HOVERED;
    if (is_in_rect) {
        res = GUI_WIDGET_STATE_HOVERED;
        if (gui->mouse_released) {
            res = GUI_WIDGET_STATE_ACTIVE;
        }
    }
    return res;
}

void gui__set_color(Gui *gui, GuiWidgetState state) {
    gui->framebuffer->color = gui__colors[state];
}

bool gui__state_to_bool(GuiWidgetState state) {
    return state == GUI_WIDGET_STATE_ACTIVE;
}

bool gui_render_button_by_rect(Gui *gui, float x, float y, float w, float h, char *text) {
    bool res = false;
    v2 dim = {w, h};
    v2 pos = {x, y};
    GuiWidgetState state = gui__check_rect(gui, pos.x, pos.x + dim.x, pos.y, pos.y + dim.y);
    gui__set_color(gui, state);
    framebuffer_fill_rect(gui->framebuffer, pos.x, pos.y, dim.x, dim.y);
    gui->framebuffer->color = 0xFFFFFFFF;
    // TODO: text clamp
    framebuffer_render_text(gui->framebuffer, pos.x + gui->margin, pos.y + gui->margin, text);
    return gui__state_to_bool(state);
}

bool gui_render_button(Gui *gui, int x, int y, char *text, unsigned int color) {
    bool res = false;
    v2 dim = gui__get_text_dim(gui, text);
    dim.x += 2 * gui->margin;
    dim.y += 2 * gui->margin;
    v2 pos = {x, y};
    GuiWidgetState state = gui__check_rect(gui, pos.x, pos.x + dim.x, pos.y, pos.y + dim.y);
    gui__set_color(gui, state);
    framebuffer_fill_rect(gui->framebuffer, pos.x, pos.y, dim.x, dim.y);
    gui->framebuffer->color = 0xFFFFFFFF;
    framebuffer_render_text(gui->framebuffer, pos.x + gui->margin, pos.y + gui->margin, text);
    return gui__state_to_bool(state);
}

bool gui_render_list(Gui *gui, int x, int y, char *texts[], size_t n, size_t *selected) {
    bool res = false;
    v2 dim = gui__get_texts_dim(gui, texts, n);
    dim = gui__advance_margin(gui, dim);
    v2 pos = {x, y};
    for (int i = 0; i < n; ++i) {
        GuiWidgetState state = gui__check_rect(gui, pos.x, pos.x + dim.x, pos.y, pos.y + dim.y);
        if (state == GUI_WIDGET_STATE_ACTIVE) {
            *selected = i;
            res = true;
        }
        if (i == *selected) {
            state = GUI_WIDGET_STATE_ACTIVE;
        }
        gui__set_color(gui, state);
        framebuffer_fill_rect(gui->framebuffer, pos.x, pos.y, dim.x, dim.y);
        gui->framebuffer->color = 0xFFFFFFFF;
        framebuffer_render_text(gui->framebuffer, pos.x + gui->margin, pos.y + gui->margin, texts[i]);
        pos.y += dim.y;
    }
    return res;
}

bool gui_render_combobox(Gui *gui, int x, int y, char *texts[], size_t n, size_t *selected, bool *collapsed) {
    bool res = false;
    char *text = texts[*selected];
    v2 pos = {x, y};
    v2 dim = gui__get_texts_dim(gui, texts, n);
    dim = gui__advance_margin(gui, dim);
    gui__set_color(gui, GUI_WIDGET_STATE_NOT_HOVERED);
    v2 dim_button = gui__get_text_dim(gui, "-");
    dim_button = gui__advance_margin(gui, dim_button);
    assert(dim_button.y == dim.y);
    framebuffer_fill_rect(gui->framebuffer, pos.x, pos.y, dim.x + dim_button.x, dim.y); 

    gui->framebuffer->color = 0xFFFFFFFF;
    framebuffer_render_text(gui->framebuffer, pos.x + gui->margin, pos.y + gui->margin, text);
    if (gui_render_button(gui, pos.x + dim.x, pos.y, *collapsed ? "|" : "-", 0)) {
        *collapsed = !*collapsed;
    }

    pos.y += dim.y;
    if (*collapsed) {
        res = gui_render_list(gui, pos.x, pos.y, texts, n, selected);
        if (res) {
            *collapsed = false;
        }
    }
    return res;
}


#endif // GUI_H
