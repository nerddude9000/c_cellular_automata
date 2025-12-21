#include "constants.h"
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

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
  if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
    return NULL;

  return &(map[x][y]);
}

void sim_and_draw_map(Cell map[MAP_SIZE][MAP_SIZE]) {
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
void print_map(Cell map[MAP_SIZE][MAP_SIZE]) {
  FILE *f = fopen("map.tmp", "w");
  fprintf(f, "Map = \n");

  for (int x = 0; x < MAP_SIZE; x++) {
    fprintf(f, "{");

    for (int y = 0; y < MAP_SIZE; y++) {
      fprintf(f, "%d,", map[x][y].type);
    }

    fprintf(f, "}\n");
  }

  fclose(f);
}

int main() {
  Cell map[MAP_SIZE][MAP_SIZE];
  init_map(map);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling sands");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();

    if (IsKeyPressed(KEY_P))
      print_map(map);

    ClearBackground(BLACK);
    sim_and_draw_map(map);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
