#include "gui.h"
#include <shared_api.h>


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
        gui_pop_row(gui);
    }
#endif

    int col_size = 200;

#if 0
    gui_push_row(gui, "header");
    gui_label(gui, "Widget Type");
    gui_label(gui, "Widget Demo");
    gui_pop_row(gui);

    gui_push_row(gui, "label");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "Label");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "MyLabel");
    gui_pop_row(gui);

    gui_push_row(gui, "button");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "Button");
    gui_button_hintx(gui, SIZE_PIXELS(col_size), "MyButton");
    gui_pop_row(gui);

    gui_push_row(gui, "toggle button");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "Toggle Button");
    gui_button_toggle_hintx(gui, SIZE_PIXELS(col_size), "MyToggleButton");
    gui_pop_row(gui);
#endif

    gui_push_row(gui, "combobox");
    gui_label_hintx(gui, SIZE_PIXELS(col_size), "Combobox");
    if (gui_combobox(gui, "test").pressed) {
        printf("pressed\n");
    }
    gui_pop_row(gui);


    gui_render(gui);
}
