#ifndef APP_CONSTANTS
#define APP_CONSTANTS

#include <raylib.h>

// ---- Customizable ----
// screen (and map) must be square, so one constant for each is enough
#define WINDOW_SIZE 1000
#define MAP_SIZE 500 // WINDOW_SIZE % MAP_SIZE must be 0 to avoid visual bugs.
#define FPS 120

#define REPLACE_CELL_OF_DIFFERENT_TYPE_ON_INSERT true
#define MAX_BRUSH_SIZE 64
enum Shortcuts {
  K_SELECT_FALLING = KEY_ONE,
  K_SELECT_ROCK = KEY_TWO,
  K_SELECT_WOOD = KEY_THREE,
  K_SELECT_FIRE = KEY_FOUR,
  K_PAUSE = KEY_P,
  K_WRITE_MAP = KEY_W,
  K_RESET = KEY_R,
  M_INSERT = MOUSE_LEFT_BUTTON,
  M_REMOVE = MOUSE_RIGHT_BUTTON
};

// for now, these are not like real life (of course) for performance reasons,
// see Cell struct definition in main.h for more info.
#define STARTING_TEMP 100
// how much the cell cools/heats up to the STARTING_TEMP per frame.
#define TEMP_RESET_STEP 20

#define HOT_CELLS_STARTING_TEMP 2000
#define FIRE_HEATING_PER_FRAME 50
#define WOOD_SET_ON_FIRE_TEMP 2000

// in frames, if the game is 60 FPS then it will last for (60*y)
// frames (y seconds).
#define FIRE_LIFETIME (FPS * 3)

#define FONT_SIZE 20

#endif
