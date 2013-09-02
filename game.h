#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "common.h"
#include "video.h"
#include "gfx.h"
#include "script.h"
#include "audio.h"
#include "input.h"

#define BOMB_TIME 2000 /* In milis */

struct element
{
  ETYPE type;
  Uint32 time;
  int x, y;
  int owner;
  char dir;
};

int max_bombs;
int max_fires;

struct element *bombs;
int active_bombs;
char *bomb_map;

struct element *fires;
int active_fires;
char *fire_map;

int game_loop(void);
void init_world(void);
void update_world(Uint32 dt);

void add_element(int x, int y, char dir, int owner, ETYPE type, Uint32 time,
                 struct element *elements, int *active_elements,
                 int MAX_ELEMENTS, char *map);
void update_elements(Uint32 dt, struct element *elements,
                     int *active_elements, char *map);
void remove_element(int index, struct element *elements,
                    int *active_elements, char *map);
int remove_element_pos(int x, int y, struct element *elements,
                       int *active_elements, char *map);
void explode_bomb(int x, int y, int owner);
void spawn_fire(int x, int y, char dir, int grow_x, int grow_y);

#endif
