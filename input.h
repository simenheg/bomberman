#ifndef INPUT_H
#define INPUT_H

#include "common.h"

SDL_Event event;
unsigned int key_down[322]; /* Number of possible SDL keys */

void handle_input(void);

#endif
