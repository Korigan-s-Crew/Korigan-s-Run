#include <SDL2/SDL.h>


#ifndef CGAME_MAP_H
#define CGAME_MAP_H

#define MAX_TILES 150

typedef struct Collision Collision;
struct Collision {
    SDL_bool up;
    SDL_bool down;
    SDL_bool left;
    SDL_bool right;
    SDL_bool traversableUp;
    SDL_bool traversableDown;
    SDL_bool traversableLeft;
    SDL_bool traversableRight;
};

typedef struct Tile Tile;
struct Tile {
    int x;
    int y;
    int width;
    int height;
    int type;
    SDL_Texture *texture;
    Collision collision;
    int rotation;

};

typedef struct Map Map;
struct Map {
    int width;
    int height;
    Tile tiles[MAX_TILES][MAX_TILES];
    int foreground[MAX_TILES][MAX_TILES];
    SDL_bool full;
    int tile_start_x;
    int tile_start_y;
};

#endif //CGAME_MAP_H
