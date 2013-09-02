#ifndef GFX_H
#define GFX_H

#include <SDL/SDL_image.h>

#include "common.h"

SDL_Surface *tile_bomb;
SDL_Surface *tile_character;
SDL_Surface *tile_character_m;
SDL_Surface *tiles_flame[16];

SDL_Surface *powerup_fire;

int init_gfx(void);
SDL_Surface *load_image(const char *filename, int alpha);

#endif
