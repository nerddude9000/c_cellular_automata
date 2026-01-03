#ifndef FALLING_SANDS_UTIL
#define FALLING_SANDS_UTIL

#include "config.h"
#include "main.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

static inline int min(int a, int b) { return a > b ? b : a; }
static inline int max(int a, int b) { return a > b ? a : b; }
static inline int clamp(int n, int nMin, int nMax) {
  return max(nMin, min(n, nMax));
}

static inline Vector2 screen_to_map_pos(Vector2 screenPos) {
  return Vector2Scale(screenPos, 1.0f / ((int)CELL_SIZE));
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
  case STEAM:
  case WATER:
    ret = "WATER";
    break;
  case WOOD:
    ret = "WOOD";
    break;
  case FIRE:
    ret = "FIRE";
    break;
  case ROCK:
    ret = "ROCK";
    break;
  case EMPTY:
    break;
  }

  return ret;
}

static inline bool can_fall_to(Cell *map, int x, int y) {
  Cell *c = get_cell(map, x, y);

  return c != NULL && (c->type == EMPTY || c->type == FIRE || c->type == STEAM);
}

static inline bool can_rise_to(Cell *map, int x, int y) {
  Cell *c = get_cell(map, x, y);

  return c != NULL && (c->type == EMPTY || c->type == FIRE || c->type == WATER);
}

#endif
