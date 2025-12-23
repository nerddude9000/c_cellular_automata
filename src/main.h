#ifndef FALLING_SANDS_H
#define FALLING_SANDS_H

#include "constants.h"
#include <raylib.h>

typedef enum { EMPTY = 0, SOLID, FALLING } CellType;

typedef struct {
  uint8_t type;
} Cell;

Cell new_cell(void);
Cell *get_cell(Cell map[], int x, int y);
void init_map(Cell map[]);
void draw_map(Cell map[]);
void sim_map(Cell map[]);
void write_map_to_file(Cell map[]);
void insert_cell_at(Cell map[], int x, int y, CellType cType);
void remove_cell_at(Cell map[], int x, int y);
void handle_input(Cell map[], bool *isPaused);

#endif
