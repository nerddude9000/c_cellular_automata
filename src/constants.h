#ifndef APP_CONSTANTS
#define APP_CONSTANTS

#include <stdint.h>

// screen (and map) must be square, so one constant for each is enough
#define WINDOW_SIZE 800
#define MAP_SIZE 80 // WINDOW_SIZE % MAP_SIZE must be 0 to avoid visual bugs.
#define CELL_SIZE WINDOW_SIZE / MAP_SIZE

typedef enum { EMPTY = 0, SOLID, FALLING } CellType;

typedef struct {
  uint8_t type;
} Cell;

#endif
