#ifndef GUI_H
#define GUI_H
#include <stdbool.h>
#include <stdarg.h>
#include <framebuffer.h>
#include <vector.h>
#include <dict.h>
#include <assert.h>

typedef struct {
    int x;
    int y;
} v2;

typedef union GuiRect {
    struct {
        float x;
        float y;
        float w;
        float h;
    };
    struct {
        float E[4];
    };
} GuiRect;

typedef enum WidgetFlag {
    WIDGET_FLAG_BACKGROUND = (1 << 0),
    WIDGET_FLAG_DEBUG = (1 << 1),
    WIDGET_FLAG_TEXT = (1 << 2),
    WIDGET_FLAG_HOVERED = (1 << 3),
    WIDGET_FLAG_ACTIVE = (1 << 4),
    // TODO: Toggle flag the right place?
    WIDGET_FLAG_TOGGLE = (1 << 5),
    WIDGET_FLAG_CLICKABLE = (1 << 6),
    WIDGET_FLAG_BORDER = (1 << 7),
    WIDGET_FLAG_HOVERABLE = (1 << 8),
} WidgetFlags;

typedef char * WidgetKey;

typedef enum SizeHint {
    SIZE_HINT_NULL,
    SIZE_HINT_SUM_OF_CHILDREN,
    SIZE_HINT_LARGEST_CHILD,
    SIZE_HINT_TEXT,
} SizeHint;

typedef enum Axis {
    AXIS_X,
    AXIS_Y,
    AXIS_COUNT,
} Axis;

typedef struct Widget Widget;
struct Widget {
    Widget **children;

    WidgetFlags flags;
    char raw_key[256];
    WidgetKey key;

    GuiRect rect;
    SizeHint size_hints[AXIS_COUNT];
};

typedef struct {
    bool pressed;
    bool down;
    bool hovered;
    bool active;
} WidgetInteraction;


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
    int spacing;
    Widget *root;
    Widget **layout_stack;
    dict *widgets;

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

char *gui__get_hash(char *s) {
    char *ptr = strstr(s, "##");
    char *res = s;
    if (ptr != NULL) {
        res = ptr + 2;
    }
    return res;
}

Widget *gui__widget_create_by_name(Gui *gui, WidgetFlags flags, char *name) {
    char buffer[256] = {0};
    sprintf(&buffer[0], "%s", name);
    char *hash = gui__get_hash(&buffer[0]);
    Widget *res = dict_get(gui->widgets, hash);
    if (res == NULL) {
        res = calloc(1, sizeof(Widget));
        res->flags = flags;
        res->key = strdup(hash);
        dict_set(gui->widgets, hash, res);
    }
    strcpy(&res->raw_key[0], &buffer[0]);
    res->flags |= flags;
    return res;
}

Widget *gui__widget_create(Gui *gui, WidgetFlags flags, char *fmt, va_list args) {
    char buffer[256] = {0};
    vsprintf(&buffer[0], fmt, args);
    char *hash = gui__get_hash(&buffer[0]);
    Widget *res = dict_get(gui->widgets, hash);
    if (res == NULL) {
        res = calloc(1, sizeof(Widget));
        res->flags = flags;
        res->key = strdup(hash);
        dict_set(gui->widgets, hash, res);
    }
    strcpy(&res->raw_key[0], &buffer[0]);
    res->flags |= flags;
    return res;
}

void gui_init(Gui *gui, Framebuffer *framebuffer) {
    gui->framebuffer = framebuffer;
    gui->margin = 5;
    gui->spacing = 2;
    gui->widgets = dict_create(1024);
    // TODO: name??
    gui->root = gui__widget_create_by_name(gui, 0, "root");
    gui->root->size_hints[AXIS_X] = SIZE_HINT_LARGEST_CHILD;
    gui->root->size_hints[AXIS_Y] = SIZE_HINT_SUM_OF_CHILDREN;
    vec_push(gui->layout_stack, gui->root);
}

void gui_set_mouse(Gui *gui, int x, int y, bool down, bool released) {
    gui->mouse_x = x;
    gui->mouse_y = y;
    gui->mouse_down = down;
    gui->mouse_released = released;
}


