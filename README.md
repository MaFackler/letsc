# Lets code in c

Small fun little projects in c.

# Code Base
## Compile Commands

```base
# Build src/main.c
# Output is a.out
make
# or
make main

# Build and execute all tests
# Tests are store under tests/test_*.c
# Output is test_*
make test

# Build all examples
# Examples are stored under examples/*.c
# Output is like the example filename
make example
```

## Create a new test
To create a new test just create a file with test_ prefix in
the tests directory. `make test` will detect it automatically

## Create a new example
To create a new example just create a file in the examples directory.
The output binary will be the same name like the file without appendix.

* Write TEST TEST_REGISTER TEST_RUN
enum {
    TEST__COLOR_DEFAULT = 0,
    TEST__COLOR_CODE = 2,
    TEST__COLOR_TEST_CASE = 35,
    TEST__COLOR_FAILURE = 91,
    TEST__COLOR_SUCCESS = 92,
	TEST__COLOR_TITLE = 96,
};
\033[m
\033[1;%dm
#define test__check(a, b) _Generic((a, b), \
    int: test__checki, \
    float: test__checkf, \
    char *: test__checks \
    )(a, b)

void test__print_state_and_file(const char *file, int line, bool success) {
void test__print_file_lines(const char *file, int line) {
