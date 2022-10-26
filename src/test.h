#ifndef TEST_H
#define TEST_H

#ifndef MAX_TESTCASES
#define MAX_TESTCASES 1024
#endif // MAX_TESTCASES

#define TEST(name) void name()
#define TEST_REGISTER(func) test__register(#func, func)
#define TEST_RUN() test__run(__FILE__)
#define SUCCESS_TO_SYMBOL(s) (s ? "==" : "!=")
#define SUCCESS_TO_COLOR(s) (s ? TEST__COLOR_SUCCESS : TEST__COLOR_FAILURE)

#define CHECK(a, b) \
    { bool success = test__check(a, b); \
    test__print(success, __FILE__, __LINE__); \
    test__print_statement(success, a, b); \
    if (!success) { \
        test__print_code(__FILE__, __LINE__); \
        g_failure = true; }}

typedef void (TestFunc)(void);
typedef struct {
    const char *name;
    TestFunc *function;
} TestCase;

enum {
    TEST__COLOR_DEFAULT = 0,
    TEST__COLOR_CODE = 2,
    TEST__COLOR_TEST_CASE = 35,
    TEST__COLOR_FAILURE = 91,
    TEST__COLOR_SUCCESS = 92,
    TEST__COLOR_TITLE = 96,
};

static TestCase g_testcases[MAX_TESTCASES];
static size_t g_num_testcases = 0;
static bool g_failure = false;

// Check functions
bool test__checki(int a, int b) {
    return a == b;
}

bool test__checks(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

bool test__checkf(float a, float b) {
    return a == b;
}

#define test__check(a, b) _Generic((a, b), \
    int: test__checki, \
    char*: test__checks, \
    float: test__checkf)(a, b)

void test__print_statementi(bool success, int a, int b) {
    printf("%d %s %d\n", a, SUCCESS_TO_SYMBOL(success), b);
}

void test__print_statements(bool success, const char *a, const char *b) {
    printf("%s %s %s\n", a, SUCCESS_TO_SYMBOL(success), b);
}

void test__print_statementf(bool success, float a, float b) {
    printf("%f %s %f\n", a, SUCCESS_TO_SYMBOL(success), b);
}

#define test__print_statement(s, a, b) _Generic((s, a, b), \
    int: test__print_statementi, \
    char*: test__print_statements, \
    float: test__print_statementf)(s, a, b)

void test__register(const char *name, TestFunc *function) {
    TestCase *tc = &g_testcases[g_num_testcases++];
    tc->name = name;
    tc->function = function;
}

void test__set_color(int color) {
    if (color == TEST__COLOR_DEFAULT) {
        printf("\033[m");
    } else {
        printf("\033[1;%dm", color);
    }
}

void test__print(bool success, const char *filename, size_t line) {
    const char *symbol = success ? "[/]" : "[x]";
    int color = SUCCESS_TO_COLOR(success);
    test__set_color(color);
    printf("%s ", symbol);
    test__set_color(TEST__COLOR_DEFAULT);
    printf("%s:%llu - ", filename, line);
}

void test__print_code(const char *filename, size_t line) {
    test__set_color(TEST__COLOR_CODE);
    FILE *fp = fopen(filename, "r");
    char buf[1024];
    if (fp) {
        int i = 0;
        while (fgets(&buf[0], sizeof(buf), fp)) {
            if (++i == line) {
                printf("%s", buf);
            }
        }
        fclose(fp);
    }
    test__set_color(TEST__COLOR_DEFAULT);
}

void test__run(const char *filename) {
    test__set_color(TEST__COLOR_TITLE);
    printf("# Run tests of file %s\n", filename);
    
    for (size_t i = 0; i < g_num_testcases; ++i) {
        TestCase *tc = &g_testcases[i];
        test__set_color(TEST__COLOR_TEST_CASE);
        printf("=> %s\n", tc->name);
        tc->function();
    }

    int color = SUCCESS_TO_COLOR(!g_failure);
    test__set_color(color);
    const char *text = g_failure ? "FAILURE" : "SUCCESS";
    printf("========");
    printf("    %s    ", text);
    printf("========\n\n");
    test__set_color(TEST__COLOR_DEFAULT);
}

#endif // TEST_H
