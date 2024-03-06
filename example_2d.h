#include <SDL2/SDL.h>

#ifndef EXAMPLE_2D_H
#define EXAMPLE_2D_H
#define SCREEN_WIDTH 1300
#define SCREEN_HEIGHT 720
#define MAX_TILES 128
#define MAX_FPS 60
#define max(a, b) (a > b ? a : b)

typedef struct Map Map;
struct Map
{
    int width;
    int height;
    int tiles[MAX_TILES][MAX_TILES];
};
typedef struct Character Character;
struct Character
{
    int x;
    int y;
    int dx;
    int dy;
    int width;
    int height;
    float speed;
    SDL_bool alive;
    SDL_bool on_ground;
    SDL_bool right;
    SDL_bool left;
    SDL_bool up;
    SDL_bool down;
    SDL_bool dash;
    SDL_Texture *images[10];
};
struct camera
{
    int x;
    int y;
    int width;
    int height;
    int fps;
};
typedef struct camera camera;

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);
Map *create_map(char *path);
void draw_map(SDL_Renderer *renderer, SDL_Texture *list_images[100], Map *map, int tile_width, int tile_height, camera *camera);
void print_map(Map *map);
Character *create_character(char *path, int x, int y, int width, int height, int speed, SDL_Renderer *renderer, SDL_bool onground);
char *addcat(char *result, char *path, char *name);
void free_character(Character *character);
void draw_character(SDL_Renderer *renderer, Character *character, camera *camera);
void draw_character_animation(SDL_Renderer *renderer, Character *character, SDL_Rect *dst, camera *camera, int index, float speed, int nb_frame);
void draw_character_animationEx(SDL_Renderer *renderer, Character *character, SDL_Rect *dst, camera *camera, int index, int SDL_angle, float speed, int nb_frame);
void move_character(Character *character, int x, int y, Map *map, int tile_width, int tile_height);
void move_character_up(Character *character, int tile_height);
void move_character_down(Character *character, int tile_height);
void move_character_left(Character *character, int tile_width);
void move_character_right(Character *character, int tile_width);
void draw(SDL_Renderer *renderer, SDL_Color bleu, SDL_Texture *list_images[100], Map *map, int tile_width, int tile_height, Character *character, camera *camera);
void mouvement(Map *map, Character *character, int tile_width, int tile_height);
void gravity(Character *character);
void collision(Character *character, Map *map, int tile_width, int tile_height);
void create_camera(camera *camera, int x, int y, int width, int height);
void move_camera(camera *camera, Character *character, Map *map);
#endif
