#ifndef FALLING_SANDS_H
#define FALLING_SANDS_H

#include "constants.h"
#include <raylib.h>

typedef enum { EMPTY = 0, SOLID, FALLING } CellType;

typedef struct {
  uint8_t type;
} Cell;

typedef struct {
  int solid;
  int falling;
} CellCount;

typedef struct {
  Cell map[MAP_SIZE * MAP_SIZE]; // 1D array, but simulates a 2D grid
                                 // (idx = y * width + x)
  CellCount cellCount;
} MapState;

Cell new_cell(void);
Cell *get_cell(Cell map[], int x, int y);
void init_state(MapState *state);
void draw_map(Cell map[]);
void sim_map(MapState *state);
void write_map_to_file(MapState *state);
void update_cell_count(MapState *state, CellType cType, int diff);
void insert_cell_at(MapState *state, int x, int y, CellType cType);
void remove_cell_at(MapState *state, int x, int y);
void handle_input(MapState *state, bool *isPaused);

#endif
