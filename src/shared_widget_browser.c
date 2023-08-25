#include "gui.h"
#include <shared_api.h>

int lerpc(int from, int to, float t) {
    int32_t r1 = COLOR_EXTRACT_RED(from);
    int32_t g1 = COLOR_EXTRACT_GREEN(from);
    int32_t b1 = COLOR_EXTRACT_BLUE(from);
    int32_t r2 = COLOR_EXTRACT_RED(to);
    int32_t g2 = COLOR_EXTRACT_GREEN(to);
    int32_t b2 = COLOR_EXTRACT_BLUE(to);

    int32_t r3 = r1 + (r2 - r1) * t;
    int32_t g3 = g1 + (g2 - g1) * t;
    int32_t b3 = b1 + (b2 - b1) * t;
    return 0xFF000000 | MIN(r3, 255) << 16 | MIN(g3, 255) << 8 | MIN(b3, 255) << 0;
}


void update(SharedApi *api) {
    Gui *gui = api->gui;
    char buf[256] = {0};

#if 0
    const int num_rows = 3;
    for (int r = 0; r < num_rows; ++r) {
        sprintf(&buf[0], "row %d", r + 1);
        gui_push_row(gui, &buf[0]);
        for (int c = 0; c <= r; ++c) {
            sprintf(&buf[0], "row %d col %d", r + 1, c + 1);
            if (c < r) {
                gui_label(gui, &buf[0]);
            } else {
                if (gui_button(gui, &buf[0]).down) {
                    COLOR(RED);
                    FILL_RECT(200, 200, 100, 100);
                }
            }
        }
        gui_pop(gui);
    }
#endif

    int col_size = 199;

#if 0
    gui_push_row(gui, "header");
    gui_label(gui, "Widget Type");
    gui_label(gui, "Widget Demo");
    gui_pop(gui);

    gui_push_row(gui, "label");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "Label");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "MyLabel");
    gui_pop(gui);

    gui_push_row(gui, "button");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "Button");
    gui_button_hintx(gui, SIZE_PIXELS(col_size), "MyButton");
    gui_pop(gui);

    gui_push_row(gui, "toggle button");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "Toggle Button");
    gui_button_toggle_hintx(gui, SIZE_PIXELS(col_size), "MyToggleButton");
    gui_pop(gui);
#endif

    static char *items[] = {
        "item1",
        "item2",
        "item3"
    };
    static size_t index = 0;

    GUI_PANEL(gui, "debug panel", 0, 0) {
        GUI_ROW(gui, "combobox_row") {
            gui_label(gui, "Combobox");
            if (gui_combobox(gui, &items[0], 3, &index, "test", &index).pressed) {
                printf("Pressed %s\n", items[index]);
            }
        }


        static bool checked = false;
        GUI_ROW(gui, "checkbox_row") {
            gui_label(gui, "Checkbox");
            if (gui_checkbox(gui, &checked, "My Checkbox").pressed) {
                printf("Pressed checkbox\n");
            }
        }

        static float value = -90.0f;
        GUI_ROW(gui, "slider_row") {
            gui_label(gui, "Slider");
            if (gui_slider(gui, &value, -100, 100, "My Slider").pressed) {
            }
        }
    }


    GUI_PANEL(gui, "mypanel", 0, 400) {
        gui_label(gui, "Panel Label");
    }

#if 0
    int c = 0xFF1C4E80;
    //int c = 0xfb4934;
    int cdark = lerpc(c, BLACK, 0.4);
    int clight = lerpc(c, WHITE, 0.2);
    //printf("0x%x\n", clight);
    COLOR(c);
    int x = 100;
    int y = 100;
    const int w = 200;
    const int h = 30;
    FILL_RECT(x, y, w, h);
    COLOR(clight);
    FILL_RECT(x, y, 1, h);
    FILL_RECT(x, y, w, 1);

    COLOR(cdark);
    FILL_RECT(x + w - 1, y, 1, h);
    FILL_RECT(x, y + h - 1, w, 1);

    COLOR(BLACK);
    FILL_RECT(x - 1, y - 1, w + 2, 1);
    FILL_RECT(x - 1, y - 1, 1, h + 2);

    FILL_RECT(x - 1, y + h, w + 2, 1);
    FILL_RECT(x + w, y - 1, 1, h + 2);

    char *text = "Hello World!";
    COLOR(0xFF222222);
    TEXT(x + 60 + 1, y + 10 + 2, text);
    COLOR(0xFFCCCCCC);
    TEXT(x + 60, y + 10, text);
#endif
}
