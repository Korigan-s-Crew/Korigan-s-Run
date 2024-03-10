TARGET = main
SRCS = basic.c
OBJS = $(SRCS:.c=.o)

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
# CFLAGS += $(shell pkg-config --cflags SDL2_image)
# LDFLAGS += $(shell pkg-config --libs SDL2_image)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

run: $(TARGET)
	./$(TARGET)