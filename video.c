#include "video.h"

int init_video(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
      fprintf(stderr, "SDL video initialization error: %s\n",
              SDL_GetError());
      return -1;
    }

  screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP, SDL_DOUBLEBUF);
  if (screen == NULL)
    {
      fprintf(stderr, "Video mode error: %s\n", SDL_GetError());
      return -1;
    }

  return 0;
}

void display(Uint32 time)
{
  push_background(0);
  push_tiles(1, time);
  push_bombs(2);
  push_fires(3);
  push_players(4);
  flush_sprite_queue();

  SDL_Flip(screen);
  return;
}

void push_background(int layer)
{
  /* for (int i = 0; i < tile_y; ++i) */
  /*   for (int j = 0; j < tile_x; ++j) */
  /*     add_to_sprite_queue(tile_grass, layer, j * TILE_SIZE, */
  /*                         i * TILE_SIZE); */
}

void push_tiles(int layer, Uint32 time)
{
  for (int i = 0; i < tile_y; ++i)
    for (int j = 0; j < tile_x; ++j)
      {
        struct block *cur = &blocks[block_map[i * tile_x + j]];
        add_to_sprite_queue(cur->sprites[(time / 17) % cur->frames],
                            layer, j * TILE_SIZE, i * TILE_SIZE);
      }
}

void push_bombs(int layer)
{
  for (int i = 0; i < active_bombs; ++i)
    {
      struct element *e = &bombs[i];
      add_to_sprite_queue(tile_bomb, layer, e->x * TILE_SIZE,
                          e->y * TILE_SIZE);
    }
}

void push_fires(int layer)
{
  for (int i = 0; i < active_fires; ++i)
    {
      struct element *e = &fires[i];
      SDL_Surface *tile_flame = get_proper_flame_tile(e->x, e->y, e->dir);
      add_to_sprite_queue(tile_flame, layer, e->x * TILE_SIZE,
                          e->y * TILE_SIZE);
    }
}

void push_players(int layer)
{
  for (int i = 0; i < n_players; ++i)
    {
      struct player *p = &players[i];
      add_to_sprite_queue(p->sprite, layer,
                          (int)p->x - TILE_SIZE / 2,
                          (int)p->y - p->sprite->h + TILE_SIZE / 3);
    }
}

void flush_sprite_queue(void)
{
  SDL_Rect dest;

  SDL_FillRect(screen, &screen->clip_rect,
               SDL_MapRGB(screen->format, 0, 0, 0));

  for (struct sprite *cur = sprite_queue; cur != NULL; cur = cur->next)
    {
      dest.x = cur->x + (SCREEN_W - tile_x * TILE_SIZE) / 2;
      dest.y = cur->y + (SCREEN_H - tile_y * TILE_SIZE) / 2;
      SDL_BlitSurface(cur->sprite, NULL, screen, &dest);
    }

  clear_sprite_queue();
}

void add_to_sprite_queue(SDL_Surface *sprite, int layer, int x, int y)
{
  int compare_sprite(struct sprite *s1, struct sprite *s2)
  {
    if (s1->layer < s2->layer)
      return 1;
    else if (s1->layer > s2->layer)
      return -1;
    else if (s1->y > s2->y)
      return -1;
    else if (s1->y < s2->y)
      return 1;
    return 0;
  }

  struct sprite *new = calloc(1, sizeof(struct sprite));
  new->sprite = sprite;
  new->layer = layer;
  new->x = x;
  new->y = y;

  if (sprite_queue == NULL)
    {
      sprite_queue = new;
      return;
    }

  if (compare_sprite(sprite_queue, new) < 0)
    {
      new->next = sprite_queue;
      sprite_queue = new;
      return;
    }

  struct sprite *cur;
  for (cur = sprite_queue; cur->next != NULL; cur = cur->next)
    {
      if (compare_sprite(cur->next, new) < 0)
        {
          new->next = cur->next;
          cur->next = new;
          return;
        }
    }

  cur->next = new;
}

void clear_sprite_queue(void)
{
  struct sprite *cur = sprite_queue;

  while (cur != NULL)
    {
      struct sprite *tmp = cur->next;
      free(cur);
      cur = tmp;
    }

  sprite_queue = NULL;
}

SDL_Surface *get_proper_flame_tile(int x, int y, char dir)
{
  int flame_index = 0;

  if (y > 0 && fire_map[(y - 1) * tile_x + x])
    flame_index |= 8;
  if (y < tile_y - 1 && fire_map[(y + 1) * tile_x + x])
    flame_index |= 4;
  if (x > 0 && fire_map[y * tile_x + x - 1])
    flame_index |= 2;
  if (x < tile_x - 1 && fire_map[y * tile_x + x + 1])
    flame_index |= 1;

  return tiles_flame[flame_index];
}
