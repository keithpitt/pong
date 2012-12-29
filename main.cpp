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

class Box {
  int upVelocity, downVelocity, leftVelocity, rightVelocity;

  public:

  int       x, y, w, h;
  int       speed;
  Rectangle bounds;

  void init(int xx, int yy, int ww = 30, int hh = 200) {
    x = xx;
    y = yy;
    w = ww;
    h = hh;
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

  Box box;

  void init(SDLKey up, SDLKey down, SDLKey left, SDLKey right) {
    upKey    = up;
    downKey  = down;
    leftKey  = left;
    rightKey = right;
  }

  void update() {
    box.update();
  }

  void render() {
    box.render();
  }

  void keyDown(SDLKey key) {
    if(key == downKey) {
      box.travel(Direction::DOWN);
    } else if (key == upKey) {
      box.travel(Direction::UP);
    } else if (key == leftKey) {
      box.travel(Direction::LEFT);
    } else if (key == rightKey) {
      box.travel(Direction::RIGHT);
    }
  }

  void keyUp(SDLKey key) {
    if(key == downKey) {
      box.stop(Direction::DOWN);
    } else if (key == upKey) {
      box.stop(Direction::UP);
    } else if (key == leftKey) {
      box.stop(Direction::LEFT);
    } else if (key == rightKey) {
      box.stop(Direction::RIGHT);
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

    if(xx < box.bounds.x1) {
      xx = box.bounds.x1;
      xVelocity = xVelocity * -1;
    } else if (xx > (box.bounds.x2 - box.w)) {
      xx = box.bounds.x2 - box.w;
      xVelocity = xVelocity * -1;
    }

    if(yy < box.bounds.y1) {
      yy = box.bounds.y1;
      yVelocity = yVelocity * -1;
    } else if (yy > (box.bounds.y2 - box.h)) {
      yy = (box.bounds.y2 - box.h);
      yVelocity = yVelocity * -1;
    }

    if(xx <= (player1.box.x + player1.box.w)) {
      if((yy + box.h) > player1.box.y && yy < (player1.box.y + player1.box.h)) {
        xVelocity = xVelocity * -1;
      }
    }

    if((xx + box.w) >= (player2.box.x)) {
      if((yy + box.h) > player2.box.y && yy < (player2.box.y + player2.box.h)) {
        xVelocity = xVelocity * -1;
      }
    }

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
  player.init(SDLK_w, SDLK_s, SDLK_a, SDLK_d);
  player.box.init(80, 280);
  player.box.bounds.init(player.box.x, 0, player.box.x + player.box.w, 720);

  Player player2;
  player2.init(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT);
  player2.box.init(1130, 280);
  player2.box.bounds.init(player2.box.x, 0, player2.box.x + player2.box.w, 720);

  Ball ball;
  ball.init();
  ball.box.init(300, 300, 30, 30);
  ball.box.bounds.init(0, 0, 1280, 720);

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

    player.update();
    player2.update();
    ball.update(player, player2);

    SDL_GL_SwapBuffers();
  }

  return 0;
}
