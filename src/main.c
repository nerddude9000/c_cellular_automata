#include "main.h"
#include "constants.h"
#include "shortcuts.h"

#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define FONT_SIZE 24

Cell new_cell(void) {
  Cell c = {.type = EMPTY};

  return c;
}

Cell *get_cell(Cell map[], int x, int y) {
  if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
    return NULL;

  return &(map[y * MAP_SIZE + x]);
}

void init_map(Cell map[]) {
  for (int x = 0; x < MAP_SIZE; x++) {
    for (int y = 0; y < MAP_SIZE; y++) {
      Cell c = new_cell();

      if (y == 5) {
        c.type = FALLING;
      } else if (y == 30 && x > 10 && x < 30) {
        c.type = SOLID;
      }

      map[y * MAP_SIZE + x] = c;
    }
  }
}

Vector2 screen_to_map_pos(Vector2 screenPos) {
  return Vector2Scale(screenPos, 1.0f / ((int)(CELL_SIZE)));
}

void insert_cell_at(Cell map[], int x, int y, CellType cType) {
  Cell neoCell = new_cell();
  neoCell.type = (uint8_t)cType;

  Cell *oldCell = get_cell(map, x, y);
  if (oldCell != NULL && oldCell->type == EMPTY) {
    *oldCell = neoCell;
  }
}

void remove_cell_at(Cell map[], int x, int y) {
  Cell *oldCell = get_cell(map, x, y);
  if (oldCell != NULL && oldCell->type != EMPTY) {
    *oldCell = new_cell();
  }
}

void draw_map(Cell map[]) {
  for (int y = 0; y < MAP_SIZE; y++) {
    for (int x = 0; x < MAP_SIZE; x++) {
      Cell *c = get_cell(map, x, y);

      if (c->type == FALLING)
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                      YELLOW);
      else if (c->type == SOLID)
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GRAY);
    }
  }
}

void sim_map(Cell map[]) {
  // we iterate backwards as to not process the same sand twice.
  // i think this is only temporary, as adding rising smoke will cause the same
  // problem. maybe we can use two maps (current and next) in the future.
  for (int y = MAP_SIZE - 1; y >= 0; y--) {
    for (int x = 0; x < MAP_SIZE; x++) {
      Cell *c = get_cell(map, x, y);

      switch (c->type) {
      case FALLING:; // NOTE: how does this ';' make the compiler shut up?!
        Cell *bot = get_cell(map, x, y + 1);
        if (bot != NULL && bot->type == EMPTY) {
          bot->type = FALLING;
          c->type = EMPTY;
        }

        break;
      case SOLID:
        break;
      case EMPTY:
      default:
        break;
      }
    }
  }
}

// for testing
void write_map_to_file(Cell map[]) {
  FILE *f = fopen("map.tmp", "w");

  for (int y = 0; y < MAP_SIZE; y++) {
    for (int x = 0; x < MAP_SIZE; x++) {
      fprintf(f, "%d", get_cell(map, x, y)->type);
    }
    fprintf(f, "\n");
  }

  fclose(f);
}

void handle_input(Cell map[], bool *isPaused) {
  // TODO: add brush size for some operations.

  if (IsKeyPressed(K_WRITE_MAP))
    write_map_to_file(map);

  if (IsKeyPressed(K_RESET))
    init_map(map);

  if (IsMouseButtonDown(M_INSERT) && IsCursorOnScreen()) {
    Vector2 mousePos = {.x = (float)GetMouseX(), .y = (float)GetMouseY()};
    Vector2 mapPos = screen_to_map_pos(mousePos);

    // for now it will just insert 1 FALLING cell.
    // TODO: add selecting CellType to inser.
    insert_cell_at(map, (int)mapPos.x, (int)mapPos.y, FALLING);
  }

  if (IsMouseButtonDown(M_REMOVE) && IsCursorOnScreen()) {
    Vector2 mousePos = {.x = (float)GetMouseX(), .y = (float)GetMouseY()};
    Vector2 mapPos = screen_to_map_pos(mousePos);

    remove_cell_at(map, (int)mapPos.x, (int)mapPos.y);
  }

  if (IsKeyPressed(K_PAUSE)) {
    *isPaused = !(*isPaused);
  }
}

int main(void) {
  if (WINDOW_SIZE % MAP_SIZE != 0) {
    printf("Window size must be divisible by map size to avoid visual bugs.\n");
    printf("Please change this in constants.h and recompile.\n");
    return 1;
  }

  Cell map[MAP_SIZE *
           MAP_SIZE]; // 1D array, but simulates a 2D grid (idx = y * width + x)
  init_map(map);

  // window is always square
  InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Falling sandc");

  bool isPaused = false;
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    handle_input(map, &isPaused);

    if (!isPaused)
      sim_map(map);

    BeginDrawing();

    ClearBackground(BLACK);
    draw_map(map);

    // ---- Draw left side text ----
    DrawFPS(0, 0);
    if (isPaused)
      DrawText("PAUSED", 0, FONT_SIZE, FONT_SIZE, RED);

    // ---- Draw right side text ----
    // TODO: display each celltype's count.

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
