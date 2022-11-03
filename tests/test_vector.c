#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <test.h>

#include <vector.h>

TEST(vec_int) {
    int *myvec = NULL;
    CHECK(vec_size(myvec), 0);
    CHECK(vec_capacity(myvec), 0);
    int *value = vec_add(myvec);
    *value = 1000;
    CHECK(vec_size(myvec), 1);
    CHECK(vec_capacity(myvec), 8);
    CHECK(myvec[0], 1000);

    for (size_t i = 0; i < 8; ++i) {
        vec_push(myvec, i);
    }

    CHECK(vec_size(myvec), 9);
    CHECK(vec_capacity(myvec), 18);
    CHECK(myvec[1], 0);
    CHECK(myvec[2], 1);
    CHECK(myvec[3], 2);
    CHECK(myvec[4], 3);
    CHECK(myvec[5], 4);
    CHECK(myvec[6], 5);
    CHECK(myvec[7], 6);
    CHECK(myvec[8], 7);

    vec_clear(myvec);
    CHECK(vec_size(myvec), 0);
    CHECK(vec_capacity(myvec), 18);
    vec_free(myvec);
    CHECK(vec_size(myvec), 0);
    CHECK(vec_capacity(myvec), 0);
}

TEST(vec_char) {
    char *s = NULL;
    vec_push(s, 'h');
    vec_push(s, 'e');
    vec_push(s, 'l');
    vec_push(s, 'l');
    vec_push(s, 'o');

    CHECK(vec_size(s), 5);
    CHECK(vec_capacity(s), 8);
    CHECK(s, "hello");
}

int main() {
    TEST_REGISTER(vec_int);
    TEST_REGISTER(vec_char);
    TEST_RUN();
}
