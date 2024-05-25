TARGET = main
SRCS = main.c init.c controls.c movement.c procedural_generation.c map.c dash_effect.c animation.c implem_map.c music.c
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

# test.o : test.c
# 	$(CC) $(CFLAGS) -c $< -o $@ 

# test: test.o music.o
# 	$(CC) $(CFLAGS) -lSDL2_mixer -o test test.o music.o
# 	./test

# music.o : music.c
# 	$(CC) $(CFLAGS) -c $< -o $@
