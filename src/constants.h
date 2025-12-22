#ifndef APP_CONSTANTS
#define APP_CONSTANTS

#include <stdint.h>

// square for now
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAP_SIZE SCREEN_WIDTH / 10
#define CELL_SIZE SCREEN_WIDTH / (MAP_SIZE)

typedef enum { EMPTY = 0, SOLID, FALLING } CellType;

typedef struct {
  uint8_t type;
} Cell;

#endif
