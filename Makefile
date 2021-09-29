CFLAGS = `sdl2-config --cflags` -Wall -g
LDFLAGS = `sdl2-config --libs`

example-font: nibl.o example-font.o
	$(CC) -o example-font $^ $(LDFLAGS)

clean:
	rm -f *.o example-font
