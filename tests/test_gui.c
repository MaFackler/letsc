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
        CHECK(row->rect.y, r * row_height + gui.spacing * (r + 1));
        CHECK(row->rect.w, 3 * (label_width + gui.spacing) + gui.spacing);
        CHECK(row->rect.h, row_height);
        for (int c = 0; c <= r; ++c) {
            TEST_MESSAGEF("column %d", c + 1);
            Widget *w = row->children[c];
            CHECK(w->rect.x, c * (label_width + gui.spacing) + gui.spacing);
            CHECK(w->rect.y, r * (row_height + gui.spacing) + gui.spacing);
            CHECK(w->rect.w, label_width);
            CHECK(w->rect.h, row_height);
        }
    }

    gui__do_layout_func(&gui, gui.root, NULL, gui__layout_func_clean);
    assert(vec_size(gui.root->children) == 0);

}

TEST(gui_combobox) {
    Gui gui = {0};
    gui_init(&gui, NULL);

    gui_push_row(&gui, "myrow");
    gui_label_hintx(&gui, SIZE_PIXELS(300), "my label");
    gui_combobox(&gui, "my combobox");
    //gui_label(&gui, "my label");
    //gui_label(&gui, "my combobox");
    gui_pop_row(&gui);

    gui__do_layout(&gui, 0, 0, 800, 600);
    CHECK(vec_size(gui.root->children), 1);

    Widget *row = gui.root->children[0];
    CHECK(vec_size(row->children), 2);
    // label
    TEST_MESSAGE("Asser label");
    Widget *label = row->children[0];
    CHECK(label->rect.x, gui.spacing);
    CHECK(label->rect.y, 0);
    CHECK(label->rect.w, 300);
    CHECK(label->rect.h, 20);
    // combobox
    TEST_MESSAGE("Assert combobox");
    Widget *cmb = row->children[1];
    CHECK(vec_size(cmb->children), 2);
    CHECK(cmb->rect.x, gui.spacing + 300 + gui.spacing);
    CHECK(cmb->rect.y, 0);
    CHECK(cmb->rect.w, 300); // TODO: spacing should be ignored
    CHECK(cmb->rect.h, 20);

    TEST_MESSAGE("Assert combobox label");
    Widget *cmb_label = cmb->children[0];
    CHECK(cmb_label->rect.x,  304);  // TODO:
    CHECK(cmb_label->rect.y, gui.spacing);
    CHECK(cmb_label->rect.w, 200);
    CHECK(cmb_label->rect.h, 20);

    TEST_MESSAGE("Assert combobox button");
    Widget *cmb_button = cmb->children[1];
    CHECK(cmb_button->rect.x, gui.spacing + 504 + gui.spacing);  // TODO:
    CHECK(cmb_button->rect.y, gui.spacing);
    CHECK(cmb_button->rect.w, 100);
    CHECK(cmb_button->rect.h, 20);

}

int main() {
    //TEST_REGISTER(gui);
    TEST_REGISTER(gui_combobox);
    TEST_RUN();
}
