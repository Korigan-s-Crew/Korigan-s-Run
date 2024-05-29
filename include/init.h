#ifndef INIT_H
#define INIT_H
#include "../include/controls.h"
#include "../include/map.h"
#include "../include/draw.h"
#include "../include/movement.h"
#include "../include/procedural_generation.h"
#include "../include/dash_effect.h"
#include "../include/animation.h"
#include "../include/music.h"
#include "../include/main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
long long getCurrentTimeInMicroseconds();
double load_settings(char *settings_path);
void save_time(char *settings_path, double time);
RdmTexture *load_from_dir(char *dir_path, SDL_Renderer *renderer);
Texture *create_texture(SDL_Renderer *renderer);
void free_texture(Texture *texture);
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);
void draw_map(SDL_Renderer *renderer, Texture *texture, Map *map, int tile_width, int tile_height, Camera *camera);
void print_map(Map *map);
Character *create_character(int x, int y, int width, int height, int speed, SDL_Renderer *renderer);
void reset_character(Character *character,Map *map, int tile_width, int tile_height);
void print_character(Character *character);
char *addcat(char *result, char *path, char *name);
#endif
