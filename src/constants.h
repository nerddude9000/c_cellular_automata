#ifndef APP_CONSTANTS
#define APP_CONSTANTS

#include <raylib.h>

// ---- Customizable ----
// screen (and map) must be square, so one constant for each is enough
#define WINDOW_SIZE 1000
#define MAP_SIZE 50 // WINDOW_SIZE % MAP_SIZE must be 0 to avoid visual bugs.

#define DO_NOT_REPLACE_CELL_ON_INSERT false
enum Shortcuts {
  K_SELECT_FALLING = KEY_ONE,
  K_SELECT_SOLID = KEY_TWO,
  K_PAUSE = KEY_P,
  K_WRITE_MAP = KEY_W,
  K_RESET = KEY_R,
  M_INSERT = MOUSE_LEFT_BUTTON,
  M_REMOVE = MOUSE_RIGHT_BUTTON
};

// for now, these are not like real life (of course) for performance reasons,
// see Cell struct definition in main.h for more info.
#define WOOD_SET_ON_FIRE_TEMP 2000;

// ---- DON'T TOUCH ----
#define CELL_SIZE WINDOW_SIZE / MAP_SIZE

#endif
