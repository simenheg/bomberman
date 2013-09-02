#ifndef MAP_H
#define MAP_H

#include "common.h"
#include "gfx.h"
#include "script.h"

struct block
{
  int frames;
  SDL_Surface **sprites;
  char walkable;
  char flame_blocking;
  SCM flame_function;
  SCM trigger_function;
};

struct block *blocks;
int *block_map;

char *hit_map;

int tile_x, tile_y;
int p1_start_x, p1_start_y, p2_start_x, p2_start_y;

int init_map(const char *file_path);
void init_blocks(void);
SCM load_scm_map(const char *file_path);
void parse_map(SCM map, int rows, int columns);
void put_map(int x, int y, int block);

int map_index(int x, int y);
int residential(float pos);
int map_residential_index(float x, float y);

SCM scm_turn_into(SCM type, SCM x, SCM y);

#endif
