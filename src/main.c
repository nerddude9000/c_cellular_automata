#include "main.h"
#include "constants.h"
#include "util.h"

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define FONT_SIZE 20

Cell new_cell(void) {
  Cell c = {.type = EMPTY};

  return c;
}

Cell *get_cell(Cell map[], int x, int y) {
  if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
    return NULL;

  return &(map[y * MAP_SIZE + x]);
}

void init_state(MapState *state) {
  // TODO: find a better way to zero these.
  state->cellCount.falling = 0;
  state->cellCount.solid = 0;

  for (int x = 0; x < MAP_SIZE; x++) {
    for (int y = 0; y < MAP_SIZE; y++) {
      Cell c = new_cell();

      if (y == 5) {
        c.type = FALLING;
        state->cellCount.falling++;
      } else if (y == 30 && x > 10 && x < 30) {
        c.type = SOLID;
        state->cellCount.solid++;
      }

      state->map[y * MAP_SIZE + x] = c;
    }
  }
}

void insert_cell_at(MapState *state, int x, int y, CellType cType) {
  Cell neoCell = new_cell();
  neoCell.type = (uint8_t)cType;

  Cell *oldCell = get_cell(state->map, x, y);
  if (oldCell != NULL && oldCell->type == EMPTY) {
    *oldCell = neoCell;
  }
}

void remove_cell_at(MapState *state, int x, int y) {
  Cell *oldCell = get_cell(state->map, x, y);
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
void write_map_to_file(MapState *state) {
  FILE *f = fopen("map.tmp", "w");

  for (int y = 0; y < MAP_SIZE; y++) {
    for (int x = 0; x < MAP_SIZE; x++) {
      fprintf(f, "%d", get_cell(state->map, x, y)->type);
    }
    fprintf(f, "\n");
  }

  fclose(f);
}

void handle_input(MapState *state, bool *isPaused) {
  // TODO: add brush size for some operations.

  if (IsKeyPressed(K_WRITE_MAP))
    write_map_to_file(state);

  if (IsKeyPressed(K_RESET))
    init_state(state);

  if (IsMouseButtonDown(M_INSERT) && IsCursorOnScreen()) {
    IVector2 mousePos = {.x = GetMouseX(), .y = GetMouseY()};
    IVector2 mapPos = screen_to_map_pos(mousePos);

    // for now it will just insert 1 FALLING cell.
    // TODO: add selecting CellType to insert.
    insert_cell_at(state, mapPos.x, mapPos.y, FALLING);
  }

  if (IsMouseButtonDown(M_REMOVE) && IsCursorOnScreen()) {
    IVector2 mousePos = {.x = GetMouseX(), .y = GetMouseY()};
    IVector2 mapPos = screen_to_map_pos(mousePos);

    remove_cell_at(state, mapPos.x, mapPos.y);
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

  MapState state;
  init_state(&state);

  // window is always square
  InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Falling sandc");

  bool isPaused = false;
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    handle_input(&state, &isPaused);

    if (!isPaused)
      sim_map(state.map);

    BeginDrawing();

    ClearBackground(BLACK);
    draw_map(state.map);

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
