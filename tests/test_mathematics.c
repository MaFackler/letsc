#include <test.h>
#include <mathematics.h>

TEST(m4_create_identity) {
    m4 res = m4_create_identity();
    // TODO: maybe check for 1.0f;
    CHECK(res.e[0], 1.0f);
    CHECK(res.e[5], 1.0f);
    CHECK(res.e[10], 1.0f);
    CHECK(res.e[15], 1.0f);

    v3 a = {1.0f, 2.0f, 3.0f};
    v3 vres = m4_transform(res, a);
    CHECK(vres.x, 1.0f);
    CHECK(vres.y, 2.0f);
    CHECK(vres.z, 3.0f);
}

TEST(m4_create_scale) {
    m4 res = m4_create_scale(1.0f, 2.0f, 3.0f);
    // TODO: maybe check for 1.0f;
    CHECK(res.e[0], 1.0f);
    CHECK(res.e[5], 2.0f);
    CHECK(res.e[10], 3.0f);
    CHECK(res.e[15], 1.0f);

    v3 a = {1.0f, 2.0f, 3.0f};
    v3 vres = m4_transform(res, a);
    CHECK(vres.x, 1.0f);
    CHECK(vres.y, 4.0f);
    CHECK(vres.z, 9.0f);
}

TEST(m4_create_rotation_x) {
    m4 res = m4_create_rotation_x(90.0f);
    // TODO: maybe check for 1.0f;
    // TODO: assert actual sin/cos values

    v3 a = {0.0f, 1.0f, 0.0f};
    v3 vres = m4_transform(res, a);
    CHECK(vres.x, 0.0f);
    CHECK(vres.y, -0.0f);
    CHECK(vres.z, 1.0f);
}

TEST(m4_create_rotation_y) {
    m4 res = m4_create_rotation_y(90.0f);
    // TODO: maybe check for 1.0f;
    // TODO: assert actual sin/cos values

    v3 a = {1.0f, 0.0f, 0.0f};
    v3 vres = m4_transform(res, a);
    CHECK(vres.x, 0.0f);
    CHECK(vres.y, 0.0f);
    CHECK(vres.z, 1.0f);
}

TEST(m4_create_rotation_z) {
    m4 res = m4_create_rotation_z(90.0f);
    // TODO: maybe check for 1.0f;
    // TODO: assert actual sin/cos values

    v3 a = {0.0f, 1.0f, 0.0f};
    v3 vres = m4_transform(res, a);
    CHECK(vres.x, 1.0f);
    CHECK(vres.y, 0.0f);
    CHECK(vres.z, 0.0f);
}

TEST(m4_create_translate) {
    m4 res = m4_create_translate(1.0f, 2.0f, 3.0f);
    CHECK(res.e[3], 1.0f);
    CHECK(res.e[7], 2.0f);
    CHECK(res.e[11], 3.0f);
    CHECK(res.e[15], 1.0f);

    v3 a = {1.0f, 2.0f, 3.0f};
    v3 vres = m4_transform(res, a);
    CHECK(vres.x, 2.0f);
    CHECK(vres.y, 4.0f);
    CHECK(vres.z, 6.0f);
}

int main() {
    TEST_REGISTER(m4_create_identity);
    TEST_REGISTER(m4_create_scale);
    TEST_REGISTER(m4_create_rotation_x);
    TEST_REGISTER(m4_create_rotation_y);
    TEST_REGISTER(m4_create_rotation_z);
    TEST_REGISTER(m4_create_translate);
    TEST_RUN();
}
