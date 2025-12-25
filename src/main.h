#ifndef FALLING_SANDS_H
#define FALLING_SANDS_H

#include "constants.h"
#include <raylib.h>
#include <stdint.h>

typedef enum
    __attribute__((__packed__)) { // this makes it a packed enum (1 byte)
      EMPTY = 0,
      SOLID,
      FALLING,
      WOOD
    } CellType;

typedef struct {
  uint8_t type;
  bool flammable;
  int16_t tempreture; // using i16 keeps the struct small (floats add 4 bytes
                      // per cell at least). i might change it in the future if
                      // we have enough padding space, but for now i like it
                      // nice and tidy (4 bytes (: )
} Cell;

typedef struct {
  int solid;
  int falling;
  int wood;
} CellCount;

typedef struct {
  Cell map[MAP_SIZE * MAP_SIZE]; // Row-Major Order; 1D array, but simulates a
                                 // 2D grid (idx = y * width + x)
  CellCount cellCount;
  bool isPaused;
  CellType typeToInsert;
} MapState;

Cell new_cell(CellType cType);
Cell *get_cell(Cell map[], int x, int y);
void init_map(MapState *state);
void draw_map(Cell map[]);
void draw_ui_text(char *txt, bool leftSide, bool resetOffset);
void sim_map(MapState *state);
void write_map_to_file(MapState *state);
void update_cell_count(MapState *state, CellType cType, int diff);
void insert_cell_at(MapState *state, int x, int y, CellType cType);
void remove_cell_at(MapState *state, int x, int y);
void handle_input(MapState *state);

#endif
