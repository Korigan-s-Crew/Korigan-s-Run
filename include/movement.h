//
// Created by junper on 3/13/24.
//

#include "main.h"
#include "controls.h"

#ifndef CGAME_MOUVEMENT_H
#define CGAME_MOUVEMENT_H

struct Point {
    int x;
    int y;
};
typedef struct Point Point;

struct Dash {
    int duration;
    int cooldown;
    Point direction;
    SDL_bool on_air;
    SDL_bool go_up;
};
typedef struct Dash Dash;

struct Slide {
    int duration;
    SDL_bool go_left;
    SDL_bool go_right;
};
typedef struct Slide Slide;


Dash *init_dash();

void action_dash(Character *character, Controls *controls);

void handle_dash(Character *character, Map *map);

void print_dash(Dash *dash);


Slide *init_slide();

void action_slide(Character *character);

void handle_slide(Character *character, Map *map);

void slide_cancel(Character *character);


void move_character(Character *character, int x, int y, Map *map);

void move_character_up(Character *character, int tile_width, int tile_height);

void move_character_down(Character *character, int tile_height);

void move_character_left(Character *character, int tile_width);

void move_character_right(Character *character, int tile_width);

void mouvement(Map *map, Character *character);


void gravity(Character *character);

void slow_down(Character *character);

#endif //CGAME_MOUVEMENT_H
