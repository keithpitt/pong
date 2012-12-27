CC=g++
CFLAGS=`sdl-config --cflags --libs` -framework GLUT -framework OpenGL
all: game
clean:
	rm -rf build
game:
	mkdir -p build
	$(CC) -o build/game main.cpp $(CFLAGS)
