#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#ifndef EXAMPLE_2D_H
#define EXAMPLE_2D_H
#define MAX_FPS 60
#define MAX_TILES 200
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
#define NUMBER_MAPS 1

typedef struct Slide Slide;
typedef struct Dash Dash;
typedef struct Map Map;
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
    SDL_bool just_landed;
    SDL_bool crouch;
    SDL_bool on_ground;
    SDL_bool wall_right;
    SDL_bool wall_left;
    SDL_bool wall_jump_right;
    SDL_bool wall_jump_left;
    SDL_bool right;
    SDL_bool left;
    SDL_bool up;
    SDL_bool down;
    SDL_bool on_portal;
    SDL_bool next_map;
    Dash *dash;
    Slide *slide;
    SDL_Texture *images[100];
    SDL_Keycode key_suggestion;
    SDL_Texture *text_suggestion;
    double timer;
    int num_map;
};
typedef struct Mouse Mouse;
struct Mouse {
    int x;
    int y;
    SDL_bool on_boutton;
    int num_boutton;
};

struct Camera {
    int x;
    int y;
    int width;
    int height;
    int fps;
    int avg_fps;
    SDL_bool show_fps;
    SDL_bool show_timer;
    int pattern_generated_history[100];
};
typedef struct Camera Camera;

struct RdmTexture {
    SDL_Texture *Data[10];
    int size;
};
typedef struct RdmTexture RdmTexture;

struct Paralax {
    SDL_Texture *image;
    int x;
};
typedef struct Paralax Paralax;

struct Texture {
    RdmTexture *collision[100];
    RdmTexture *transparent[100];
    SDL_Texture *main_character[100];
    SDL_Texture *key_suggestion[100];
    SDL_Texture *trail_frames[100];
    SDL_Texture *bouttons[100];
    SDL_Texture *timer[11];
    SDL_Texture *background[11];
    TTF_Font *font;
};
typedef struct Texture Texture;
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
void print_character(Character *character);
char *addcat(char *result, char *path, char *name);
void draw_character(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera);
void draw_character_offset(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera, SDL_Rect dst, int offset);
void draw_character_animation(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera, int index, float speed, int nb_frame);
void draw_dash_trail(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera, int nb_frame);
void draw_dash_trail(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera,
                     int nb_frame);
void draw_character_animationEx(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera, int index, int SDL_angle, float speed, int nb_frame);
void draw_indication(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera, SDL_Rect dst_key);
void draw_fps(SDL_Renderer *renderer, Camera *camera, Texture *texture);
void draw_ingame(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Map *map, int tile_width, int tile_height, Character *character, Camera *camera);
void draw_background(SDL_Renderer *renderer, Texture *texture, Camera *camera, Map *map);
void draw_homepage(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Camera *camera, Mouse *mouse);
void draw_endpage(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Camera *camera, Mouse *mouse, Character *character, double best_time);
void create_camera(Camera *camera, int x, int y, int width, int height);
void move_camera(Camera *camera, Character *character, Map *map);
#endif
