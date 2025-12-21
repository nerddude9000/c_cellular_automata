#include "constants.h"
#include <raylib.h>
#include <stddef.h>

void init_map(Cell map[MAP_SIZE][MAP_SIZE]) {
  for (int x = 0; x < MAP_SIZE; x++) {
    for (int y = 0; y < MAP_SIZE; y++) {
      Cell c;
      if (y == 10) {
        c.type = FALLING;
      } else {
        c.type = EMPTY;
      }
      map[x][y] = c;
    }
  }
}

Cell *get_cell(Cell map[MAP_SIZE][MAP_SIZE], int x, int y) {
  if (x < 0 || x > MAP_SIZE || y < 0 || y > MAP_SIZE)
    return NULL;

  return &(map[x][y]);
}

void draw_map(Cell map[MAP_SIZE][MAP_SIZE]) {
  for (int x = 0; x < MAP_SIZE; x++) {
    for (int y = 0; y < MAP_SIZE; y++) {
      Cell *c = get_cell(map, x, y);

      switch (c->type) {
      case EMPTY:
        break;
      case FALLING:
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                      YELLOW);

/*         if (get_cell(map, x, y + 1)->type == EMPTY) {
          c->type = EMPTY;
          get_cell(map, x, y + 1)->type = FALLING;
        } */

        break;
      case SOLID:
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                      BLACK);
        break;
      }
    }
  }
}

int main() {
  Cell map[MAP_SIZE][MAP_SIZE];
  init_map(map);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling sands");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(BLACK);
    draw_map(map);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
