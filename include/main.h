#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#ifndef EXAMPLE_2D_H
#define EXAMPLE_2D_H
#define MAX_TILES 150
#define MAX_FPS 120
#define max(a, b) (a > b ? a : b)

typedef struct Map Map;
struct Map {
    int width;
    int height;
    int tiles[MAX_TILES][MAX_TILES];
    int foreground[MAX_TILES][MAX_TILES];
    SDL_bool full;
    int tile_start_x;
    int tile_start_y;
};
typedef struct Character Character;
struct Character {
    int x;
    int y;
    int dx;
    int dy;
    int width;
    int height;
    int original_width;
    int original_height;
    float speed;
    SDL_bool alive;
    SDL_bool on_ground;
    SDL_bool right;
    SDL_bool left;
    SDL_bool up;
    SDL_bool down;
    int dash;
    SDL_Texture *images[100];
};
struct Camera {
    int x;
    int y;
    int width;
    int height;
    int fps;
    int avg_fps;
    SDL_bool show_fps;
    int pattern_generated_history[100];
};
typedef struct Camera Camera;
struct Texture {
    SDL_Texture *collision[100];
    SDL_Texture *transparent[100];
    SDL_Texture *main_character[100];
    TTF_Font *font;
};
typedef struct Texture Texture;
long long getCurrentTimeInMicroseconds();
Texture *create_texture(SDL_Renderer *renderer);
void free_texture(Texture *texture);
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);
Map *create_map(char *path);
void draw_map(SDL_Renderer *renderer, Texture *texture, Map *map, int tile_width, int tile_height, Camera *camera);
void print_map(Map *map);
Character *create_character(int x, int y, int width, int height, int speed, SDL_Renderer *renderer);
char *addcat(char *result, char *path, char *name);
void draw_character(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera);
void draw_character_animation(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera, int index, float speed, int nb_frame);
void draw_character_animationEx(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera, int index, int SDL_angle, float speed, int nb_frame);
void draw_fps(SDL_Renderer *renderer, Camera *camera, Texture *texture);
void draw(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Map *map, int tile_width, int tile_height, Character *character, Camera *camera);
void collision(Character *character, Map *map, int tile_width, int tile_height);
void create_camera(Camera *camera, int x, int y, int width, int height);
void move_camera(Camera *camera, Character *character, Map *map);
void add_right_pattern_to_map(Map *pattern, Map *map);
#endif
