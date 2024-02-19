#include <SDL2/SDL.h>

#ifndef EXAMPLE_2D_H
#define EXAMPLE_2D_H
#define SCREEN_WIDTH 1300
#define SCREEN_HEIGHT 720
#define MAX_TILES 128
#define max(a, b) (a > b ? a : b)


typedef struct Map Map;
struct Map {
    int width;
    int height;
    int tiles[MAX_TILES][MAX_TILES];
};

typedef struct Character Character;
struct Character {
    int x;
    int y;
    int width;
    int height;
    int speed;
    SDL_Texture *image;
};

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);
Map *create_map(char* path);
void draw_map(SDL_Renderer *renderer, SDL_Texture *list_images[10], Map *map);
void print_map(Map *map);

#endif
