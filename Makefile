CC=g++
CPPFLAGS=`pkg-config freetype2 --libs --static --cflags` \
				 `pkg-config ftgl --libs --static --cflags` \
				 `sdl-config --static-libs --cflags`
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
pong:
	mkdir -p $(DIR)
	$(CC) -Wall -o $(DIR)/pong main.cpp $(CPPFLAGS)
