#ifndef APP_CONSTANTS
#define APP_CONSTANTS

#include <raylib.h>
#include <stdint.h>

enum Shortcuts {
  K_PAUSE = KEY_P,
  K_WRITE_MAP = KEY_W,
  K_RESET = KEY_R,
  M_INSERT = MOUSE_LEFT_BUTTON,
  M_REMOVE = MOUSE_RIGHT_BUTTON
};

// screen (and map) must be square, so one constant for each is enough
#define WINDOW_SIZE 800
#define MAP_SIZE 80 // WINDOW_SIZE % MAP_SIZE must be 0 to avoid visual bugs.
#define CELL_SIZE WINDOW_SIZE / MAP_SIZE

typedef enum { EMPTY = 0, SOLID, FALLING } CellType;

typedef struct {
  uint8_t type;
} Cell;

#endif
