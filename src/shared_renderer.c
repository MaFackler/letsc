#include "gui.h"
#include <shared_api.h>
#include <vector.h>
#include <dict.h>

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
    
    char *items[] = {
        "one",
        "two",
    };
    Gui *gui = api->gui;
#if 0

    static float result = 0;
    static float display_value = 0;
    static char display[256] = {0};
    static size_t display_index = 0;
    static bool use_fraction = false;
    static char operator = '\0';

    void reset_display() {
        display_value = atof(&display[0]);
        memset(&display[0], 0, sizeof(display));
        display_index = 0;
    };

    gui_row(gui);
    if (gui_button(gui, "7").pressed) {
        display[display_index++] = '7';
    }
    if (gui_button(gui, "8").pressed) {
        display[display_index++] = '8';
    }
    if (gui_button(gui, "9").pressed) {
        display[display_index++] = '9';
    }
    if (gui_button(gui, "/").pressed) {
        operator = '/';
        reset_display();
    }
    gui_row(gui);
    if (gui_button(gui, "4").pressed) {
        display[display_index++] = '4';
    }
    if (gui_button(gui, "5").pressed) {
        display[display_index++] = '5';
    }
    if (gui_button(gui, "6").pressed) {
        display[display_index++] = '6';
    }
    if (gui_button(gui, "*").pressed) {
        operator = '*';
        reset_display();
    }

    gui_row(gui);
    if (gui_button(gui, "1").pressed) {
        display[display_index++] = '1';
    }
    if (gui_button(gui, "2").pressed) {
        display[display_index++] = '2';
    }
    if (gui_button(gui, "3").pressed) {
        display[display_index++] = '3';
    }
    if (gui_button(gui, "-").pressed) {
        operator = '-';
        reset_display();
    }

    gui_row(gui);
    if (gui_button(gui, "0").pressed) {
        display[display_index++] = '0';
    }
    if (gui_button(gui, ".").pressed) {
        display[display_index++] = '.';
    }
    if (gui_button(gui, "=").pressed) {
        if (operator) {
            switch (operator) {
                case '+':
                    result = display_value + atof(&display[0]);
                    break;
                case '-':
                    result = display_value - atof(&display[0]);
                    break;
                case '*':
                    result = display_value * atof(&display[0]);
                    break;
                case '/':
                    result = display_value / atof(&display[0]);
                    break;
                default:
                    assert(!"INVALID");
                    break;
            }
            sprintf(&display[0], "%.2f", result);
            display_value = 0;
        }
    }
    if (gui_button(gui, "+").pressed) {
        operator = '+';
        reset_display();
    }

    if (gui_button(gui, "C").pressed) {
        operator = '\0';
        result = 0;
        reset_display();
        display_value = 0;
    }

    gui_row(gui);
    static char result_buffer[256] = {0};
    gui_label(gui, "Current: %s##display", &display[0]);
    gui_render(gui);
#endif

}
