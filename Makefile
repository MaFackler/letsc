BUILD_DIR=build
CC=gcc -g -Isrc -lX11 -lXext -lm
HEADERS=$(wildcard src/*.h)
TESTS=$(wildcard tests/test_*.c)
TESTS_BIN=$(subst tests/, $(BUILD_DIR)/, $(basename $(TESTS)))
SHARED_SRC=$(wildcard src/shared_*.c)
SHARED_BIN=$(patsubst src/%.c, $(BUILD_DIR)/%.so, $(SHARED_SRC))

$(BUILD_DIR)/a.out: src/main.c $(HEADERS) $(BUILD_DIR) $(SHARED_BIN)
	$(CC) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: test
test: $(TESTS_BIN)
	@for t in $(TESTS_BIN); do ./$$t; done

$(TESTS_BIN): $(TESTS) $(HEADERS) $(BUILD_DIR)

$(BUILD_DIR)/%: tests/%.c
	$(CC) $< -o $@



$(SHARED_BIN): $(SHARED_SRC)

$(BUILD_DIR)/%.so: src/%.c $(HEADERS)
	echo "LOCK" > .shared.lock
	$(CC) -fpic -shared -o $@ $<
	rm .shared.lock

clean:
	rm -rf $(BUILD_DIR)/*

run:
	echo ./build/a.out | entr -c -r -s './build/a.out'
