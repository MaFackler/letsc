#include <test.h>
#include <gui.h>

TEST(gui) {
    Gui gui = {0};
    gui_init(&gui, NULL);
    CHECK(vec_size(gui.layout_stack), 1);
    char buf[256] = {0};

    const int num_rows = 3;
    for (int r = 0; r < num_rows; ++r) {
        sprintf(&buf[0], "row %d", r + 1);
        gui_push_row(&gui, &buf[0]);
        for (int c = 0; c <= r; ++c) {
            sprintf(&buf[0], "row %d col %d", r + 1, c + 1);
            gui_label(&gui, &buf[0]);
        }
        gui_pop_row(&gui);
    }

    int label_width = 98;
    int row_height = 20;

    // ACT
    CHECK(vec_size(gui.layout_stack), 1);
    gui__do_layout(&gui, 0, 0, 800, 600);

    // ASSERT
    for (int r = 0; r < num_rows; ++r) {
        TEST_MESSAGEF("- row %d", r + 1);
        Widget *row = gui.root->children[r];
        CHECK(row->rect.x, 0);
        CHECK(row->rect.y, r * row_height);
        CHECK(row->rect.w, 3 * label_width);
        CHECK(row->rect.h, row_height);
        for (int c = 0; c <= r; ++c) {
            TEST_MESSAGEF("column %d", c + 1);
            Widget *w = row->children[c];
            CHECK(w->rect.x, c * label_width);
            CHECK(w->rect.y, r * row_height);
            CHECK(w->rect.w, label_width);
            CHECK(w->rect.h, row_height);
        }
    }

    gui__do_layout_func(&gui, gui.root, NULL, gui__layout_func_clean);
    assert(vec_size(gui.root->children) == 0);


}

int main() {
    TEST_REGISTER(gui);
    TEST_RUN();
}
