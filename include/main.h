#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
#define NUMBER_MAPS 3


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
    SDL_Texture *best_time[4];
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
#endif
