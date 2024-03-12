#include "controls.h"

int switchLayout(Controls *controls)
{
    if (strcmp(controls->layout, "azerty") == 0)
    {
        controls->layout = "qwerty";
        controls->left = SDLK_a;
        controls->right = SDLK_d;
        controls->up = SDLK_w;
        controls->down = SDLK_s;
        return 0;
    }
    else if (strcmp(controls->layout, "qwerty") == 0)
    {
        controls->layout = "azerty";
        controls->left = SDLK_q;
        controls->right = SDLK_d;
        controls->up = SDLK_z;
        controls->down = SDLK_s;
        return 0;
    }
    else
    {
        return -1;
    }
}

Controls *init_controls(void) {
    Controls *controls = malloc(sizeof(Controls));
    if (!controls) {
        fprintf(stderr, "Failed to allocate memory for controls\n");
        return NULL;
    }

    controls->layout = "azerty";
    controls->left = SDLK_q;
    controls->right = SDLK_d;
    controls->up = SDLK_z;
    controls->down = SDLK_s;

    return controls;
}
