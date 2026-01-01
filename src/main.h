#ifndef FALLING_SANDS_H
#define FALLING_SANDS_H

#include "config.h"
#include <raylib.h>
#include <stdint.h>

typedef enum
    __attribute__((__packed__)) { // this makes it a packed enum (1 byte)
      EMPTY = 0,
      FALLING,
      WOOD,
      FIRE,
      WATER,
      STEAM,
      ROCK,
    } CellType;

typedef struct {
  uint8_t type;
  bool flammable;
  int16_t
      tempreture; // using i16 keeps the struct small (floats add 4 bytes
                  // per cell at least). and it's enough for this simple app.

  uint16_t lifetime; // in frames (not all cells have a lifetime)
} Cell;

typedef struct {
  int falling;
  int wood;
  int fire;
  int water;
  int rock;
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
void insert_cell_at(MapState *state, int x, int y, CellType newType);
void remove_cell_at(MapState *state, int x, int y);
void handle_input(MapState *state);

// ---- DON'T TOUCH ----
#define CELL_SIZE (WINDOW_SIZE / MAP_SIZE)

#endif
