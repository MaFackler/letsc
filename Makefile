BUILD_DIR=build
CC=gcc -g -Isrc
HEADERS=$(wildcard src/*.h)
TESTS=$(wildcard tests/test_*.c)
TESTS_BIN=$(subst tests/, $(BUILD_DIR)/, $(basename $(TESTS)))

$(BUILD_DIR)/a.out: src/main.c $(HEADERS) $(BUILD_DIR)
	$(CC) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: test
test: $(TESTS_BIN)
	@for t in $(TESTS_BIN); do ./$$t; done

$(TESTS_BIN): $(TESTS) $(HEADERS) $(BUILD_DIR)

$(BUILD_DIR)/%: tests/%.c
	$(CC) $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*

run:
	echo ./build/a.out | entr -c -r -s './build/a.out'
