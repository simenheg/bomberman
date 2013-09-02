#include "script.h"

static void *register_functions(void *data)
{
  scm_c_define_gsubr("c-turn-into", 3, 0, 0, &scm_turn_into);
  scm_c_define_gsubr("kill-player", 1, 0, 0, &scm_kill_player);
  scm_c_define_gsubr("teleport-player", 3, 0, 0, &scm_teleport_player);
  scm_c_define_gsubr("move-player", 3, 0, 0, &scm_move_player);
  scm_c_define_gsubr("inc-player-firepower", 2, 0, 0,
                     &scm_inc_player_firepower);
  scm_c_define_gsubr("inc-player-bombs", 2, 0, 0, &scm_inc_player_bombs);
  scm_c_define_gsubr("inc-player-speed", 2, 0, 0, &scm_inc_player_speed);
  scm_c_define_gsubr("modify-player-movement", 2, 0, 0,
                     &scm_modify_player_movement);
  return NULL;
}

int init_script(void)
{
  scm_with_guile(&register_functions, NULL);
  scm_c_primitive_load("data/script/main.scm");
  animation = scm_variable_ref(scm_c_lookup("animation?"));
  animation_frames = scm_variable_ref(scm_c_lookup("animation-frames"));
  return 0;
}