char *gui__widget_extract_label(Widget *widget, size_t *n) {
    char *res = widget->raw_key;
    char *ptr = strstr(res, "##");
    if (ptr) {
        *n = ptr - res;
    } else {
        *n = strlen(widget->raw_key);
    }
    return res;
}


Widget *gui__widget_link_parent(Gui *gui, Widget *widget) {

    assert(vec_size(gui->layout_stack) > 0);
    Widget **parent = &gui->layout_stack[vec_size(gui->layout_stack) -1];
    Widget *p = *parent;
    vec_push(p->children, widget);
}

WidgetInteraction gui__widget_interact(Gui *gui, Widget *widget) {
    WidgetInteraction res = {0};
    bool is_in_rect = widget->rect.x <= gui->mouse_x
        && gui->mouse_x < widget->rect.x + widget->rect.w
        && widget->rect.y <= gui->mouse_y
        && gui->mouse_y < widget->rect.y + widget->rect.h;
    res.hovered = is_in_rect;
    res.down = is_in_rect && gui->mouse_down;
    res.pressed = is_in_rect && gui->mouse_released;
    if (widget->flags & WIDGET_FLAG_HOVERABLE) {
        if (res.hovered) {
            widget->flags |= WIDGET_FLAG_HOVERED;
        } else {
            widget->flags &= ~WIDGET_FLAG_HOVERED;
        }
    }
    if ((widget->flags & WIDGET_FLAG_TOGGLE) != 0) {
        if (res.pressed)
            widget->flags ^= WIDGET_FLAG_ACTIVE;
    } else if ((widget->flags & WIDGET_FLAG_CLICKABLE) != 0) {
        if (res.down) {
            widget->flags |= WIDGET_FLAG_ACTIVE;
        } else {
            widget->flags &= ~WIDGET_FLAG_ACTIVE;
        }
    }

    res.active = (widget->flags & WIDGET_FLAG_ACTIVE) != 0;
    return res;
}


WidgetInteraction gui_button(Gui *gui, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Widget *widget = gui__widget_create(gui,
                                        WIDGET_FLAG_BACKGROUND |
                                        WIDGET_FLAG_TEXT |
                                        WIDGET_FLAG_CLICKABLE |
                                        WIDGET_FLAG_BORDER |
                                        WIDGET_FLAG_HOVERABLE,
                                        fmt, args);
    widget->size_hints[AXIS_X] = SIZE_HINT_TEXT;
    widget->size_hints[AXIS_Y] = SIZE_HINT_TEXT;
    gui__widget_link_parent(gui, widget);
    WidgetInteraction action = gui__widget_interact(gui, widget);
    va_end(args);
    return action;
}

WidgetInteraction gui_button_toggle(Gui *gui, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Widget *widget = gui__widget_create(gui,
                                        WIDGET_FLAG_BACKGROUND |
                                        WIDGET_FLAG_TEXT |
                                        WIDGET_FLAG_BORDER |
                                        WIDGET_FLAG_TOGGLE,
                                        fmt, args);
    widget->size_hints[AXIS_X] = SIZE_HINT_TEXT;
    widget->size_hints[AXIS_Y] = SIZE_HINT_TEXT;
    gui__widget_link_parent(gui, widget);
    WidgetInteraction action = gui__widget_interact(gui, widget);
    va_end(args);
    return action;
}

void gui_label(Gui *gui, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Widget *widget = gui__widget_create(gui,
                                        WIDGET_FLAG_BACKGROUND | WIDGET_FLAG_TEXT,
                                        fmt, args);
    widget->size_hints[AXIS_X] = SIZE_HINT_TEXT;
    widget->size_hints[AXIS_Y] = SIZE_HINT_TEXT;
    gui__widget_link_parent(gui, widget);
    WidgetInteraction action = gui__widget_interact(gui, widget);
    va_end(args);
}

