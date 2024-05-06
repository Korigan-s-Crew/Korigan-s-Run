#ifndef CGAME_ANIMATION_H
#define CGAME_ANIMATION_H

#include "main.h"

#include <stdbool.h>
#include <stdio.h>

#define MAX_CHARACTERS 50


// Structure to hold information about a character animation
typedef struct {
    Character *character;
    SDL_Texture *texture[100];
    SDL_Rect dst;
    int nb_frame;
    Uint32 start_time;
    Uint32 duration;
    int camera_x;
    int camera_y;
    double angle;
    SDL_Point *center;
    SDL_RendererFlip flip;
    bool finished;
} CharacterAnimation;


void init_character_animations_buffer();
int add_character_animation(Character *character, SDL_Texture *texture[], Camera *camera, SDL_Rect dst, int nb_frame, int duration, double angle, SDL_Point *center, SDL_RendererFlip flip);
void update_character_animations();
void render_character_animations(SDL_Renderer *renderer, Camera *camera);
int print_nb_animation(int prev_nb);


#endif //CGAME_ANIMATION_H
