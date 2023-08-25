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
        gui_pop(&gui);
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
    char *items[] = {
        "one",
        "two"
    };
    size_t index = 0;
    gui_combobox(&gui, &items[0], sizeof(items), &index, "my combobox");
    gui__do_layout(&gui, 0, 0, 800, 600);
    CHECK(vec_size(gui.root->children), 1);

    Widget *combobox = gui.root->children[0];
    CHECK(vec_size(combobox->children), 2);
    CHECK(combobox->rect.x, 0);
    CHECK(combobox->rect.y, 0);
    CHECK(combobox->rect.w, 200);
    CHECK(combobox->rect.h, 30);
    Widget *label = combobox->children[0];
    CHECK(label->rect.x, 0);
    CHECK(label->rect.y, 0);
    CHECK(label->rect.w, 180);
    CHECK(label->rect.h, 30);

    Widget *button = combobox->children[1];
    CHECK(button->rect.x, 180);
    CHECK(button->rect.y, 0);
    CHECK(button->rect.w, 20);
    CHECK(button->rect.h, 30);
    // TODO: test dropdown
#if 0
    Widget *dropdown = label->children[0];
    CHECK(dropdown->rect.x, 78);
    CHECK(dropdown->rect.y, 20);
#endif

#if 0

    gui_push_row(&gui, "myrow");
    gui_label_hintx(&gui, SIZE_PIXELS(300), "my label");
    gui_combobox(&gui, "my combobox");
    //gui_label(&gui, "my label");
    //gui_label(&gui, "my combobox");
    gui_pop(&gui);

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
#endif

}

TEST(gui_slider) {
    Gui gui = {0};
    gui_init(&gui, NULL);
    //gui_push_row(&gui, "myrow");
    float myvalue = 0;
    float minvalue = 0;
    float maxvalue = 100;
    gui_slider(&gui, &myvalue, minvalue, maxvalue, "my slider");

    gui__do_layout(&gui, 0, 0, 800, 600);

    TEST_MESSAGE("Check Slider");
    Widget *slider = gui.root->children[0];
    CHECK(slider->key, "my slider");
    CHECK(slider->rect.x, 0);
    CHECK(slider->rect.y, 0);
    CHECK(slider->rect.w, 200);
    CHECK(slider->rect.h, 30);



    TEST_MESSAGE("Check Slider Knob");
    Widget *knob = slider->children[0];
    CHECK(knob->key, "my slider_knob");
    CHECK(knob->rect.x, 0);
    CHECK(knob->rect.y, 5);
    CHECK(knob->rect.w, 20);
    CHECK(knob->rect.h, 20);

#if 0
    gui__end(&gui);
    myvalue = 100; 
    // Test slider value changed
    gui_slider(&gui, &myvalue, minvalue, maxvalue, "my slider");
    gui__do_layout(&gui, 0, 0, 800, 600);
    CHECK(knob->key, "my slider_knob");
    CHECK(knob->rect.x, 130);
    CHECK(knob->rect.y, 2);
    CHECK(knob->rect.w, 26);
    CHECK(knob->rect.h, 26);
#endif
}

TEST(gui_panel) {
    Gui gui = {0};
    gui_init(&gui, NULL);

    gui_push_panel(&gui, "mypanel1", 200, 0);
    gui_label(&gui, "panel1 label");
    gui_pop(&gui);

    gui_push_panel(&gui, "mypanel2", 0, 300);
    gui_label(&gui, "panel2 label");
    gui_pop(&gui);

    gui__do_layout(&gui, 0, 0, 800, 600);
    Widget *panel1 = gui.root->children[0];
    CHECK(vec_size(panel1->children), 1);
    CHECK(panel1->rect.x, 200);
    CHECK(panel1->rect.y, 0);
    CHECK(panel1->rect.w, 106);
    CHECK(panel1->rect.h, 30);
    Widget *panel1label = panel1->children[0];
    CHECK(panel1label->rect.x, 200);
    CHECK(panel1label->rect.y, 0);
    CHECK(panel1label->rect.w, 106);
    CHECK(panel1label->rect.h, 30);


    Widget *panel2 = gui.root->children[1];
    CHECK(panel2->rect.x, 0);
    CHECK(vec_size(panel2->children), 1);
    CHECK(panel2->rect.x, 0);
    CHECK(panel2->rect.y, 300);
    CHECK(panel2->rect.w, 106);
    CHECK(panel2->rect.h, 30);
    Widget *panel2label = panel2->children[0];
    CHECK(panel2label->rect.x, 0);
    CHECK(panel2label->rect.y, 300);
    CHECK(panel2label->rect.w, 106);
    CHECK(panel2label->rect.h, 30);


}

TEST_MAIN() {
    // TODO: refactor this to layout test or something
    // TEST_REGISTER(gui);
    TEST_REGISTER(gui_combobox);
    // TEST_REGISTER(gui_slider);
    // TEST_REGISTER(gui_panel);
    TEST_RUN();
}
