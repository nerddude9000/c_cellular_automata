#include "constants.h"
#include <raylib.h>

#ifndef FALLING_SANDS_H
#define FALLING_SANDS_H

Cell new_cell(void);
Cell *get_cell(Cell map[], int x, int y);
void init_map(Cell map[]);
void draw_map(Cell map[]);
void sim_map(Cell map[]);
void write_map_to_file(Cell map[]);
void insert_into_map(Cell map[], int x, int y, CellType cType);
void handle_input(Cell map[]);

Vector2 screen_to_map_pos(Vector2 screenPos);

#endif
