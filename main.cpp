#include <iostream>
#include <SDL/SDL.h>
#include <stdint.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

int main(int argc, char** argv) {
  std::cout << "Hello World!";

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface * screen = SDL_SetVideoMode(1280, 720, 32, SDL_SWSURFACE|SDL_OPENGL);

  bool running = true;
  uint32_t start;
  SDL_Event event;

  while(running) {
    start = SDL_GetTicks();

    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        running = false;
        break;
      }
    }
  }

  return 0;
}
