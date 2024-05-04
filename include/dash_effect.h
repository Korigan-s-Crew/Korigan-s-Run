//
// Created by junper on 5/4/24.
//

#ifndef CGAME_DASH_EFFECT_H
#define CGAME_DASH_EFFECT_H

#include <math.h>
#include <SDL2/SDL.h>
#include "main.h"

typedef Character Character;
typedef Texture Texture;

void apply_box_blur(unsigned char* data, int width, int height, int channels, int radius);
void apply_ghostly_effect(const char* input_file, const char* output_file_prefix, int num_frames, int blur_radius);
void apply_dash_effect(SDL_Renderer* renderer, Character* character, Texture* textures, int num_frames, float dash_duration, float fade_duration);

#endif //CGAME_DASH_EFFECT_H
