#include <shared_api.h>
#include <mathematics.h>

void render_cube_side(Framebuffer *framebuffer, m4 transform, v3 a, v3 b, v3 c, v3 d) {
    a = m4_transform(transform, a);
    b = m4_transform(transform, b);
    c = m4_transform(transform, c);
    d = m4_transform(transform, d);
    v3 normal = v3_cross(v3_direction(a, b), v3_direction(a, c));
    float value = v3_dot(normal, (v3) {0.0f, 0.0f, 1.0f});
    if (value > 0) {

        framebuffer_fill_triangle(framebuffer,
                                  a.x, a.y,
                                  b.x, b.y,
                                  c.x, c.y);
        framebuffer_fill_triangle(framebuffer,
                                  a.x, a.y,
                                  c.x, c.y,
                                  d.x, d.y);
    }
}

void update(SharedApi *api) {
    Framebuffer *framebuffer = api->framebuffer;

    float r = 50.0f;
    v3 a = {-r, -r, -r};
    v3 b = {r, -r, -r};
    v3 c = {r, r, -r};
    v3 d = {-r, r, -r};

    v3 ab = {-r, -r, r};
    v3 bb = {r, -r, r};
    v3 cb = {r, r, r};
    v3 db = {-r, r, r};

    static float angle = 0.0f;
    static bool animate = false;
    float max = 360.0f;
    float min = 0.0f;
    Gui *gui = api->gui;
    const int colsize = 100;
    GUI_PANEL(gui, "values_panel", 0, 0) {
        GUI_ROW(gui, "animate_row") {
            GUI_SIZE_HINT(gui, AXIS_X, SIZE_PIXELS(colsize)) {
                gui_label(api->gui, "Animate");
                if (gui_checkbox(api->gui, &animate, "chk_animate").active) {
                    angle += 0.5f;
                    if (angle > max) {
                        angle -= max;
                    }
                }
            }
        }
        GUI_ROW(gui, "angle_row") {
            GUI_SIZE_HINT(gui, AXIS_X, SIZE_PIXELS(colsize)) {
                gui_label(api->gui, "Angle");
                if (gui_slider(api->gui, &angle, min, max, "slider_animate").active) {
                }
            }
        }
    }
    m4 transform = m4_create_identity();
    m4 scale = m4_create_scale(2.0f, 2.0f, 2.0f);
    m4 rotate_x = m4_create_rotation_x(angle);
    m4 rotate_y = m4_create_rotation_y(angle);
    m4 rotate_z = m4_create_rotation_y(angle);
    m4 translate = m4_create_translate(400.0f, 300.0f, 0.0f);

    // NOTE: SCALE - ROTATE - TRANSLATE
    transform = m4_mul(transform, scale);
    transform = m4_mul(transform, rotate_x);
    transform = m4_mul(transform, rotate_y);
    transform = m4_mul(transform, rotate_z);
    transform = m4_mul(transform, translate);

    // front
    framebuffer->color = 0xFFFF0000;
    render_cube_side(framebuffer, transform, a, b, c, d);
    // back
    framebuffer->color = 0xFF00FF00;
    render_cube_side(framebuffer, transform, bb, ab, db, cb);
    // right
    framebuffer->color = 0xFF0000FF;
    render_cube_side(framebuffer, transform, b, bb, cb, c);
    // left
    framebuffer->color = 0xFFFFFF00;
    render_cube_side(framebuffer, transform, a, d, db, ab);
    // top
    framebuffer->color = 0xFF00FFFF;
    render_cube_side(framebuffer, transform, ab, bb, b, a);
    // bottom
    framebuffer->color = 0xFFFF00FF;
    render_cube_side(framebuffer, transform, d, c, cb, db);

}
