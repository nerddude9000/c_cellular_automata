#include "constants.h"

#ifndef FALLING_SANDS_H
#define FALLING_SANDS_H

Cell *get_cell(Cell *map, int x, int y);
void init_map(Cell *map);
void draw_map(Cell *map);
void sim_map(Cell *map);
void write_map_to_file(Cell *map);

#endif
