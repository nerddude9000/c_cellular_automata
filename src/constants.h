#ifndef APP_CONSTANTS
#define APP_CONSTANTS

// square for now
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAP_SIZE SCREEN_WIDTH / 10
#define CELL_SIZE MAP_SIZE / SCREEN_WIDTH

typedef enum { EMPTY, SOLID, FALLING } CellType;

typedef struct {
  CellType type;
} Cell;

#endif
