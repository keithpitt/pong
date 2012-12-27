CC=g++
CFLAGS=`sdl-config --cflags --libs`
all: game
clean:
	rm -rf *.o
	rm game
game:
	$(CC) -o game main.cpp $(CFLAGS)
