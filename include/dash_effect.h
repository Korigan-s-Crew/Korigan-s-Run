//
// Created by junper on 5/4/24.
//

#ifndef CGAME_DASH_EFFECT_H
#define CGAME_DASH_EFFECT_H

#include <math.h>
#include <SDL2/SDL.h>
#include "main.h"
#include "movement.h"

typedef Character Character;
typedef Texture Texture;
typedef Camera Camera;
typedef Dash Dash;

void apply_box_blur(unsigned char* data, int width, int height, int channels, int radius);
void apply_ghostly_effect(const char* input_file, const char* output_file_prefix, int num_frames, int blur_radius);
void dash_display(Character *character, Texture *texture, SDL_Renderer *renderer, Camera *camera, SDL_Rect dst);

#endif //CGAME_DASH_EFFECT_H
