#include "constants.h"
#include <raylib.h>

void init_map(Cell map[MAP_SIZE][MAP_SIZE]) {
  for (int x = 0; x < MAP_SIZE; x++) {
    for (int y = 0; y < MAP_SIZE; y++) {
      Cell c;
      c.type = EMPTY;
      map[x][y] = c;
    }
  }
}

int main() {
  Cell map[MAP_SIZE][MAP_SIZE];
  init_map(map);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bismillah");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
