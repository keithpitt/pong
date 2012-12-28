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

namespace Direction {
  enum type {
    UP, DOWN, LEFT, RIGHT
  };
};

class Entity {
  int upVelocity, downVelocity, leftVelocity, rightVelocity;

  public:

  int x, y, w, h;
  int speed;

  void init(int xx, int yy) {
    x = xx;
    y = yy;
    w = 70;
    h = 200;
    speed = 10;
    upVelocity    = 0;
    downVelocity  = 0;
    leftVelocity  = 0;
    rightVelocity = 0;
  }

  void travel(Direction::type direction) {
    if(direction == Direction::DOWN) {
      downVelocity = speed;
    } else if (direction == Direction::UP) {
      upVelocity = speed * -1;
    } else if (direction == Direction::LEFT) {
      leftVelocity = speed * -1;
    } else if (direction == Direction::RIGHT) {
      rightVelocity = speed;
    }
  }

  void stop(Direction::type direction) {
    if(direction == Direction::DOWN) {
      downVelocity = 0;
    } else if (direction == Direction::UP) {
      upVelocity = 0;
    } else if (direction == Direction::LEFT) {
      leftVelocity = 0;
    } else if (direction == Direction::RIGHT) {
      rightVelocity = 0;
    }
  }

  void render() {
    glBegin(GL_QUADS);
      glVertex2f(x,y);
      glVertex2f(x + w, y);
      glVertex2f(x + w, y + h);
      glVertex2f(x, y + h);
    glEnd();
  }

  void update() {
    x += leftVelocity + rightVelocity;
    y += upVelocity + downVelocity;
  }
};

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface * screen = SDL_SetVideoMode(1280, 720, 32, SDL_SWSURFACE|SDL_OPENGL);

  bool running = true;
  uint32_t start;
  SDL_Event event;

  init();

  Entity player;
  player.init(80, 280);

  Entity player2;
  player2.init(1130, 280);

  while(running) {
    start = SDL_GetTicks();

    glClear(GL_COLOR_BUFFER_BIT);
    player.render();
    player2.render();
    glFlush();

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
          player.travel(Direction::DOWN);
          break;
        } else if (key == SDLK_UP) {
          player.travel(Direction::UP);
          break;
        } else if (key == SDLK_LEFT) {
          player.travel(Direction::LEFT);
          break;
        } else if (key == SDLK_RIGHT) {
          player.travel(Direction::RIGHT);
          break;
        }

        if(key == SDLK_s) {
          player2.travel(Direction::DOWN);
          break;
        } else if (key == SDLK_w) {
          player2.travel(Direction::UP);
          break;
        } else if (key == SDLK_a) {
          player2.travel(Direction::LEFT);
          break;
        } else if (key == SDLK_d) {
          player2.travel(Direction::RIGHT);
          break;
        }
      }

      if(event.type == SDL_KEYUP) {
        SDLKey key = event.key.keysym.sym;

        if(key == SDLK_DOWN) {
          player.stop(Direction::DOWN);
          break;
        } else if (key == SDLK_UP) {
          player.stop(Direction::UP);
          break;
        } else if (key == SDLK_LEFT) {
          player.stop(Direction::LEFT);
          break;
        } else if (key == SDLK_RIGHT) {
          player.stop(Direction::RIGHT);
          break;
        }

        if(key == SDLK_s) {
          player2.stop(Direction::DOWN);
          break;
        } else if (key == SDLK_w) {
          player2.stop(Direction::UP);
          break;
        } else if (key == SDLK_a) {
          player2.stop(Direction::LEFT);
          break;
        } else if (key == SDLK_d) {
          player2.stop(Direction::RIGHT);
          break;
        }
      }

      // debug("unhandled event %i", event.type);
    }

    player.update();
    player2.update();

    SDL_GL_SwapBuffers();
  }

  return 0;
}
