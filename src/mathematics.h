#ifndef MATHEMATICS
#define MATHEMATICS
#include <math.h>

// (rad/deg) = (PI/180)
// rad = deg * PI / 180
// deg = rad * 180 / PI
#define TO_RAD(deg) (deg * (M_PI / 180.0f))
#define TO_DEG(rad) (rad * (180.0f / M_PI))

typedef struct {
    float x;
    float y;
    float z;
} v3;

typedef struct {
    float e[16];
} m4;

#define M4_ELEMENT(m, x, y) m.e[y * 4 + x]

v3 v3_sub(v3 a, v3 b) {
    v3 res = {0};
    res.x = a.x - b.x;
    res.y = a.y - b.y;
    return res;
}

float v3_length(v3 a) {
    float res = a.x * a.x + a.y * a.y + a.z * a.z;
    return sqrt(res);
}

v3 v3_normalize(v3 a) {
    v3 res = {0};
    float len = v3_length(a);
    res.x = a.x / len;
    res.y = a.y / len;
    res.z = a.z / len;
    return res;
}

v3 v3_direction(v3 from, v3 to) {
    v3 res = v3_sub(to, from);
    return v3_normalize(res);
}

v3 v3_cross(v3 a, v3 b) {
    v3 res = {0};
    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;
    return res;
}

float v3_dot(v3 a, v3 b) {
    float res = 0;
    res += a.x * b.x;
    res += a.y * b.y;
    res += a.z * b.z;
    return res;
}

m4 m4_create_scale(float x, float y, float z) {
    m4 res = {0};
    M4_ELEMENT(res, 0, 0) = x;
    M4_ELEMENT(res, 1, 1) = y;
    M4_ELEMENT(res, 2, 2) = z;
    M4_ELEMENT(res, 3, 3) = 1.0f;
    return res;
}

m4 m4_create_identity() {
    return m4_create_scale(1.0f, 1.0f, 1.0f);
}

m4 m4_create_rotation_x(float angle) {
    m4 res = {0};

    angle = TO_RAD(angle);
    float sv = sinf(angle);
    float cv = cosf(angle);
    M4_ELEMENT(res, 0, 0) = 1.0f;

    M4_ELEMENT(res, 1, 1) = cv;
    M4_ELEMENT(res, 2, 1) = -sv;

    M4_ELEMENT(res, 1, 2) = sv;
    M4_ELEMENT(res, 2, 2) = cv;

    M4_ELEMENT(res, 3, 3) = 1.0f;
    return res;
}

m4 m4_create_rotation_y(float angle) {
    m4 res = {0};

    angle = TO_RAD(angle);
    float sv = sinf(angle);
    float cv = cosf(angle);

    M4_ELEMENT(res, 0, 0) = cv;
    M4_ELEMENT(res, 2, 0) = sv;

    M4_ELEMENT(res, 1, 1) = 1.0f;

    M4_ELEMENT(res, 0, 2) = -sv;
    M4_ELEMENT(res, 2, 2) = cv;

    M4_ELEMENT(res, 3, 3) = 1.0f;
    return res;
}

m4 m4_create_rotation_z(float angle) {
    m4 res = {0};

    angle = TO_RAD(angle);
    float sv = sinf(angle);
    float cv = cosf(angle);

    M4_ELEMENT(res, 0, 0) = cv;
    M4_ELEMENT(res, 1, 0) = -sv;

    M4_ELEMENT(res, 0, 1) = sv;
    M4_ELEMENT(res, 1, 1) = cv;

    M4_ELEMENT(res, 3, 3) = 1.0f;
    return res;
}

m4 m4_create_translate(float x, float y, float z) {
    m4 res = m4_create_identity();
    M4_ELEMENT(res, 3, 0) = x;
    M4_ELEMENT(res, 3, 1) = y;
    M4_ELEMENT(res, 3, 2) = z;
    return res;
}

v3 m4_transform(m4 m, v3 a) {
    v3 res = {0};
    res.x = M4_ELEMENT(m, 0, 0) * a.x +
            M4_ELEMENT(m, 1, 0) * a.y +
            M4_ELEMENT(m, 2, 0) * a.z +
            M4_ELEMENT(m, 3, 0) * 1.0f;
    res.y = M4_ELEMENT(m, 0, 1) * a.x +
            M4_ELEMENT(m, 1, 1) * a.y +
            M4_ELEMENT(m, 2, 1) * a.z +
            M4_ELEMENT(m, 3, 1) * 1.0f;
    res.z = M4_ELEMENT(m, 0, 2) * a.x +
            M4_ELEMENT(m, 1, 2) * a.y +
            M4_ELEMENT(m, 2, 2) * a.z +
            M4_ELEMENT(m, 3, 2) * 1.0f;
    return res;
}

m4 m4_mul(m4 a, m4 b) {
    m4 res = {0};

#define M4_CALC(x, y) \
    M4_ELEMENT(res, x, y) = M4_ELEMENT(a, x, 0) * M4_ELEMENT(b, 0, y) + \
                            M4_ELEMENT(a, x, 1) * M4_ELEMENT(b, 1, y) + \
                            M4_ELEMENT(a, x, 2) * M4_ELEMENT(b, 2, y) + \
                            M4_ELEMENT(a, x, 3) * M4_ELEMENT(b, 3, y)

    M4_CALC(0, 0);
    M4_CALC(1, 0);
    M4_CALC(2, 0);
    M4_CALC(3, 0);

    M4_CALC(0, 1);
    M4_CALC(1, 1);
    M4_CALC(2, 1);
    M4_CALC(3, 1);

    M4_CALC(0, 2);
    M4_CALC(1, 2);
    M4_CALC(2, 2);
    M4_CALC(3, 2);

    M4_CALC(0, 3);
    M4_CALC(1, 3);
    M4_CALC(2, 3);
    M4_CALC(3, 3);

#undef M4_CALC

    return res;
}

#endif // MATHEMATICS