void gui_combobox(Gui *gui, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
#if 0
    Widget *label = gui__widget_create(gui,
                                        WIDGET_FLAG_BACKGROUND | WIDGET_FLAG_TEXT,
                                        fmt, args);
    gui__widget_link_parent(gui, label);

    Widget *button = gui__widget_create_by_name(gui,
                                        WIDGET_FLAG_BACKGROUND | WIDGET_FLAG_TEXT | WIDGET_FLAG_CLICKABLE,
                                        ">");
    WidgetInteraction action = gui__widget_interact(gui, button);
#endif
    va_end(args);
}

void gui_push_row(Gui *gui, char *name) {
    Widget *widget = gui__widget_create_by_name(gui,
                                                0,
                                                name);
    widget->size_hints[AXIS_X] = SIZE_HINT_SUM_OF_CHILDREN;
    widget->size_hints[AXIS_Y] = SIZE_HINT_LARGEST_CHILD;
    gui__widget_link_parent(gui, widget);
    vec_push(gui->layout_stack, widget);
}

void gui_pop_row(Gui *gui) {
    vec_pop_last(gui->layout_stack);
}

void gui__render_widget(Gui *gui, Widget *widget) {
    WidgetFlags flags = widget->flags; 
    Framebuffer *framebuffer = gui->framebuffer;
    size_t label_size = 0;
    char *label = gui__widget_extract_label(widget, &label_size);

    GuiRect rect_background = widget->rect;
    if (flags & WIDGET_FLAG_BACKGROUND) {
        framebuffer->color = gui__colors[0];
        if (flags & WIDGET_FLAG_BORDER) {
            framebuffer->color = 0xFFBBBBBB;
            framebuffer_fill_rect(framebuffer,
                                  rect_background.x, rect_background.y,
                                  rect_background.w, rect_background.h);
            rect_background.x += 1;
            rect_background.y += 1;
            rect_background.w -= 2;
            rect_background.h -= 2;
            framebuffer->color = gui__colors[0];
        }
        framebuffer_fill_rect(framebuffer,
                              rect_background.x, rect_background.y,
                              rect_background.w, rect_background.h);

    }
    if (flags & WIDGET_FLAG_DEBUG) {
        framebuffer->color = 0xFFFF0000;
        framebuffer_fill_rect(framebuffer,
                              rect_background.x, rect_background.y,
                              rect_background.w, rect_background.h);

    }
    if (flags & WIDGET_FLAG_HOVERED) {
        framebuffer->color = 0xFFFFFFFF;
        framebuffer_fill_rect(framebuffer,
                              rect_background.x, rect_background.y,
                              rect_background.w, rect_background.h);
    }
    if (flags & WIDGET_FLAG_ACTIVE) {
        framebuffer->color = gui__colors[2];
        framebuffer_fill_rect(framebuffer,
                              rect_background.x, rect_background.y,
                              rect_background.w, rect_background.h);
    }
    if (flags & WIDGET_FLAG_TEXT) {
        framebuffer->color = flags & WIDGET_FLAG_ACTIVE ? 0xFF000000 : 0xFFBBBBBB;
        framebuffer_render_textn(framebuffer,
                                 rect_background.x + gui->margin,
                                 rect_background.y + gui->margin,
                                 label, label_size);
    }
}


void gui__do_layout_func(Gui *gui, Widget *widget, void preorder(Gui *gui, Widget *widget), void postorder(Gui *gui, Widget *widget)) {

    if (preorder) preorder(gui, widget);
    for (int i = 0; i < vec_size(widget->children); ++i) {
        Widget *child = widget->children[i];
        gui__do_layout_func(gui, child, preorder, postorder);
    }
    if (postorder) postorder(gui, widget);
}

void gui__render_widget_and_children(Gui *gui, Widget *widget) {
    gui__render_widget(gui, widget);
    for (int i = 0; i < vec_size(widget->children); ++i) {
        Widget *child = widget->children[i];
        gui__render_widget_and_children(gui, child);
    }
}

void gui__layout_func_set_fixed_sizes(Gui *gui, Widget *widget) {
    if (widget->size_hints[AXIS_X] == SIZE_HINT_TEXT) {
        size_t n = 0;
        char *label = gui__widget_extract_label(widget, &n);
        int w = n * 8 + 2 * gui->margin;
        widget->rect.w = w;
    }

    if (widget->size_hints[AXIS_Y] == SIZE_HINT_TEXT) {
        // TODO: line height
        widget->rect.h = 20;
    }
}

