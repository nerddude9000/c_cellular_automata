#include "constants.h"
#include <raylib.h>

int main() {
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
