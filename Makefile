CFLAGS = `sdl2-config --cflags` -Wall -g
LDFLAGS = `sdl2-config --libs` -lm

example-font: nibl.o example-font.o
	$(CC) -o example-font nibl.o example-font.o $(LDFLAGS)

example-nibl: nibl.o example-nibl.o
	$(CC) -o example-nibl nibl.o example-nibl.o $(LDFLAGS)


clean:
	rm -f *.o example-font
