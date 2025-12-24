#ifndef FALLING_SANDS_UTIL
#define FALLING_SANDS_UTIL

#include "constants.h"
#include "main.h"
#include <raylib.h>
#include <raymath.h>

static inline Vector2 screen_to_map_pos(Vector2 screenPos) {
  return Vector2Scale(screenPos, 1.0f / CELL_SIZE);
}

static inline Vector2 get_mouse_pos_on_map() {
  Vector2 mousePos = {.x = GetMouseX() * 1.0f, .y = GetMouseY() * 1.0f};
  Vector2 mapPos = screen_to_map_pos(mousePos);

  return mapPos;
}

static inline const char *cell_type_to_str(CellType cType) {
  const char *ret = "";
  switch (cType) {
  case FALLING:
    ret = "SAND";
    break;
  case SOLID:
    ret = "ROCK";
    break;
  case EMPTY:
    break;
  }

  return ret;
}

#endif
