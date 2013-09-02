#include "game.h"

int game_loop(void)
{
  Uint32 t = SDL_GetTicks();

  init_world();
  init_players(n_players);

  while (1)
    {
      Uint32 tmp = SDL_GetTicks();
      Uint32 dt = tmp - t;
      t = tmp;

      handle_input();
      update_world(dt);
      display(tmp);
    }

  return 0;
}

void init_world(void)
{
  active_bombs = 0;
  active_fires = 0;

  if (init_map("data/script/maps/test-map.scm") != 0)
    exit(1);

  max_bombs = tile_x * tile_y;
  bombs = calloc(max_bombs, sizeof(struct element));
  bomb_map = calloc(tile_x * tile_y, sizeof(char));
  max_fires = tile_x * tile_y * 3;
  fires = calloc(max_fires, sizeof(struct element));
  fire_map = calloc(tile_x * tile_y, sizeof(char));
}

void update_world(Uint32 dt)
{
  update_elements(dt, fires, &active_fires, fire_map);
  update_elements(dt, bombs, &active_bombs, bomb_map);
  update_players(dt);
}

void add_element(int x, int y, char dir, int owner, ETYPE type, Uint32 time,
                 struct element *elements, int *active_elements,
                 int MAX_ELEMENTS, char *map)
{
  if (*active_elements == MAX_ELEMENTS)
    return;

  elements[*active_elements].type = type;
  elements[*active_elements].time = time;
  elements[*active_elements].owner = owner;
  elements[*active_elements].x = x;
  elements[*active_elements].y = y;
  elements[*active_elements].dir = dir;
  ++map[y * tile_x + x];
  ++(*active_elements);
}

void update_elements(Uint32 dt, struct element *elements,
                     int *active_elements, char *map)
{
  for (int i = 0; i < *active_elements; ++i)
    {
      struct element *e = &elements[i];
      if (e->time < dt) {
        if (e->type == BOMB)
          explode_bomb(e->x, e->y, e->owner);
        else
          remove_element(i, elements, active_elements, map);
      } else
        e->time -= dt;
    }
}

void remove_element(int index, struct element *elements, int *active_elements,
                    char *map)
{
  struct element *e = &elements[index];
  --map[e->y * tile_x + e->x];

  if (e->type == BOMB)
    --(players[e->owner]).my_bombs;

  --(*active_elements);
  elements[index] = elements[*active_elements];
}

int remove_element_pos(int x, int y, struct element *elements,
                       int *active_elements, char *map)
{
  struct element *e = elements;

  for (int i = 0; i < *active_elements; ++i, ++e)
    {
      if (x == e->x && y == e->y)
        {
          --map[y * tile_x + x];

          if (e->type == BOMB)
            --(players[e->owner]).my_bombs;

          --(*active_elements);
          elements[i] = elements[*active_elements];
          return e->owner;
        }
    }

  return -1;
}

void explode_bomb(int x, int y, int owner)
{
  remove_element_pos(x, y, bombs, &active_bombs, bomb_map);

  play_sound(x, y, SOUND_EXPLOSION);

  if (owner == -1)
    {
      fprintf(stderr, "Tried to detonate a bomb without an owner\n");
      return;
    }

  int fire_power = (&players[owner])->fire_power;

  spawn_fire(x, y, 'r', fire_power, 0);
  spawn_fire(x, y, 'l', -fire_power, 0);
  spawn_fire(x, y, 'd', 0, fire_power);
  spawn_fire(x, y, 'u', 0, -fire_power);
}

void spawn_fire(int x, int y, char dir, int grow_x, int grow_y)
{
  if (x < 0 || x >= tile_x ||
      y < 0 || y >= tile_y)
    return;

  int flame_blocking = blocks[block_map[y * tile_x + x]].flame_blocking;

  scm_call_2(blocks[block_map[map_index(x, y)]].flame_function,
             scm_from_int(x),
             scm_from_int(y));

  if (flame_blocking)
    return;

  add_element(x, y, dir, 0, FIRE, 700, fires, &active_fires, max_fires,
              fire_map);

  if (bomb_map[y * tile_x + x])
    {
      int owner = remove_element_pos(x, y, bombs, &active_bombs, bomb_map);
      explode_bomb(x, y, owner);
    }

  if (grow_x > 0)
    spawn_fire(x + 1, y, dir, grow_x - 1, grow_y);
  else if (grow_x < 0)
    spawn_fire(x - 1, y, dir, grow_x + 1, grow_y);

  if (grow_y > 0)
    spawn_fire(x, y + 1, dir, grow_x, grow_y - 1);
  else if (grow_y < 0)
    spawn_fire(x, y - 1, dir, grow_x, grow_y + 1);
}
