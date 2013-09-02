#include "map.h"

int init_map(const char *file_path)
{
  SCM map = load_scm_map(file_path);
  hit_map = calloc(tile_x * tile_y, sizeof(char));
  block_map = calloc(tile_x * tile_y, sizeof(int));
  init_blocks();

  parse_map(map, tile_y, tile_x);

  return 0;
}

void init_blocks(void)
{
  SCM block_list = scm_variable_ref(scm_c_lookup("blocks"));
  int block_list_len = scm_to_int(scm_length(block_list));
  blocks = calloc(block_list_len, sizeof(struct block));

  SCM block_sprite_path = scm_variable_ref(scm_c_lookup("block-sprite"));
  SCM block_flame_blocking =
    scm_variable_ref(scm_c_lookup("block-flame-blocking?"));
  SCM block_flame_functions =
    scm_variable_ref(scm_c_lookup("block-flame-function"));
  SCM block_walkable = scm_variable_ref(scm_c_lookup("block-walkable?"));
  SCM block_trigger_functions =
    scm_variable_ref(scm_c_lookup("block-trigger-function"));

  for (int i = 0; SCM_FALSEP(scm_null_p(block_list)); ++i)
    {
      SCM cur_block = SCM_CAR(block_list);
      SCM sprite_path = scm_call_1(block_sprite_path, cur_block);
      SCM flame_blocking = scm_call_1(block_flame_blocking, cur_block);
      SCM walkable = scm_call_1(block_walkable, cur_block);
      SCM flame_function = scm_call_1(block_flame_functions, cur_block);
      SCM trigger_function = scm_call_1(block_trigger_functions,
                                        cur_block);

      if (scm_call_1(animation, sprite_path) == SCM_BOOL_T)
        {
          SCM frames = scm_call_1(animation_frames, sprite_path);
          blocks[i].frames = scm_to_int(frames);
          blocks[i].sprites = calloc(blocks[i].frames,
                                     sizeof(SDL_Surface*));

          for (int j = 0; SCM_FALSEP(scm_null_p(sprite_path)); ++j)
            {
              SCM cur = SCM_CAR(sprite_path);
              blocks[i].sprites[j]
                = load_image(scm_to_locale_string(cur), 0);
              sprite_path = SCM_CDR(sprite_path);
            }

        } else {
        blocks[i].frames = 1;
        blocks[i].sprites = calloc(1, sizeof(SDL_Surface*));
        blocks[i].sprites[0] =
          load_image(scm_to_locale_string(sprite_path), 0);
      }

      blocks[i].flame_blocking = SCM_NFALSEP(flame_blocking);
      blocks[i].walkable = SCM_NFALSEP(walkable);
      blocks[i].flame_function = flame_function;
      blocks[i].trigger_function = trigger_function;

      block_list = SCM_CDR(block_list);
    }
}

SCM load_scm_map(const char *file_path)
{
  scm_c_primitive_load(file_path);

  SCM get_map = scm_variable_ref(scm_c_lookup("get-map"));

  tile_x = scm_to_int(scm_variable_ref(scm_c_lookup("cols")));
  tile_y = scm_to_int(scm_variable_ref(scm_c_lookup("rows")));

  SCM point_x = scm_variable_ref(scm_c_lookup("point-x"));
  SCM point_y = scm_variable_ref(scm_c_lookup("point-y"));

  SCM p1_start = scm_variable_ref(scm_c_lookup("p1-start"));
  p1_start_x = scm_to_int(scm_call_1(point_x, p1_start));
  p1_start_y = scm_to_int(scm_call_1(point_y, p1_start));
  SCM p2_start = scm_variable_ref(scm_c_lookup("p2-start"));
  p2_start_x = scm_to_int(scm_call_1(point_x, p2_start));
  p2_start_y = scm_to_int(scm_call_1(point_y, p2_start));

  return scm_call_0(get_map);
}

void parse_map(SCM map, int rows, int columns)
{
  int row = 0;
  int col = 0;

  while (SCM_FALSEP(scm_null_p(map)))
    {
      SCM cur_block = SCM_CAR(map);
      map = SCM_CDR(map);
      put_map(col, row, scm_to_int(cur_block));
      col = col == columns - 1 ? 0 : col + 1;
      row = col == 0 ? row + 1 : row;
    }
}

void put_map(int x, int y, int block)
{
  block_map[y * tile_x + x] = block;
  if (!(blocks[block].walkable))
    hit_map[y * tile_x + x] = 1;
}

int map_index(int x, int y)
{
  return y * tile_x + x;
}

int residential(float pos)
{
  return (int)pos / TILE_SIZE;
}

int map_residential_index(float x, float y)
{
  return residential(y) * tile_x + residential(x);
}

SCM scm_turn_into(SCM type, SCM x, SCM y)
{
  int c_x = scm_to_int(x);
  int c_y = scm_to_int(y);

  block_map[c_y * tile_x + c_x] = scm_to_int(type);
  hit_map[c_y * tile_x + c_x] =
    !blocks[block_map[c_y * tile_x + c_x]].walkable;
  return SCM_BOOL_T;
}
