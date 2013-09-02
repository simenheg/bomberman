#include "player.h"

void init_players(int n_players)
{
  players = calloc(n_players, sizeof(struct player));
  for (int i = 0; i < n_players; ++i)
    {
      struct player *p = &players[i];
      p->sprite = tile_character;
      p->my_bombs = 0;
      p->fire_power = starter_firepower;
      p->max_bombs = starter_bombs;
      p->speed = starter_speed;

      if (i == 0)
        {
          p->x = p1_start_x * TILE_SIZE + TILE_SIZE / 2;
          p->y = p1_start_y * TILE_SIZE + TILE_SIZE / 2;
          p->keymap.left = SDLK_a;
          p->keymap.right = SDLK_d;
          p->keymap.up = SDLK_w;
          p->keymap.down = SDLK_s;
          p->keymap.drop = SDLK_SPACE;
        }

      if (i == 1)
        {
          p->x = p2_start_x * TILE_SIZE + TILE_SIZE / 2;
          p->y = p2_start_y * TILE_SIZE + TILE_SIZE / 2;
          p->keymap.left = SDLK_LEFT;
          p->keymap.right = SDLK_RIGHT;
          p->keymap.up = SDLK_UP;
          p->keymap.down = SDLK_DOWN;
          p->keymap.drop = SDLK_RCTRL;
        }
    }
}

void update_players(Uint32 dt)
{
  for (int i = 0; i < n_players; ++i)
    {
      struct player *p = &players[i];

      if (fire_map[map_residential_index(p->x, p->y)])
        kill_player(i);

      if (key_down[p->keymap.drop] && p->my_bombs < p->max_bombs)
        {
          drop_bomb(i);
          key_down[p->keymap.drop] = 0;
        }

      if (key_down[p->keymap.left] == 1)
        p->vx = -p->speed;
      else if (key_down[p->keymap.right] == 1)
        p->vx = p->speed;
      else
        p->vx = 0.0;

      if (key_down[p->keymap.up] == 1)
        p->vy = -p->speed;
      else if (key_down[p->keymap.down] == 1)
        p->vy = p->speed;
      else
        p->vy = 0.0;

      /* Compensate so that the total speed when player is moving diagonally
         doesn't exceed normal movement speed. */
      if (p->vx != 0 && p->vy != 0)
        {
          p->vx *= 0.707106;
          p->vy *= 0.707106;
        }

      if (p->vx > 0)
        p->sprite = tile_character;
      else if (p->vx < 0)
        p->sprite = tile_character_m;

      scm_call_3(blocks[block_map[map_residential_index(p->x, p->y)]]
                 .trigger_function,
                 scm_from_int(i),
                 scm_from_int(residential(p->x)),
                 scm_from_int(residential(p->y)));

      move_player(i, dt);
    }
}

void move_player(int player, Uint32 dt)
{
  struct player *p = &players[player];

  p->x += p->vx * dt;
  if (hit_map[map_residential_index(p->x, p->y)] ||
      hit_map[map_residential_index(p->x, p->y + TILE_SIZE / 3)] ||
      hit_map[map_residential_index(p->x + TILE_SIZE / 3, p->y)] ||
      hit_map[map_residential_index(p->x - TILE_SIZE / 3, p->y)] ||
      hit_map[map_residential_index(p->x - TILE_SIZE / 3,
                                    p->y + TILE_SIZE / 3)] ||
      hit_map[map_residential_index(p->x + TILE_SIZE / 3,
                                    p->y + TILE_SIZE / 3)])
    p->x -= p->vx * dt;

  p->y += p->vy * dt;
  if (hit_map[map_residential_index(p->x, p->y)] ||
      hit_map[map_residential_index(p->x, p->y + TILE_SIZE / 3)] ||
      hit_map[map_residential_index(p->x + TILE_SIZE / 3, p->y)] ||
      hit_map[map_residential_index(p->x - TILE_SIZE / 3, p->y)] ||
      hit_map[map_residential_index(p->x - TILE_SIZE / 3,
                                    p->y + TILE_SIZE / 3)] ||
      hit_map[map_residential_index(p->x + TILE_SIZE / 3,
                                    p->y + TILE_SIZE / 3)])
    p->y -= p->vy * dt;

  if (p->x - TILE_SIZE / 2 < 0)
    p->x = TILE_SIZE / 2;
  else if (p->x + TILE_SIZE / 2 > tile_x * TILE_SIZE)
    p->x = tile_x * TILE_SIZE - TILE_SIZE / 2;

  if (p->y - TILE_SIZE / 2 < 0)
    p->y = TILE_SIZE / 2;
  else if (p->y + TILE_SIZE / 2 > tile_y * TILE_SIZE)
    p->y = tile_y * TILE_SIZE - TILE_SIZE / 2;
}

void drop_bomb(int player)
{
  struct player *p = &players[player];
  int index = map_residential_index(p->x, p->y);

  if (bomb_map[index])
    return;

  ++(p->my_bombs);
  add_element(residential(p->x), residential(p->y), 0, player, BOMB,
              BOMB_TIME, bombs, &active_bombs, max_bombs, bomb_map);
}

void kill_player(int player)
{
  struct player *p = &players[player];
  p->x = 0.0;
  p->y = 0.0;
}

SCM scm_kill_player(SCM player)
{
  kill_player(scm_to_int(player));
  return SCM_BOOL_T;
}

SCM scm_teleport_player(SCM player, SCM x, SCM y)
{
  struct player *p = &players[scm_to_int(player)];
  p->x = scm_to_int(x) * TILE_SIZE + TILE_SIZE / 2;
  p->y = scm_to_int(y) * TILE_SIZE + TILE_SIZE / 2;
  return SCM_BOOL_T;
}

SCM scm_move_player(SCM player, SCM x, SCM y)
{
  struct player *p = &players[scm_to_int(player)];
  p->vx += scm_to_double(x);
  p->vy += scm_to_double(y);
  return SCM_BOOL_T;
}

SCM scm_inc_player_firepower(SCM player, SCM amount)
{
  struct player *p = &players[scm_to_int(player)];
  play_sound(residential(p->x), residential(p->y), SOUND_POWERUP);
  p->fire_power += scm_to_int(amount);
  return SCM_BOOL_T;
}

SCM scm_inc_player_bombs(SCM player, SCM amount)
{
  struct player *p = &players[scm_to_int(player)];
  play_sound(residential(p->x), residential(p->y), SOUND_POWERUP);
  p->max_bombs += scm_to_int(amount);
  return SCM_BOOL_T;
}

SCM scm_inc_player_speed(SCM player, SCM amount)
{
  struct player *p = &players[scm_to_int(player)];
  play_sound(residential(p->x), residential(p->y), SOUND_POWERUP);
  p->speed += scm_to_double(amount);
  return SCM_BOOL_T;
}

SCM scm_modify_player_movement(SCM player, SCM amount)
{
  struct player *p = &players[scm_to_int(player)];
  p->vx *= scm_to_double(amount);
  p->vy *= scm_to_double(amount);
  return SCM_BOOL_T;
}
