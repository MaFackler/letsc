# Lets code in c

Small fun little projects in c.

# Code Base
## Compile Commands

```base
# Build src/main.c
# Output is build/a.out
make
# or
make main

# Build and execute all tests
# Tests are store under tests/test_*.c
# Output is build/test_*
make test
```

## Create a new test
To create a new test just create a file with test_ prefix in
the tests directory. `make test` will detect it automatically
