#ifndef DRAW_H
#define DRAW_H
#include "controls.h"
#include "map.h"
#include "init.h"
#include "movement.h"
#include "procedural_generation.h"
#include "dash_effect.h"
#include "animation.h"
#include "music.h"
#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


void draw_character(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera);
void draw_character_offset(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera, SDL_Rect dst, int offset);
void draw_character_animation(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera, int index, float speed, int nb_frame);
void draw_dash_trail(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera, int nb_frame);
void draw_dash_trail(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera,
                     int nb_frame);
void draw_character_animationEx(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera, int index, int SDL_angle, float speed, int nb_frame);
void draw_indication(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera, SDL_Rect dst_key);
void load_best_time(SDL_Renderer *renderer, Texture *texture, Character *character, double best_time);
void draw_fps(SDL_Renderer *renderer, Camera *camera, Texture *texture);
void draw_ingame(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Map *map, int tile_width, int tile_height, Character *character, Camera *camera);
void draw_background(SDL_Renderer *renderer, Texture *texture, Camera *camera, Map *map);
void draw_homepage(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Camera *camera, Mouse *mouse);
void draw_endpage(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Camera *camera, Mouse *mouse, Character *character, double best_time);
void create_camera(Camera *camera, int x, int y, int width, int height);
void move_camera(Camera *camera, Character *character, Map *map);
#endif
