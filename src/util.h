#ifndef FALLING_SANDS_UTIL
#define FALLING_SANDS_UTIL

#include "constants.h"
#include "main.h"
#include <raylib.h>
#include <raymath.h>

static inline Vector2 screen_to_map_pos(Vector2 screenPos) {
  // NOTE: i could NOT make this shorter, for some reason try to make it a one
  // liner screws up the result!
  float x = (float)screenPos.x;
  float y = (float)screenPos.y;

  x /= (float)CELL_SIZE;
  y /= (float)CELL_SIZE;

  return (Vector2){x, y};
}

static inline Vector2 get_mouse_pos_on_map(void) {
  Vector2 mousePos = {(float)GetMouseX(), (float)GetMouseY()};
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
  case WOOD:
    ret = "WOOD";
    break;
  case EMPTY:
    break;
  }

  return ret;
}

#endif
