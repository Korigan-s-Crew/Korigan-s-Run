CFLAGS = -std=c99 -Wall -Wextra -pedantic -g3 
LDFLAGS = 
LIBS = -lSDL2 -lSDL2_image

main: example.o
	clang -o main $(LDFLAGS) example.o $(LIBS)
main_2d: example_2d.o
	clang -o main_2d $(LDFLAGS) example_2d.o $(LIBS)
example.o: example.c
	clang -c $(CFLAGS) example.c
example_2d.o: example_2d.c
	clang -c $(CFLAGS) example_2d.c
clean:
	rm -f main *.o
	rm -f main_2d *.o
run: main main_2d
	./main_2d
	rm -f main_2d *.o

