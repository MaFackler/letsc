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

# Build src/shared_*.c
# (build/a.out will call the shared lib) 
make shared

# Build and execute all tests
# Tests are store under tests/test_*.c
# Output is build/test_*
make test
```

## Create a new test
To create a new test just create a file with test_ prefix in
the tests directory. `make test` will detect it automatically

## NOTE
The data/font.bmp is not part of the git repo.
It has to be downloaded manually:

[OpenGameArt](https://opengameart.org/content/ascii-bitmap-font-cellphone)

