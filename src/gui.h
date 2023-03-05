#ifndef GUI_H
#define GUI_H

typedef struct {
    int x;
    int y;
} v2;

typedef enum GuiWidgetState {
    GUI_WIDGET_STATE_NOT_HOVERED,
    GUI_WIDGET_STATE_HOVERED,
    GUI_WIDGET_STATE_ACTIVE,
} GuiWidgetState;

typedef struct {
    Framebuffer *framebuffer;

    int mouse_down;
    int mouse_x;
    int mouse_y;
} Gui;

v2 gui__get_text_dim(Gui *gui, char *text) {
    int x = font8x8_get_text_width(&gui->framebuffer->font, text);
    return (v2) {x, 8};
}

void gui_init(Gui *gui, Framebuffer *framebuffer) {
    gui->framebuffer = framebuffer;
}

void gui_set_mouse(Gui *gui, int x, int y, bool down) {
    gui->mouse_x = x;
    gui->mouse_y = y;
    gui->mouse_down = down;
}


GuiWidgetState gui__check_rect(Gui *gui, int xmin, int xmax, int ymin, int ymax) {
    bool is_in_rect = xmin <= gui->mouse_x && gui->mouse_x <= xmax && ymin <= gui->mouse_y && gui->mouse_y <= ymax;
    GuiWidgetState res = GUI_WIDGET_STATE_NOT_HOVERED;
    if (is_in_rect) {
        res = GUI_WIDGET_STATE_HOVERED;
        if (gui->mouse_down) {
            res = GUI_WIDGET_STATE_ACTIVE;
        }
    }
    return res;
}

unsigned int gui__set_color(Gui *gui, GuiWidgetState state) {
    unsigned int color = 0;
    switch (state) {
        case GUI_WIDGET_STATE_HOVERED:
            color = 0xFF0091D5;
            break;
        case GUI_WIDGET_STATE_NOT_HOVERED:
            color = 0xFF1C4E80;
            break;
        case GUI_WIDGET_STATE_ACTIVE:
            color = 0xFF4CB5F5;
            break;
            
    }
    assert(color != 0);
    gui->framebuffer->color = color;
}

bool gui__state_to_bool(GuiWidgetState state) {
    return state == GUI_WIDGET_STATE_ACTIVE;
}


bool gui_render_button(Gui *gui, int x, int y, char *text, unsigned int color) {
    bool res = false;
    v2 dim = gui__get_text_dim(gui, text);
    int margin = 2;
    dim.x += 2 * margin;
    dim.y += 2 * margin;
    v2 pos = {x, y};
    GuiWidgetState state = gui__check_rect(gui, pos.x, pos.x + dim.x, pos.y, pos.y + dim.y);
    gui__set_color(gui, state);
    framebuffer_fill_rect(gui->framebuffer, pos.x, pos.y, dim.x, dim.y);
    gui->framebuffer->color = 0xFFFFFFFF;
    framebuffer_render_text(gui->framebuffer, pos.x + margin, pos.y + margin, text);
    return gui__state_to_bool(state);
}


#endif // GUI_H
