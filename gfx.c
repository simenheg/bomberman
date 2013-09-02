#include "gfx.h"

int init_gfx(void)
{
  tile_bomb = load_image("data/gfx/tile_bomb.png", 1);
  tile_character = load_image("data/gfx/tile_character.png", 1);
  tile_character_m = load_image("data/gfx/tile_character_mirror.png", 1);

  tiles_flame[0] = load_image("data/gfx/flames/0000.png", 1);
  tiles_flame[1] = load_image("data/gfx/flames/000f.png", 1);
  tiles_flame[2] = load_image("data/gfx/flames/00f0.png", 1);
  tiles_flame[3] = load_image("data/gfx/flames/00ff.png", 1);
  tiles_flame[4] = load_image("data/gfx/flames/0f00.png", 1);
  tiles_flame[5] = load_image("data/gfx/flames/0f0f.png", 1);
  tiles_flame[6] = load_image("data/gfx/flames/0ff0.png", 1);
  tiles_flame[7] = load_image("data/gfx/flames/0fff.png", 1);
  tiles_flame[8] = load_image("data/gfx/flames/f000.png", 1);
  tiles_flame[9] = load_image("data/gfx/flames/f00f.png", 1);
  tiles_flame[10] = load_image("data/gfx/flames/f0f0.png", 1);
  tiles_flame[11] = load_image("data/gfx/flames/f0ff.png", 1);
  tiles_flame[12] = load_image("data/gfx/flames/ff00.png", 1);
  tiles_flame[13] = load_image("data/gfx/flames/ff0f.png", 1);
  tiles_flame[14] = load_image("data/gfx/flames/fff0.png", 1);
  tiles_flame[15] = load_image("data/gfx/flames/ffff.png", 1);

  powerup_fire = load_image("data/gfx/powerup_fire.png", 0);
  return 0;
}

SDL_Surface *load_image(const char *filename, int alpha)
{
  SDL_Surface *image = IMG_Load(filename);

  if (image == NULL)
    {
      fprintf(stderr, "Failed to load image %s\n", filename);
      exit(1);
    }

  if (alpha)
    image = SDL_DisplayFormatAlpha(image);
  else
    image = SDL_DisplayFormat(image);

  return image;
}
