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

// Stolen from:
// http://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing
#define DEBUG true
#define debug(fmt, ...) \
  if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, \
      __LINE__, __func__, __VA_ARGS__);

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(100,0);
    glVertex2f(100,100);
    glVertex2f(0,100);
  glEnd();

  glFlush();
};

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface * screen = SDL_SetVideoMode(1280, 720, 32, SDL_SWSURFACE|SDL_OPENGL);

  bool running = true;
  uint32_t start;
  SDL_Event event;

  init();

  while(running) {
    start = SDL_GetTicks();
    draw();

    while(SDL_PollEvent(&event)) {
      debug("%i hello %s", event.type, "something");
      if(event.type == SDL_QUIT) {
        running = false;
        break;
      }
    }

    SDL_GL_SwapBuffers();
  }

  return 0;
}
