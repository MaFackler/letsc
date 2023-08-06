#include "gui.h"
#include <shared_api.h>


void update(SharedApi *api) {
    Gui *gui = api->gui;
    char buf[256] = {0};
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

    COLOR(0xFFFFFFFF);
    FILL_RECT(0, 100, 100, 10);
    COLOR(0xFFFF0000);
    FILL_RECT(101, 100, 100, 10);

    gui_render(gui);
}
