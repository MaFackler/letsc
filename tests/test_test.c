#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <test.h>

TEST(test_int) {
    CHECK(1, 1);
    CHECK(2, 2);
}

TEST(test_float) {
    CHECK(1.0f, 1.0f);
}

TEST(test_string) {
    CHECK("hey", "hey");
    char buf[] = "heyyy";
    CHECK(&buf[0], "heyyy");
}

TEST_MAIN() {
    TEST_REGISTER(test_int);
    TEST_REGISTER(test_float);
    TEST_REGISTER(test_string);
    TEST_RUN();

}
