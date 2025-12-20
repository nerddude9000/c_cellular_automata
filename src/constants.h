#ifndef APP_CONSTANTS
#define APP_CONSTANTS

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef enum { EMPTY, SOLID, FALLING } CellType;

typedef struct {
  CellType type;
} Cell;

#endif
