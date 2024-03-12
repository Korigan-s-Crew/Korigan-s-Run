//
// Created by junper on 3/12/24.
//

#ifndef CGAME_CONTROLS_H
#define CGAME_CONTROLS_H

#include "SDL.h"

#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Controls {
    const char *layout;
    SDL_KeyCode left;
    SDL_KeyCode right;
    SDL_KeyCode up;
    SDL_KeyCode down;
}Controls;

int switchLayout(Controls *controls);
Controls *init_controls(void);

#endif //CGAME_CONTROLS_H
