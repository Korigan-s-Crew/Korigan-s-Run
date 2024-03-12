//
// Created by junper on 3/12/24.
//

#ifndef CGAME_CONTROLS_H
#define CGAME_CONTROLS_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Controls {
    const char *layout;
    SDL_KeyCode left;
    SDL_KeyCode right;
    SDL_KeyCode up;
    SDL_KeyCode down;
};
typedef struct Controls Controls;

int switchLayout(Controls *controls);
Controls *init_controls(void);

#endif //CGAME_CONTROLS_H
