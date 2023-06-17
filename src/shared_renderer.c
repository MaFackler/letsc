#include "gui.h"
#include <shared_api.h>
#include <vector.h>

static size_t selected = 0;
static bool collapsed = false;

typedef enum {
    LAYOUT_UNDEFINED,
    LAYOUT_VBOX,
    LAYOUT_HBOX,
    LAYOUT_VBOX_FLOAT,
    LAYOUT_HBOX_FLOAT,
} LayoutType;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} LayoutRect;

typedef struct {
    LayoutRect rect;
    size_t count;
    LayoutType type;
    float current_offset;
} Layout;

typedef enum {
    // Layouts
    LAYOUT_COMMAND_VBOX_BEGIN,
    LAYOUT_COMMAND_VBOX_END,
    // Slotting
    LAYOUT_COMMAND_SLOT,
    // widgets
    LAYOUT_COMMAND_BUTTON,
} LayoutCommand;

typedef struct {
    size_t current_widget_count;
    LayoutCommand *commands;
} Layouter;


Layout vbox(float x, float y, float w, float h, size_t count) {
    Layout res = {0, 0, 400, 400, count, LAYOUT_VBOX};
    return res;
}


LayoutRect layout_slot(Layout *layout, size_t slot_number, float userdata) {
    assert(slot_number < layout->count);
    float dw = layout->rect.width;
    float dh = layout->rect.height;
    if (layout->type == LAYOUT_HBOX) {
        dw = dw / layout->count;
        return (LayoutRect) {
            layout->rect.x + slot_number * dw,
            layout->rect.y,
            dw,
            dh
        };
    } else if (layout->type == LAYOUT_VBOX) {
        dh = dh / layout->count;
        return (LayoutRect) {
            layout->rect.x,
            layout->rect.y + slot_number * dh,
            dw,
            dh
        };
    } else if (layout->type == LAYOUT_VBOX_FLOAT) {
        LayoutRect res = {
            layout->rect.x,
            layout->rect.y + layout->current_offset,
            dw,
            MIN(userdata, dh - layout->current_offset)
        };
        layout->current_offset += userdata;
        return res;
    } else if (layout->type == LAYOUT_HBOX_FLOAT) {
        LayoutRect res = {
            layout->rect.x + layout->current_offset,
            layout->rect.y,
            MIN(userdata, dw - layout->current_offset),
            dh
        };
        layout->current_offset += userdata;
        return res;
    }
    

}

