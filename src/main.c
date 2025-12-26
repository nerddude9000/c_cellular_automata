#include "main.h"
#include "constants.h"
#include "util.h"

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

Cell new_cell(CellType cType) {
  Cell c = {
      .type = (uint8_t)cType, .tempreture = STARTING_TEMP, .flammable = false};

  switch (cType) {
  case FIRE:
    c.lifetime = FIRE_LIFETIME;
    c.tempreture = HOT_CELLS_STARTING_TEMP;
    break;
  case WOOD:
    c.flammable = true;
    break;
  case FALLING:
  case ROCK:
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
  case ROCK:
    state->cellCount.rock += diff;
    break;
  case WOOD:
    state->cellCount.wood += diff;
    break;
  case FIRE:
    state->cellCount.fire += diff;
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
      state->map[y * MAP_SIZE + x] = c;
    }
  }
}

void insert_cell_at(MapState *state, int x, int y, CellType newType) {
  // NOTE: we never replace cell if it's of the same type, and i didn't think it
  // would be a good idea to do so, maybe i'll add a boolean in constants.h in
  // the future.

  Cell *oldCell = get_cell(state->map, x, y);

  if (oldCell == NULL) // out of bounds
    return;
  if (newType == oldCell->type)
    return;
  if (!REPLACE_CELL_OF_DIFFERENT_TYPE_ON_INSERT && oldCell->type != EMPTY)
    return;

  if (oldCell->type != EMPTY)
    update_cell_count(state, oldCell->type, -1);

  update_cell_count(state, newType, 1);
  *oldCell = new_cell(newType);
}

void remove_cell_at(MapState *state, int x, int y) {
  Cell *oldCell = get_cell(state->map, x, y);

  // must be in bounds AND not empty to remove it.
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
      case ROCK:
        color = GRAY;
        break;
      case WOOD:
        color = BROWN;
        break;
      case FIRE:;
        int halfLife = FIRE_LIFETIME / 2;
        color = c->lifetime > halfLife ? RED : ORANGE;
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

void draw_ui_text(char *txt, bool leftSide, bool resetOffset) {
  static int yOffsetLeft = 0;
  static int yOffsetRight = 0;

  if (resetOffset) {
    if (leftSide)
      yOffsetLeft = 0;
    else
      yOffsetRight = 0;
  }

  int txtWidth = MeasureText(txt, FONT_SIZE);
  int y = leftSide ? yOffsetLeft : yOffsetRight;
  int x = leftSide ? 0 : WINDOW_SIZE - txtWidth;

  DrawText(txt, x, y, FONT_SIZE, LIME);

  if (leftSide)
    yOffsetLeft += FONT_SIZE;
  else
    yOffsetRight += FONT_SIZE;
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
        if (bot != NULL && (bot->type == EMPTY || bot->type == FIRE)) {
          bot->type = FALLING;
          c->type = EMPTY;
        }
        break;

      case FIRE:;
        if (c->lifetime <= 0 || c->tempreture <= HOT_CELLS_STARTING_TEMP / 2)
          remove_cell_at(state, x, y);
        else
          c->lifetime--;

        // fire spread
        for (int cx = -1; cx <= 1; cx++) {
          for (int cy = -1; cy <= 1; cy++) {
            if (cx == 0 && cy == 0)
              continue; // we don't check own cell

            Cell *neighbor = get_cell(state->map, x + cx, y + cy);
            neighbor->tempreture = (int16_t)min(
                neighbor->tempreture + FIRE_HEATING_PER_FRAME, INT16_MAX);
          }
        }
        break;
      case ROCK:
        break;
      case WOOD:
        if (c->tempreture >= WOOD_SET_ON_FIRE_TEMP) {
          remove_cell_at(state, x, y);
          insert_cell_at(state, x, y, FIRE);
        }
        break;
      case EMPTY:
        break;
      }

      // These run on ALL cells
      if (c->tempreture > STARTING_TEMP) {
        c->tempreture =
            (int16_t)max(c->tempreture - TEMP_RESET_STEP, STARTING_TEMP);
      } else if (c->tempreture < STARTING_TEMP) {
        c->tempreture =
            (int16_t)min(c->tempreture + TEMP_RESET_STEP, STARTING_TEMP);
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
  else if (IsKeyPressed(K_SELECT_ROCK))
    state->typeToInsert = ROCK;
  else if (IsKeyPressed(K_SELECT_WOOD))
    state->typeToInsert = WOOD;
  else if (IsKeyPressed(K_SELECT_FIRE))
    state->typeToInsert = FIRE;

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

#define LEFT_SIDE true
    // ---- Draw left side text ----
    char *txt = (char *)TextFormat("FPS %d", GetFPS());
    draw_ui_text(txt, LEFT_SIDE, true);

    txt =
        (char *)TextFormat("SELECTED %s", cell_type_to_str(state.typeToInsert));
    draw_ui_text(txt, LEFT_SIDE, false);

    if (state.isPaused)
      draw_ui_text("PAUSED", LEFT_SIDE, false);

#define RIGHT_SIDE false
    // ---- Draw right side text ----
    txt = (char *)TextFormat("%s\t%d", cell_type_to_str(FALLING),
                             state.cellCount.falling);
    draw_ui_text(txt, RIGHT_SIDE, true);
    txt = (char *)TextFormat("%s\t%d", cell_type_to_str(ROCK),
                             state.cellCount.rock);
    draw_ui_text(txt, RIGHT_SIDE, false);
    txt = (char *)TextFormat("%s\t%d", cell_type_to_str(WOOD),
                             state.cellCount.wood);
    draw_ui_text(txt, RIGHT_SIDE, false);
    txt = (char *)TextFormat("%s\t%d", cell_type_to_str(FIRE),
                             state.cellCount.fire);
    draw_ui_text(txt, RIGHT_SIDE, false);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
