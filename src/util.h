#ifndef FALLING_SANDS_UTIL
#define FALLING_SANDS_UTIL

#include "constants.h"

typedef struct {
  int x, y;
} IVector2;

static inline IVector2 screen_to_map_pos(IVector2 screenPos) {
  float x = (float)screenPos.x;
  float y = (float)screenPos.y;

  x /= 1.f * CELL_SIZE;
  y /= 1.f * CELL_SIZE;

  return (IVector2){(int)x, (int)y};
}

#endif
