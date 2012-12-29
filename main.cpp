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

const int FPS=60;

namespace Direction {
  enum type {
    UP, DOWN, LEFT, RIGHT
  };
};

class Box {
  public:

  int x, y, w, h;

  void init(int xx, int yy, int ww = 30, int hh = 200) {
    x = xx;
    y = yy;
    w = ww;
    h = hh;
  }

  void render() {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
  }
};

class Player {
  int upVelocity, downVelocity;
  SDLKey upKey, downKey;

  public:

  int speed;
  Box box;

  void init(SDLKey up, SDLKey down) {
    upKey    = up;
    downKey  = down;

    upVelocity    = 0;
    downVelocity  = 0;

    speed = 10;
  }

  void update(Box top, Box bottom) {
    int yy = box.y + (upVelocity + downVelocity);
    int bottomY = bottom.y - box.h;

    if(yy <= top.y)
      yy = top.y;
    else if(yy >= bottomY)
      yy = bottomY;

    box.y = yy;
  }

  void render() {
    box.render();
  }

  void travel(Direction::type direction) {
    if(direction == Direction::DOWN) {
      downVelocity = speed;
    } else if (direction == Direction::UP) {
      upVelocity = speed * -1;
    }
  }

  void stop(Direction::type direction) {
    if(direction == Direction::DOWN) {
      downVelocity = 0;
    } else if (direction == Direction::UP) {
      upVelocity = 0;
    }
  }

  void keyDown(SDLKey key) {
    if(key == downKey) {
      travel(Direction::DOWN);
    } else if (key == upKey) {
      travel(Direction::UP);
    }
  }

  void keyUp(SDLKey key) {
    if(key == downKey) {
      stop(Direction::DOWN);
    } else if (key == upKey) {
      stop(Direction::UP);
    }
  }
};

class Ball {
  int angle;
  int speed;
  int xVelocity;
  int yVelocity;

  public:

  Box box;

  void init() {
    angle = 45;
    speed = 4;

    xVelocity = speed;
    yVelocity = speed;
  }

  void update(Player player1, Player player2) {
    int xx = box.x + xVelocity;
    int yy = box.y + yVelocity;


    box.x = xx + xVelocity;
    box.y = yy + yVelocity;
  }

  void render() {
    box.render();
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
  player.init(SDLK_w, SDLK_s);
  player.box.init(80, 280);

  Player player2;
  player2.init(SDLK_UP, SDLK_DOWN);
  player2.box.init(1130, 280);

  Ball ball;
  ball.init();
  ball.box.init(300, 300, 30, 30);
  // ball.box.bounds.init(0, 0, 1280, 720);

  // Bounds
  Box top; top.init(0, 0, 1280, 0);
  Box bottom; bottom.init(0, 718, 1280, 0);
  Box left; left.init(0, 0, 1280, 0);
  Box right; right.init(1280, 0, 720, 0);

  while(running) {
    start = SDL_GetTicks();

    glClear(GL_COLOR_BUFFER_BIT);
    player.render();
    player2.render();
    ball.render();
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

    player.update(top, bottom);
    player2.update(top, bottom);
    ball.update(player, player2);

    SDL_GL_SwapBuffers();

    int offset = SDL_GetTicks() - start;
    if((1000 / FPS) > offset)
      SDL_Delay((1000 / FPS) - offset);
  }

  return 0;
}
