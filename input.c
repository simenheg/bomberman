#include "input.h"

void handle_input(void)
{
  while (SDL_PollEvent(&event))
    {
      switch (event.type)
        {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym)
            {
            case 'q':
            case SDLK_ESCAPE:
              exit(0);
              break;
            default:
              key_down[event.key.keysym.sym] = 1;
              break;
            }
          break;
        case SDL_KEYUP:
          key_down[event.key.keysym.sym] = 0;
          break;
        default:
          break;
        }
    }
}
