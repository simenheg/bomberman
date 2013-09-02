#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "game.h"
#include "script.h"
#include "gfx.h"
#include "input.h"

static const int starter_firepower = 1;
static const int starter_bombs     = 1;
static const float starter_speed   = 0.13;

struct player
{
  SDL_Surface *sprite;
  float x, y;
  float vx, vy;
  int my_bombs;
  int max_bombs;
  int fire_power;
  float speed;

  struct keymap
  {
    SDLKey left, right, up, down, drop;
  } keymap;
};

struct player *players;
static unsigned int n_players = 2;

void init_players(int n_players);
void update_players(Uint32 dt);
void move_player(int player, Uint32 dt);
void drop_bomb(int player);
void kill_player(int player);

SCM scm_kill_player(SCM player);
SCM scm_teleport_player(SCM player, SCM x, SCM y);
SCM scm_move_player(SCM player, SCM x, SCM y);
SCM scm_inc_player_firepower(SCM player, SCM amount);
SCM scm_inc_player_bombs(SCM player, SCM amount);
SCM scm_inc_player_speed(SCM player, SCM amount);
SCM scm_modify_player_movement(SCM player, SCM amount);

#endif
