#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "example_2d.h"
#include <string.h>

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    // Crée la couleur blanche avec rgb et alpha
    SDL_Color blanc = {0, 102, 204, 0};

    // Initialise la fenêtre et le rendu
    if(0 != init(&window, &renderer, SCREEN_WIDTH, SCREEN_HEIGHT))
            goto Quit;
    // Crée une texture à partir d'une image BMP
    SDL_Texture *list_images[10];
    list_images[0] = loadImage("tile1.bmp", renderer);
    
    // Remplit la fenêtre de blanc
    setWindowColor(renderer, blanc);
    // Crée la ma
    Map *map = create_map("map.txt");
    print_map(map);
    // Character *character = create_character("character.bmp", 0, 0, 10, renderer);
    // move_character(character, 100, 100);

    draw_map(renderer, list_images, map);
    SDL_RenderPresent(renderer);
    free(map);
    statut = EXIT_SUCCESS;
    // Affiche le rendu
    // SDL_RenderPresent(renderer);
    // Attend 6 secondes
    SDL_Delay(6000);

    Quit:
    if(NULL != list_images[0])
        SDL_DestroyTexture(list_images[0]);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    // Crée la fenêtre et le rendu
    if(0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_RESIZABLE, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL; 
    SDL_Texture *texture = NULL;
    // Charge l'image dans une surface temporaire
    tmp = SDL_LoadBMP(path);
    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    // Crée une texture à partir de la surface temporaire
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    // Libère la surface temporaire
    SDL_FreeSurface(tmp);
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    // Retourne la texture
    return texture;
}


int setWindowColor(SDL_Renderer *renderer, SDL_Color color)
{
    // Change la couleur du pinceau utiliser (un peu comme sur paint tu change la couleur du pinceau avant de dessiner)
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    // Remplit toute la fenêtre de la couleur du pinceau
    if(SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;  
}

Map *create_map(char* path)
{
    FILE *file = fopen(path, "r");
    char ch;
    int width = 0;
    int height = 0;
    int max_width = 0;
    Map *map = malloc(sizeof(Map));
    do {
        ch = fgetc(file);
        printf("%c", ch);
        if (ch == ' ') {
            map->tiles[height][width] = 0;
            width++;
        } else if (ch == '\n') {
            max_width = max(max_width, width);
            width = 0;
            height++;
        } else {
            map->tiles[height][width] = 1;
            width++;
        }
    } while (ch != EOF);
    map->width = max_width;
    map->height = height;
    return map;
}

void print_map(Map *map)
{
    printf("width: %d, height: %d\n", map->width, map->height);
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            printf("%d", map->tiles[i][j]);
        }
        printf("\n");
    }
}


void draw_map(SDL_Renderer *renderer, SDL_Texture *list_images[10], Map *map)
{
    int tile_width = SCREEN_WIDTH/map->width;
    int tile_height = SCREEN_HEIGHT/map->height;
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (map->tiles[i][j] == 1) {
                //printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width, i * tile_height, tile_width, tile_height};
                SDL_RenderCopy(renderer, list_images[0], NULL, &dst);
            }
        }
    }
}

// Character *create_character(char* path, int x, int y, int speed, SDL_Renderer *renderer)
// {
//     Character *character = malloc(sizeof(Character));
//     character->x = x;
//     character->y = y;
//     character->speed = speed;
//     character->image = loadImage(path, renderer);
//     SDL_QueryTexture(character->image, NULL, NULL, &character->width, &character->height);
//     return character;
// }

// void free_character(Character *character)
// {
//     SDL_DestroyTexture(character->image);
//     free(character);
// }

// void move_character(Character *character, int x, int y)
// {
//     character->x = x;
//     character->y = y;
// }

// void move_character_up(Character *character)
// {
//     character->y -= character->speed;
// }

// void move_character_down(Character *character)
// {
//     character->y += character->speed;
// }

// void move_character_left(Character *character)
// {
//     character->x -= character->speed;
// }

// void move_character_right(Character *character)
// {
//     character->x += character->speed;
// }

// void draw_character(SDL_Renderer *renderer, Character *character)
// {
//     SDL_Rect dst = {character->x, character->y, character->width, character->height};
//     SDL_RenderCopy(renderer, character->image, NULL, &dst);
// }