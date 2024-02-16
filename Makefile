CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3 -fsanitize=address 
LDFLAGS = -fsanitize=address
LIBS = -lSDL2

main: example.o
	clang -o main $(LDFLAGS) example.o $(LIBS)
example.o: example.c
	clang -c $(CFLAGS) example.c
clean:
	rm -f main *.o
run: main
	./main
	rm -f main *.o

