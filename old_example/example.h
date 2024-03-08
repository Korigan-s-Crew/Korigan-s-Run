#include <SDL2/SDL.h>

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);
int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h);
int setWindowColor(SDL_Renderer *renderer, SDL_Color color);
