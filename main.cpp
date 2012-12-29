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

class Rectangle {
  public:
  int x1, y1, x2, y2;

  void init(int _x1, int _y1, int _x2, int _y2) {
    x1 = _x1;
    y1 = _y1;
    x2 = _x2;
    y2 = _y2;
  };
};

class Entity {
  int upVelocity, downVelocity, leftVelocity, rightVelocity;

  public:

  int       x, y, w, h;
  int       speed;
  Rectangle bounds;

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
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
  }

  void update() {
    int xx = x + (leftVelocity + rightVelocity);
    int yy = y + (upVelocity + downVelocity);

    if(xx < bounds.x1) {
      xx = bounds.x1;
    } else if (xx > (bounds.x2 - w)) {
      xx = bounds.x2 - w;
    }

    if(yy < bounds.y1) {
      yy = bounds.y1;
    } else if (yy > (bounds.y2 - h)) {
      yy = (bounds.y2 - h);
    }

    x = xx;
    y = yy;
  }
};

class Player {
  SDLKey upKey, downKey, leftKey, rightKey;

  public:

  Entity entity;

  void init(SDLKey up, SDLKey down, SDLKey left, SDLKey right) {
    upKey    = up;
    downKey  = down;
    leftKey  = left;
    rightKey = right;
  }

  void update() {
    entity.update();
  }

  void render() {
    entity.render();
  }

  void keyDown(SDLKey key) {
    if(key == downKey) {
      entity.travel(Direction::DOWN);
    } else if (key == upKey) {
      entity.travel(Direction::UP);
    } else if (key == leftKey) {
      entity.travel(Direction::LEFT);
    } else if (key == rightKey) {
      entity.travel(Direction::RIGHT);
    }
  }

  void keyUp(SDLKey key) {
    if(key == downKey) {
      entity.stop(Direction::DOWN);
    } else if (key == upKey) {
      entity.stop(Direction::UP);
    } else if (key == leftKey) {
      entity.stop(Direction::LEFT);
    } else if (key == rightKey) {
      entity.stop(Direction::RIGHT);
    }
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

  Player player;
  player.init(SDLK_w, SDLK_s, SDLK_a, SDLK_d);
  player.entity.init(80, 280);
  player.entity.bounds.init(player.entity.x, 0, player.entity.x + player.entity.w, 720);

  Player player2;
  player2.init(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
  player2.entity.init(1130, 280);
  player2.entity.bounds.init(player2.entity.x, 0, player2.entity.x + player2.entity.w, 720);

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

        player.keyDown(key);
        player2.keyDown(key);
      }

      if(event.type == SDL_KEYUP) {
        SDLKey key = event.key.keysym.sym;

        player.keyUp(key);
        player2.keyUp(key);
      }

      // debug("unhandled event %i", event.type);
    }

    player.update();
    player2.update();

    SDL_GL_SwapBuffers();
  }

  return 0;
}
