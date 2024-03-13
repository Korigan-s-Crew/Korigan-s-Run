TARGET = main
SRCS = main.c init.c controls.c movement.c procedural_generation.c
OBJS = $(patsubst %.c, obj/%.o, $(SRCS))
BINARY = bin/$(TARGET)

# Compilation
CC = clang
CFLAGS = -std=gnu11 -Wall -pedantic -O3

# Debug
CFLAGS += -g
CFLAGS += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address

# Libs
CFLAGS += $(shell pkg-config --cflags sdl2)
LDFLAGS += $(shell pkg-config --libs sdl2)
CFLAGS += $(shell pkg-config --cflags SDL2_image)
LDFLAGS += $(shell pkg-config --libs SDL2_image)
CFLAGS += $(shell pkg-config --cflags SDL2_ttf)
LDFLAGS += $(shell pkg-config --libs SDL2_ttf)

# Source directories
VPATH = src:.

all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BINARY)

.PHONY: all clean

run: $(BINARY)
	./$(BINARY)
