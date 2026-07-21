CC := gcc
WARNINGS := -Wall -Wextra -Wpedantic -Wformat=2 -Wformat-signedness -Wconversion -Wsign-conversion -std=gnu11

SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.c)
HDRS := $(wildcard $(SRC_DIR)/*.h)

CLANG_FORMAT ?= clang-format

# Build configuration (debug by default). Selected via the debug/release targets.
CONFIG ?= debug

ifeq ($(CONFIG),debug)
  CFLAGS := $(WARNINGS) -g3 -fsanitize=address -fno-omit-frame-pointer
  LDFLAGS := -fsanitize=address
else ifeq ($(CONFIG),release)
  CFLAGS := $(WARNINGS) -O3 -march=native -flto -DNDEBUG
  LDFLAGS := -flto
else
  $(error unknown CONFIG '$(CONFIG)' (expected 'debug' or 'release'))
endif

OUT := $(BUILD_DIR)/$(CONFIG)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OUT)/%.o,$(SRCS))
TARGET := $(OUT)/ironbook

.PHONY: debug release build fmt fmt-check clean

debug:
	$(MAKE) build CONFIG=debug

release:
	$(MAKE) build CONFIG=release

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OUT)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

fmt:
	$(CLANG_FORMAT) -i $(SRCS) $(HDRS)

fmt-check:
	$(CLANG_FORMAT) --dry-run --Werror $(SRCS) $(HDRS)

clean:
	rm -rf $(BUILD_DIR)/
