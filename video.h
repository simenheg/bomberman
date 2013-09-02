#ifndef VIDEO_H
#define VIDEO_H

#include <SDL/SDL_image.h>

#include "common.h"
#include "gfx.h"
#include "player.h"
#include "game.h"

#define SCREEN_BPP 32
#define TILE_SIZE  48

#define SCREEN_W 800
#define SCREEN_H 600

struct sprite
{
  SDL_Surface *sprite;
  int x, y, layer;
  struct sprite *next;
};

SDL_Surface *screen;
struct sprite *sprite_queue;

int init_video(void);
void display(Uint32 time);
void push_background(int layer);
void push_tiles(int layer, Uint32 time);
void push_bombs(int layer);
void push_fires(int layer);
void push_players(int layer);
void flush_sprite_queue(void);
void add_to_sprite_queue(SDL_Surface *image, int layer, int x, int y);
void clear_sprite_queue(void);
SDL_Surface *get_proper_flame_tile(int x, int y, char dir);

#endif