void update(SharedApi *api) {
    Framebuffer *framebuffer = api->framebuffer;
    framebuffer->stencil_value = 0x00;
    framebuffer->color = 0xFFFF0000;
#if 0
    framebuffer_fill_rect_stencil(framebuffer, 10, 10, 80, 80);
    framebuffer_fill_rect(framebuffer, 0, 0, 100, 100);

    framebuffer_fill_circle_stencil(framebuffer, 200, 200, 40);
    framebuffer_fill_circle(framebuffer, 200, 200, 50);

    framebuffer_fill_triangle_stencil(framebuffer, 420, 410, 490, 410, 490, 480);
    framebuffer_fill_triangle(framebuffer, 400, 400, 500, 400, 500, 500);
#endif
    
    char *items[] = {
        "one",
        "two",
    };
    Gui *gui = api->gui;

    Layouter layouter;
    layouter.commands = NULL;
    vec_push(layouter.commands, LAYOUT_COMMAND_VBOX_BEGIN);
        vec_push(layouter.commands, LAYOUT_COMMAND_SLOT);
        vec_push(layouter.commands, LAYOUT_COMMAND_BUTTON);


        vec_push(layouter.commands, LAYOUT_COMMAND_VBOX_BEGIN);
            vec_push(layouter.commands, LAYOUT_COMMAND_SLOT);
            vec_push(layouter.commands, LAYOUT_COMMAND_BUTTON);
            vec_push(layouter.commands, LAYOUT_COMMAND_SLOT);
            vec_push(layouter.commands, LAYOUT_COMMAND_BUTTON);
        vec_push(layouter.commands, LAYOUT_COMMAND_VBOX_END);


        vec_push(layouter.commands, LAYOUT_COMMAND_SLOT);
        vec_push(layouter.commands, LAYOUT_COMMAND_BUTTON);
        vec_push(layouter.commands, LAYOUT_COMMAND_SLOT);
        vec_push(layouter.commands, LAYOUT_COMMAND_BUTTON);
    vec_push(layouter.commands, LAYOUT_COMMAND_VBOX_END);

    Layout current_layout;
    current_layout.type = LAYOUT_UNDEFINED;
    Layout previous_layout;
    LayoutCommand *widgets_to_render = NULL;

    size_t slot_index = 0;
    for (size_t i = 0; i < vec_size(layouter.commands); ++i) {
        switch (layouter.commands[i]) {
            case LAYOUT_COMMAND_VBOX_BEGIN:
                previous_layout = current_layout;
                if (previous_layout.type == LAYOUT_UNDEFINED)
                    current_layout = vbox(0, 0, 500, 500, 3);
                else:
                    current_layout = vbox(previ
                break;
            case LAYOUT_COMMAND_VBOX_END:
                assert(vec_size(widgets_to_render) == slot_index);
                current_layout.count = slot_index;
                for (size_t j = 0; j < vec_size(widgets_to_render); ++j) {
                    LayoutRect rect = layout_slot(&current_layout, j, 0);
                    gui_render_button_by_rect(gui, rect.x, rect.y, rect.width, rect.height, "test");
                }
                vec_clear(widgets_to_render);
                slot_index = 0;
                current_layout = previous_layout;
                break;
            case LAYOUT_COMMAND_SLOT:
                assert(current_layout.type != LAYOUT_UNDEFINED);
                slot_index++;
                break;
            case LAYOUT_COMMAND_BUTTON:
                vec_push(widgets_to_render, LAYOUT_COMMAND_BUTTON);
                break;
            default:
                assert(!"INVALID");

            
        }
    }

    vec_free(layouter.commands);
    vec_free(widgets_to_render);

#if 0
    Layout layout = vbox(0, 0, 400, 400, 3);
        LayoutRect rect = layout_slot(&layout, 0, 0);
        framebuffer->color = 0x000000;
        //framebuffer_fill_rect(framebuffer, rect.x, rect.y, rect.width, rect.height);
        gui_render_button_by_rect(gui, rect.x, rect.y, rect.width, rect.height, "test");

        rect = layout_slot(&layout, 1, 0);
        Layout inner = {rect.x, rect.y, rect.width, rect.height, 3, LAYOUT_HBOX};
            rect = layout_slot(&inner, 0, 0);
            framebuffer->color = 0xFF0000;
            framebuffer_fill_rect(framebuffer, rect.x, rect.y, rect.width, rect.height);

            rect = layout_slot(&inner, 1, 0);
            framebuffer->color = 0x0000FF;
            framebuffer_fill_rect(framebuffer, rect.x, rect.y, rect.width, rect.height);

            rect = layout_slot(&inner, 2, 0);
            framebuffer->color = 0xFF0000;
            framebuffer_fill_rect(framebuffer, rect.x, rect.y, rect.width, rect.height);

        rect = layout_slot(&layout, 2, 0);
        framebuffer->color = 0xFFFF00;
        framebuffer_fill_rect(framebuffer, rect.x, rect.y, rect.width, rect.height);

        Layout floating = {rect.x, rect.y, rect.width, rect.height, 3, LAYOUT_VBOX_FLOAT};

            rect = layout_slot(&floating, 0, 25);
            framebuffer->color = 0xFFFFFF;
            gui_render_button_by_rect(gui, rect.x, rect.y, rect.width, rect.height, "one");

            rect = layout_slot(&floating, 1, 200);
            framebuffer->color = 0x00FFFF;
            gui_render_button_by_rect(gui, rect.x, rect.y, rect.width, rect.height, "two");

    float x = 200;
    float y = 100;
#endif

    //gui_render_list(gui, x, y, items, sizeof(items) / sizeof(items[0]), &selected);
    //gui_render_combobox(gui, x, y, items, sizeof(items) / sizeof(items[0]), &selected, &collapsed);
}
