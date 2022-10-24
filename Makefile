BUILD_DIR=build
CC=gcc -g -Isrc

$(BUILD_DIR)/a.out: src/main.c $(BUILD_DIR)
	$(CC) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run:
	echo ./build/a.out | entr -c -r -s './build/a.out'
