CC=g++
CFLAGS=`sdl-config --cflags --static-libs` -framework GLUT -framework OpenGL
OBJECTS=main.o
all: game
clean:
	rm -rf build
	rm -rf Pong.app
	rm -rf Pong.zip
test: game
	./build/game
bundle: game
	rm -rf Pong.app
	mkdir -p Pong/Contents/MacOS
	mkdir -p Pong/Contents/Resources
	cp build/game Pong/Contents/MacOS/Pong
	iconutil -c icns -o Pong/Contents/Resources/Pong.icns resources/osx.iconset
	cp Info.plist Pong/Contents
	mv Pong Pong.app
dist: bundle
	cp -R Pong.app ~/Dropbox/Games/
.cpp.o:
	$(CC) -Wall -c -o $@ $<
game: $(OBJECTS)
	mkdir -p build
	$(CC) -o build/game $(OBJECTS) $(CFLAGS)
