#include <shared_api.h>
#include <mathematics.h>

static float animation_value = 0;

void draw_part(Framebuffer *framebuffer, m4 translate, m4 rotate, float len) {
    v3 start = {0.0f, 0.0f, 0.0f};
    m4 translate_end = m4_create_translate(0, -len, 0);
    v3 end = m4_transform(translate_end, start);

    m4 t = m4_mul(rotate, translate);
    start = m4_transform(t, start);
    end = m4_transform(t, end);
    framebuffer->color = 0xFFFFFFFF;
    if (len > 10) {
        framebuffer->color = 0x8f2d00;
    } else {
        framebuffer->color = 0x438f00;
    }
    framebuffer_draw_line(framebuffer, start.x, start.y, end.x, end.y);

    float new_len = len * 0.75;
    if (fabs(new_len) > 2) {
        m4 rotate_left = m4_create_rotation_z(-20 * (sin(animation_value) + 1.5f));
        rotate_left = m4_mul(rotate, rotate_left);
        draw_part(framebuffer, m4_create_translate(end.x, end.y, 0), rotate_left, new_len);

        m4 rotate_right = m4_create_rotation_z(20 * (cos(animation_value) + 1.5f));
        rotate_right = m4_mul(rotate, rotate_right);
        draw_part(framebuffer, m4_create_translate(end.x, end.y, 0), rotate_right, new_len);
    }
}

void update(SharedApi *api) {
    Framebuffer *framebuffer = api->framebuffer;
    Platform *p = api->platform;
    m4 translate_bottom = m4_create_translate(400, 500, 0);
    animation_value += 0.01;
    if (animation_value > 360.0f) {
        animation_value -= 360.0f;
    }

    float len = 100.0f;
    draw_part(framebuffer, translate_bottom, m4_create_identity(), len);
}
