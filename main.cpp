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

int x = 0;
int y = 0;
int speed = 10;

int upVelocity = 0;
int downVelocity = 0;
int leftVelocity = 0;
int rightVelocity = 0;

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x + 100, y);
    glVertex2f(x + 100, y +100);
    glVertex2f(x,y + 100);
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

      // Handle OS quit events
      if(event.type == SDL_QUIT) {
        running = false;
        break;
      }

      // Handle key down events
      if(event.type == SDL_KEYDOWN) {
        // debug("%s key pressed.", SDL_GetKeyName(key));
        SDLKey key      = event.key.keysym.sym;
        SDLMod modifier = event.key.keysym.mod;

        // Handling CMD+Q for OSX
        if((modifier & KMOD_META) && key == SDLK_q) {
          running = false;
          break;
        }

        // Handle ALT-F4
        if((modifier & KMOD_ALT) && key == SDLK_F4) {
          running = false;
          break;
        }

        if(key == SDLK_DOWN) {
          downVelocity = speed;
          break;
        } else if (key == SDLK_UP) {
          upVelocity = speed * -1;
          break;
        } else if (key == SDLK_LEFT) {
          leftVelocity = speed * -1;
          break;
        } else if (key == SDLK_RIGHT) {
          rightVelocity = speed;
          break;
        }
      }

      if(event.type == SDL_KEYUP) {
        SDLKey key      = event.key.keysym.sym;

        if(key == SDLK_DOWN) {
          downVelocity = 0;
          break;
        } else if (key == SDLK_UP) {
          upVelocity = 0;
          break;
        } else if (key == SDLK_LEFT) {
          leftVelocity = 0;
          break;
        } else if (key == SDLK_RIGHT) {
          rightVelocity = 0;
          break;
        }
      }

      // debug("unhandled event %i", event.type);
    }

    x += leftVelocity + rightVelocity;
    y += upVelocity + downVelocity;

    SDL_GL_SwapBuffers();
  }

  return 0;
}
