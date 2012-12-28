CC=g++
CFLAGS=`sdl-config --cflags --static-libs` -framework GLUT -framework OpenGL
DIR=build
NAME=Pong
all: pong
clean:
	rm -rf $(DIR)
	rm -rf $(NAME).app
	rm -rf $(NAME).zip
test: pong
	./$(DIR)/pong
bundle: pong
	rm -rf $(NAME).app
	mkdir -p $(NAME)/Contents/MacOS
	mkdir -p $(NAME)/Contents/Resources
	cp $(DIR)/pong $(NAME)/Contents/MacOS/$(NAME)
	iconutil -c icns -o $(NAME)/Contents/Resources/$(NAME).icns assets/icon.iconset
	cp Info.plist $(NAME)/Contents
	mv $(NAME) $(NAME).app
dist: bundle
	cp -R $(NAME).app ~/Dropbox/Games/
prepare:
	mkdir -p $(DIR)
main: prepare
	$(CC) -Wall -c main.cpp -o $(DIR)/main.o
pong: main
	$(CC) -Wall -o $(DIR)/pong $(DIR)/main.o $(CFLAGS)
