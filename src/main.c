#include "main.h"
#include "constants.h"
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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

void handle_input(Cell map[]) {
  if (IsKeyPressed(KEY_P))
    write_map_to_file(map);

  if (IsKeyPressed(KEY_R))
    init_map(map);

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && IsCursorOnScreen()) {
    Vector2 mousePos = {.x = (float)GetMouseX(), .y = (float)GetMouseY()};
    Vector2 mapPos = screen_to_map_pos(mousePos);

    // for now it will just insert 1 FALLING cell.
    // TODO: add brush size and insert.
    // TODO: add selecting CellType to inser.
    insert_cell_at(map, (int)mapPos.x, (int)mapPos.y, FALLING);
  }

  if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && IsCursorOnScreen()) {
    Vector2 mousePos = {.x = (float)GetMouseX(), .y = (float)GetMouseY()};
    Vector2 mapPos = screen_to_map_pos(mousePos);

    // TODO: add brush size.
    remove_cell_at(map, (int)mapPos.x, (int)mapPos.y);
  }
}

int main(void) {
  Cell map[MAP_SIZE *
           MAP_SIZE]; // 1D array, but simulates a 2D grid (idx = y * width + x)
  init_map(map);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling sandc");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    handle_input(map);
    sim_map(map);

    BeginDrawing();

    ClearBackground(BLACK);
    draw_map(map);
    DrawFPS(0, 0);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
