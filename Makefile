CC=g++
CFLAGS=`sdl-config --cflags --libs` -framework GLUT -framework OpenGL
all: game
clean:
	rm -rf build
test: game
	./build/game
bundle: game
	rm -rf Pong Pong.app
	mkdir Pong
	cp build/game Pong/Pong
	touch Pong/Info.plistp
	mv Pong Pong.app
game:
	mkdir -p build
	$(CC) -o build/game main.cpp $(CFLAGS)