void gui__layout_func_set_size_dependent_on_children(Gui *gui, Widget *widget) {
    for (int dimension = 0; dimension < AXIS_COUNT; ++dimension) {

        int rect_index = dimension + 2;
        if (widget->size_hints[dimension] == SIZE_HINT_SUM_OF_CHILDREN) {
            int size = gui->spacing;
            for (int i = 0; i < vec_size(widget->children); ++i) {
                Widget *child = widget->children[i];
                size += child->rect.E[rect_index] + gui->spacing;
            }
            widget->rect.E[rect_index] = size;
        } else if (widget->size_hints[dimension] == SIZE_HINT_LARGEST_CHILD) {
            int size = 0;
            for (int i = 0; i < vec_size(widget->children); ++i) {
                Widget *child = widget->children[i];
                size = MAX(size, child->rect.E[rect_index]);
            }
            //size += (vec_size(widget->children) + 1) * gui->spacing;
            widget->rect.E[rect_index] = size;
        }
    }
}
void gui__layout_func_set_xy(Gui *gui, Widget *widget) {
    for (int dimension = 0; dimension < AXIS_COUNT; ++dimension) {
        int rect_index = dimension + 2;
        int other_dimension = dimension == 0 ? 1 : 0;
        if (widget->size_hints[dimension] == SIZE_HINT_SUM_OF_CHILDREN) {
            // printf("children for -- %s dim %d\n", widget->key, dimension);
            int pos = widget->rect.E[dimension];
            for (int i = 0; i < vec_size(widget->children); ++i) {
                Widget *child = widget->children[i]; 
                child->rect.E[dimension] = pos + gui->spacing;
                // printf("    %s - pos %d\n", child->key, pos);
                pos += child->rect.E[rect_index] + gui->spacing;
                child->rect.E[other_dimension] = widget->rect.E[other_dimension];
            }
        } else if (widget->size_hints[dimension] == SIZE_HINT_LARGEST_CHILD) {
            int size = 0;
            // printf("largest for -- %s dim %d\n", widget->key, dimension);
            for (int i = 0; i < vec_size(widget->children); ++i) {
                Widget *child = widget->children[i]; 
                size = MAX(size, child->rect.E[rect_index]);
            }
            for (int i = 0; i < vec_size(widget->children); ++i) {
                Widget *child = widget->children[i]; 
                child->rect.E[rect_index] = size;
                //child->rect[other_dimension] = widget->rect[other_dimension];
                // printf("    %s - size %d == %f\n", child->key, size, child->rect[rect_index]);
            }
        }
    }


#if 0
    int x = widget->rect[0];
    int y = widget->rect[1];
    for (int i = 0; i < vec_size(widget->children); ++i) {
        Widget *child = widget->children[i]; 
        child->rect[0] = x;
        child->rect[1] = y;
        x += child->rect[2];
        y += child->rect[3];

    }
#endif
}

void gui__layout_func_clean(Gui *gui, Widget *widget) {
    vec_clear(widget->children);
    assert(vec_size(widget->children) == 0);
    //widget->rect = (const union GuiRect){0};
}

void gui__do_layout(Gui *gui, int x, int y, int w, int h) {
    gui->root->rect = (const union GuiRect){0};
    gui__do_layout_func(gui, gui->root, gui__layout_func_set_fixed_sizes, NULL);
    gui__do_layout_func(gui, gui->root, NULL, gui__layout_func_set_size_dependent_on_children);
    gui__do_layout_func(gui, gui->root, gui__layout_func_set_xy, NULL);
}

void gui_render(Gui *gui) {
    assert(vec_size(gui->layout_stack) == 1);  // NOTE: root is in the layout stack
    assert(gui->root != NULL);
    gui__do_layout(gui, 0, 0, gui->framebuffer->width, gui->framebuffer->height);
    gui__render_widget_and_children(gui, gui->root);
    gui__do_layout_func(gui, gui->root, NULL, gui__layout_func_clean);
    assert(vec_size(gui->root->children) == 0);
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
