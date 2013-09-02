#include "main.h"

int main(void)
{
  if (init() != 0)
    return EXIT_FAILURE;

  if (game_loop() != 0)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int init(void)
{
  if (init_video() != 0)
    {
      fprintf(stderr, "Failed to initialize video\n");
      return -1;
    }

  if (init_script() != 0)
    {
      fprintf(stderr, "Failed to initialize scripting engine\n");
      return -1;
    }

  if (init_gfx() != 0)
    {
      fprintf(stderr, "Failed to initialize graphics\n");
      return -1;
    }

  if (init_audio() != 0)
    {
      fprintf(stderr, "Failed to initialize audio\n");
      return -1;
    }

  if (init_sdl() != 0)
    {
      fprintf(stderr, "Failed to initialize SDL\n");
      return -1;
    }

  return 0;
}

int init_sdl(void)
{
  if (atexit(SDL_Quit) != 0)
    return -1;

  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_SetCaption("Bomberman", "Bomberman");
  srand(SDL_GetTicks());

  return 0;
}
