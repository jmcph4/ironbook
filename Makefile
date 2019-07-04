PROJ_NAME=ironbook

SRC_DIR=src
BUILD_DIR=build
LIB_DIR=lib
INC_DIR=include

CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -pedantic -std=c11 -g3 -I$(INC_DIR) -L$(LIB_DIR) -ljcrl -lm

$(BUILD_DIR)/$(PROJ_NAME): $(SRC_DIR)/*.c
	$(CC) $? $(CFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

