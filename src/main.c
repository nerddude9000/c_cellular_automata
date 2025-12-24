#include "main.h"
#include "constants.h"
#include "util.h"

#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define FONT_SIZE 20

Cell new_cell(CellType cType) {
  Cell c = {.type = (uint8_t)cType, .tempreture = 0, .flammable = false};

  switch (cType) {
  case WOOD:
    c.flammable = true;
    break;
  case FALLING:
  case SOLID:
  case EMPTY:
    break;
  }

  return c;
}

Cell *get_cell(Cell map[], int x, int y) {
  if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
    return NULL;

  return &(map[y * MAP_SIZE + x]);
}

void update_cell_count(MapState *state, CellType cType, int diff) {
  switch (cType) {
  case FALLING:
    state->cellCount.falling += diff;
    break;
  case SOLID:
    state->cellCount.solid += diff;
    break;
  case WOOD:
    state->cellCount.wood += diff;
    break;
  case EMPTY:
    break;
  }
}

void init_map(MapState *state) {
  state->cellCount = (CellCount){0};

  // for looping through the map (here and in other functions), making the rows
  // (y) first on the loop, then cols (x), is much much faster because of cache.
  for (int y = 0; y < MAP_SIZE; y++) {
    for (int x = 0; x < MAP_SIZE; x++) {
      Cell c = new_cell(EMPTY);

      if (y == 5) {
        c.type = FALLING;
        update_cell_count(state, FALLING, 1);
      } else if (y == 30 && x > 10 && x < 30) {
        c.type = SOLID;
        update_cell_count(state, SOLID, 1);
      }

      state->map[y * MAP_SIZE + x] = c;
    }
  }
}

void insert_cell_at(MapState *state, int x, int y, CellType cType) {
  // NOTE: we never replace cell if it's of the same type, and i didn't think it
  // would be a good idea to do so, maybe i'll add a boolean in constants.h in
  // the future.

  Cell neoCell = new_cell(cType);

  Cell *oldCell = get_cell(state->map, x, y);

  if (oldCell == NULL)
    return;
  if (neoCell.type == oldCell->type)
    return;
  if (DO_NOT_REPLACE_CELL_ON_INSERT && oldCell->type != EMPTY)
    return;

  if (oldCell->type == EMPTY)
    update_cell_count(state, cType, 1);
  else // here oldCell must be of different type than neoCell
    update_cell_count(state, oldCell->type, -1);

  *oldCell = neoCell;
}

void remove_cell_at(MapState *state, int x, int y) {
  Cell *oldCell = get_cell(state->map, x, y);
  if (oldCell != NULL && oldCell->type != EMPTY) {
    update_cell_count(state, oldCell->type, -1);
    *oldCell = new_cell(EMPTY);
  }
}

void draw_map(Cell map[]) {
  for (int y = 0; y < MAP_SIZE; y++) {
    for (int x = 0; x < MAP_SIZE; x++) {
      Cell *c = get_cell(map, x, y);
      Color color;

      switch ((CellType)c->type) {
      case FALLING:
        color = YELLOW;
        break;
      case SOLID:
        color = GRAY;
        break;
      case WOOD:
        color = BROWN;
        break;
      case EMPTY:
        break;
      }

      if (c->type != EMPTY)
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                      color);
    }
  }
}

void draw_cell_count(CellType cType, int count, bool resetOffset) {
  static int yOffset = 0;

  if (resetOffset)
    yOffset = 0;
  else
    yOffset += FONT_SIZE;

  char *txt = (char *)TextFormat("%s\t%d", cell_type_to_str(cType), count);
  int width = MeasureText(txt, FONT_SIZE);
  DrawText(txt, WINDOW_SIZE - width, yOffset, FONT_SIZE, LIME);
}

void sim_map(MapState *state) {
  // we iterate backwards as to not process the same sand twice.
  // i think this is only temporary, as adding rising smoke will cause the same
  // problem. maybe we can use two maps (current and next) in the future.
  for (int y = MAP_SIZE - 1; y >= 0; y--) {
    for (int x = 0; x < MAP_SIZE; x++) {
      Cell *c = get_cell(state->map, x, y);

      switch ((CellType)c->type) {
      case FALLING:; // NOTE: how does this ';' make the compiler shut up?!
        Cell *bot = get_cell(state->map, x, y + 1);
        if (bot != NULL && bot->type == EMPTY) {
          bot->type = FALLING;
          c->type = EMPTY;
        }
        break;
      case SOLID:
        break;
      case WOOD:
        break;
      case EMPTY:
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

void handle_input(MapState *state) {
  // TODO: add brush size for some operations.

  if (IsKeyPressed(K_SELECT_FALLING))
    state->typeToInsert = FALLING;
  else if (IsKeyPressed(K_SELECT_SOLID))
    state->typeToInsert = SOLID;
  else if (IsKeyPressed(K_SELECT_WOOD))
    state->typeToInsert = WOOD;

  if (IsKeyPressed(K_WRITE_MAP))
    write_map_to_file(state);

  if (IsKeyPressed(K_RESET))
    init_map(state);

  if (IsMouseButtonDown(M_INSERT) && IsCursorOnScreen()) {
    Vector2 mapPos = get_mouse_pos_on_map();
    insert_cell_at(state, (int)mapPos.x, (int)mapPos.y, state->typeToInsert);
  }

  if (IsMouseButtonDown(M_REMOVE) && IsCursorOnScreen()) {
    Vector2 mapPos = get_mouse_pos_on_map();
    remove_cell_at(state, (int)mapPos.x, (int)mapPos.y);
  }

  if (IsKeyPressed(K_PAUSE)) {
    state->isPaused = !(state->isPaused);
  }
}

int main(void) {
  if (WINDOW_SIZE % MAP_SIZE != 0) {
    printf("Window size must be divisible by map size to avoid visual bugs.\n");
    printf("Please change this in constants.h and recompile.\n");
    return 1;
  }

  MapState state = {.isPaused = false, .typeToInsert = FALLING};

  init_map(&state);

  // window is always square
  InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Falling sandc");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    handle_input(&state);

    if (!state.isPaused)
      sim_map(&state);

    BeginDrawing();

    ClearBackground(BLACK);
    draw_map(state.map);

    // ---- Draw left side text ----
    DrawFPS(0, 0);

    char *txt = (char *)TextFormat("SELECTED: %s",
                                   cell_type_to_str(state.typeToInsert));
    DrawText(txt, 0, FONT_SIZE, FONT_SIZE, LIME);

    if (state.isPaused)
      DrawText("PAUSED", 0, FONT_SIZE * 2, FONT_SIZE, RED);

    // ---- Draw right side text ----
    draw_cell_count(FALLING, state.cellCount.falling, true);
    draw_cell_count(SOLID, state.cellCount.solid, false);
    draw_cell_count(WOOD, state.cellCount.solid, false);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
