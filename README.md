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
