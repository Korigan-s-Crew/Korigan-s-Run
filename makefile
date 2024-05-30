TARGET = main
SRCS = main.c init.c controls.c movement.c procedural_generation.c map.c dash_effect.c animation.c implem_map.c music.c draw.c global.c
SRCS_test = test.c music.c init.c implem_map.c map.c movement.c draw.c global.c dash_effect.c animation.c
#SRCS = $(wildcard src/*.c)
OBJS = $(patsubst %.c, obj/%.o, $(SRCS))
OBJS_test = $(patsubst %.c, obj/%.o, $(SRCS_test))
BINARY = bin/$(TARGET)

# Compilation
CC = clang
CFLAGS = -std=gnu11 -Wall -pedantic -O3
LDFLAGS = -lm

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
CFLAGS += $(shell pkg-config --cflags SDL2_mixer)
LDFLAGS += $(shell pkg-config --libs SDL2_mixer)

# Source directories
VPATH = src:.

all: $(BINARY)

$(BINARY): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $^ $(LDFLAGS) -o  $@

obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BINARY)

.PHONY: all clean

run: $(BINARY)
	./$(BINARY)

valgrind: $(BINARY)
	valgrind --verbose a--leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BINARY)
 

test: $(OBJS_test)
	$(CC) $^ $(LDFLAGS) -o $@

run_test: test
	./test
