#include <test.h>
#include <mathematics.h>

TEST(v3_sub) {
    v3 a = {1.0f, 2.0f, 3.0f};
    v3 b = {1.0f, 2.0f, 3.0f};
    v3 res = v3_sub(a, b);
    CHECK(res.x, 0.0f);
    CHECK(res.y, 0.0f);
    CHECK(res.z, 0.0f);
}

TEST(v3_length) {
    v3 a = {1.0f, 0.0f, 0.0f};
    float res = v3_length(a);
    CHECK(res, 1.0f);
}

TEST(v3_normalize) {
    v3 a = {1.0f, 2.0f, 3.0f};
    v3 res = v3_normalize(a);
    CHECK(v3_length(res), 1.0f);
}

TEST(v3_direction) {
    v3 a = {1.0f, 1.0f, 0.0f};
    v3 b = {2.0f, 2.0f, 0.0f};
    v3 res = v3_direction(a, b);
    CHECK(res.x, 0.707107f);
    CHECK(res.y, 0.707107f);
}

TEST(v3_cross) {
    v3 a = {1.0f, 0.0f, 0.0f};
    v3 b = {0.0f, 1.0f, 0.0f};
    v3 res = v3_cross(b, a);
    CHECK(res.x, 0.0f);
    CHECK(res.y, 0.0f);
    CHECK(res.z, -1.0f);

    res = v3_cross(a, b);
    CHECK(res.x, 0.0f);
    CHECK(res.y, 0.0f);
    CHECK(res.z, 1.0f);
}

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
    CHECK(vres.z, -1.0f);
}

TEST(m4_create_rotation_z) {
    m4 res = m4_create_rotation_z(90.0f);
    // TODO: maybe check for 1.0f;
    // TODO: assert actual sin/cos values

    v3 a = {0.0f, 1.0f, 0.0f};
    v3 vres = m4_transform(res, a);
    CHECK(vres.x, -1.0f);
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

TEST(m4_mul) {
    m4 a = m4_create_scale(1.0f, 2.0f, 3.0f);
    m4 b = m4_create_translate(1.0f, 2.0f, 3.0f);
    m4 res = m4_mul(b, a);
    CHECK(M4_ELEMENT(res, 0, 0), 1.0f);
    CHECK(M4_ELEMENT(res, 1, 1), 2.0f);
    CHECK(M4_ELEMENT(res, 2, 2), 3.0f);
    CHECK(M4_ELEMENT(res, 3, 3), 1.0f);

    // NOTE: scale affects translate
    CHECK(M4_ELEMENT(res, 3, 0), 1.0f);
    CHECK(M4_ELEMENT(res, 3, 1), 4.0f);
    CHECK(M4_ELEMENT(res, 3, 2), 9.0f);
    CHECK(M4_ELEMENT(res, 3, 3), 1.0f);
}

int main() {
    TEST_REGISTER(m4_create_identity);
    TEST_REGISTER(m4_create_scale);
    TEST_REGISTER(m4_create_rotation_x);
    TEST_REGISTER(m4_create_rotation_y);
    TEST_REGISTER(m4_create_rotation_z);
    TEST_REGISTER(m4_create_translate);
    TEST_REGISTER(v3_sub);
    TEST_REGISTER(v3_length);
    TEST_REGISTER(v3_normalize);
    TEST_REGISTER(v3_direction);
    TEST_REGISTER(v3_cross);
    TEST_REGISTER(m4_mul);
    TEST_RUN();
}
