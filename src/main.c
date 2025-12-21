#include "constants.h"
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

Cell *get_cell(Cell *map, int x, int y) {
  if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
    return NULL;

  return &(map[y * MAP_SIZE + x]);
}

void init_map(Cell *map) {
  for (int x = 0; x < MAP_SIZE; x++) {
    for (int y = 0; y < MAP_SIZE; y++) {
      Cell c;

      if (y == 5) {
        c.type = FALLING;
      } else if (y == 30 && x > 10 && x < 30) {
        c.type = SOLID;
      } else {
        c.type = EMPTY;
      }

      map[y * MAP_SIZE + x] = c;
    }
  }
}

void sim_and_draw_map(Cell *map) {
  // we iterate backwards as to not process the same sand twice.
  // i think this is only temporary, as adding rising smoke will cause the same
  // problem. maybe we can use two maps (current and next) in the future.
  for (int y = MAP_SIZE - 1; y >= 0; y--) {
    for (int x = 0; x < MAP_SIZE; x++) {
      Cell *c = get_cell(map, x, y);

      switch (c->type) {
      case EMPTY:
        break;
      case FALLING:
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                      YELLOW);

        Cell *bot = get_cell(map, x, y + 1);
        if (bot != NULL && bot->type == EMPTY) {
          bot->type = FALLING;
          c->type = EMPTY;
        }

        break;
      case SOLID:
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                      WHITE);
        break;
      }
    }
  }
}

// for testing
void print_map(Cell *map) {
  FILE *f = fopen("map.tmp", "w");

  for (int x = 0; x < MAP_SIZE; x++) {
    for (int y = 0; y < MAP_SIZE; y++) {
      fprintf(f, "%d", get_cell(map, x, y)->type);
    }
    fprintf(f, "\n");
  }

  fclose(f);
}

int main(void) {
  Cell map[MAP_SIZE *
           MAP_SIZE]; // 1D array, but simulates a 2D grid (idx = y * width + x)
  init_map(map);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling sands");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_P))
      print_map(map);

    if (IsKeyPressed(KEY_R))
      init_map(map);

    BeginDrawing();

    ClearBackground(BLACK);
    sim_and_draw_map(map);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
