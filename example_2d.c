#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "example_2d.h"
#include <string.h>
#include <time.h>

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    int statut = EXIT_FAILURE;
    // Crée la couleur blanche avec rgb et alpha
    // SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color bleu = {0, 102, 204, 255};

    // Initialise la fenêtre et le rendu
    if (0 != init(&window, &renderer, SCREEN_WIDTH, SCREEN_HEIGHT))
        goto Quit;

    // Remplit la fenêtre de blanc
    setWindowColor(renderer, bleu);
    // Crée la ma
    Map *map = create_map("map.txt");
    print_map(map);
    camera camera;
    create_camera(&camera, 0, 0, 13, 6);
    int tile_width = SCREEN_WIDTH / camera.width;
    int tile_height = SCREEN_HEIGHT / camera.height;
    Character *character = create_character("character.png", 0, 0, tile_width, tile_height, 1, renderer, SDL_FALSE);
    // DEBUG MAP
    // print_map(map);
    move_character(character, SCREEN_WIDTH / 2 - character->width / 2, SCREEN_HEIGHT / 2 - character->height / 2);
    // Boucle principale
    int running = 1;
    SDL_Texture *list_images[10];
    list_images[0] = loadImage("Textures/texture.png", renderer);
    list_images[1] = loadImage("Textures/terre.png", renderer);
    list_images[2] = loadImage("Textures/texture_limite_gauche.png", renderer);
    list_images[3] = loadImage("Textures/texture_limite_droite.png", renderer);
    list_images[4] = loadImage("Textures/nuage.png", renderer);
    list_images[5] = loadImage("Textures/nuage_gauche.png", renderer);
    list_images[6] = loadImage("Textures/nuage_droite.png", renderer);
    list_images[7] = loadImage("Textures/gate.png", renderer);
    list_images[8] = loadImage("Textures/gate_top.png", renderer);
    // data *data = draw_thread(renderer, bleu, map, tile_width, tile_height, character);
    draw(renderer, bleu, list_images, map, tile_width, tile_height, character, &camera);
    printf("main\n");
    // int lenght = 0;
    // const Uint8 *key;
    // struct data *data = malloc(sizeof(data));
    // data->renderer = renderer;
    // data->bleu = bleu;
    // // Crée une texture à partir d'une image BMP
    // data->list_images[0] = loadImage("tile1.bmp", renderer);
    // data->map = map;
    // data->tile_width = tile_width;
    // data->tile_height = tile_height;
    // data->character = character;
    // int button_pressed = 0;
    while (running)
    {
        if (SDL_PollEvent(&event))
        {
            // key = SDL_GetKeyboardState(&lenght);
            // if (key[SDL_SCANCODE_ESCAPE])
            // {
            //     running = 0;
            // }
            // if (key[SDL_SCANCODE_SPACE])
            // {
            //     move_character_up(character);
            // }
            // if (key[SDL_SCANCODE_S])
            // {
            //     move_character_down(character);
            // }
            // if (key[SDL_SCANCODE_A])
            // {
            //     move_character_left(character);
            // }
            // if (key[SDL_SCANCODE_D])
            // {
            //     move_character_right(character);
            // }
            // if (event.type == SDL_QUIT)
            // {
            //     running = 0;
            // }

            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    character->up = SDL_TRUE;
                    break;
                case SDLK_s:
                    character->down = SDL_TRUE;
                    break;
                case SDLK_q:
                    character->left = SDL_TRUE;
                    break;
                case SDLK_d:
                    character->right = SDL_TRUE;
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    character->up = SDL_FALSE;
                    character->dy = 0;
                    break;
                case SDLK_s:
                    character->down = SDL_FALSE;
                    character->dy = 0;
                    break;
                case SDLK_q:
                    character->left = SDL_FALSE;
                    character->dx = 0;
                    break;
                case SDLK_d:
                    character->right = SDL_FALSE;
                    character->dx = 0;
                    break;
                }
                break;
            }
        }
        // data->character = character;
        //  Dessine les textures dans le rendu

        // if (SDL_GetTicks() % 100 == 0)
        // {
        //     // printf("Timestamp: %d\n", SDL_GetTicks());
        //     gravity(character);
        // }
        if (SDL_GetTicks() % 30 == 0)
        {
            // printf("Timestamp: %d\n", SDL_GetTicks());
            gravity(character);
            mouvement(map, character);
            draw(renderer, bleu, list_images, map, tile_width, tile_height, character, &camera);
        }
        // gravity(character);
        // collision(character, map);
        
    }
    printf("x: %d, y: %d \n", character->x, character->y);
    statut = EXIT_SUCCESS;
    free(map);
    free_character(character);
Quit:
    // free(data);
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    // Crée la fenêtre et le rendu
    if (0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_RESIZABLE, window, renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer)
{
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    // Charge l'image dans une surface temporaire
    tmp = IMG_Load(path);
    if (NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
        return NULL;
    }
    // Crée une texture à partir de la surface temporaire
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    // Libère la surface temporaire
    SDL_FreeSurface(tmp);
    if (NULL == texture)
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
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    // Remplit toute la fenêtre de la couleur du pinceau
    if (SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

Map *create_map(char *path)
{
    FILE *file = fopen(path, "r");
    char ch;
    int width = 0;
    int height = 0;
    int max_width = 0;
    Map *map = malloc(sizeof(Map));
    do
    {
        ch = fgetc(file);
        // printf("%c", ch);
        if (ch == ' ')
        {
            map->tiles[height][width] = 0;
            width++;
        }
        else if (ch == '\n')
        {
            max_width = max(max_width, width);
            width = 0;
            height++;
        }
        else if (ch == '#')
        {
            map->tiles[height][width] = 1;
            width++;
        }
        else if (ch == '@')
        {
            map->tiles[height][width] = 2;
            width++;
        }
        else if (ch == 'G')
        {
            map->tiles[height][width] = 3;
            width++;
        }
        else if (ch == 'D')
        {
            map->tiles[height][width] = 4;
            width++;
        }
        else if (ch == 'N')
        {
            map->tiles[height][width] = 5;
            width++;
        }
        else if (ch == '[')
        {
            map->tiles[height][width] = 6;
            width++;
        }
        else if (ch == ']')
        {
            map->tiles[height][width] = 7;
            width++;
        }
        else if (ch == '!')
        {
            map->tiles[height][width] = 8;
            width++;
        }
        else if (ch == 'T')
        {
            map->tiles[height][width] = 9;
            width++;
        }
    } while (ch != EOF);
    max_width = max(max_width, width);
    width = 0;
    height++;
    map->width = max_width;
    map->height = height;
    printf("width: %d, height: %d\n", map->width, map->height);
    return map;
}

void print_map(Map *map)
{
    printf("width: %d, height: %d\n", map->width, map->height);
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            printf("%d", map->tiles[i][j]);
        }
        printf("\n");
    }
}

void draw_map(SDL_Renderer *renderer, SDL_Texture *list_images[10], Map *map, int tile_width, int tile_height, camera *camera)
{
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            if (map->tiles[i][j] == 1)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[0], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
            else if (map->tiles[i][j] == 2)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[1], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }

            else if (map->tiles[i][j] == 3)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[2], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
            else if (map->tiles[i][j] == 4)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[3], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
            else if (map->tiles[i][j] == 5)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[4], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
            else if (map->tiles[i][j] == 6)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[5], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
            else if (map->tiles[i][j] == 7)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[6], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
            else if (map->tiles[i][j] == 8)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[7], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
            else if (map->tiles[i][j] == 9)
            {
                // printf("i: %d, j: %d\n", i, j);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, list_images[8], NULL, &dst) < 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
        }
    }
}

Character *create_character(char *path, int x, int y, int width, int height, int speed, SDL_Renderer *renderer, SDL_bool on_ground)
{
    Character *character = malloc(sizeof(Character));
    character->x = x;
    character->y = y;
    character->width = width;
    character->height = height;
    character->speed = speed;
    character->image = loadImage(path, renderer);
    character->on_ground = on_ground;
    // SDL_QueryTexture(character->image, NULL, NULL, &character->width, &character->height);
    return character;
}

void free_character(Character *character)
{
    SDL_DestroyTexture(character->image);
    free(character);
}

void move_character(Character *character, int x, int y)
{
    character->x = x;
    character->y = y;
}

void move_character_up(Character *character)
{
    if (character->on_ground == SDL_TRUE)
    {
        character->dy = -(character->height / 5);
        character->on_ground = SDL_FALSE;
    }
}

void move_character_down(Character *character)
{
    character->dy = (character->height / 5);
}

void move_character_left(Character *character)
{
    character->dx = -(character->width / 15) * character->speed;
}

void move_character_right(Character *character)
{
    character->dx = (character->width / 15) * character->speed;
}

void draw_character(SDL_Renderer *renderer, Character *character, camera *camera)
{
    SDL_Rect dst = {character->x - camera->x, character->y - camera->y, character->width, character->height};
    SDL_RenderCopy(renderer, character->image, NULL, &dst);
}

void draw(SDL_Renderer *renderer, SDL_Color bleu, SDL_Texture *list_images[10], Map *map, int tile_width, int tile_height, Character *character, camera *camera)
{
    setWindowColor(renderer, bleu);
    move_camera(camera, character, map);
    draw_map(renderer, list_images, map, tile_width, tile_height, camera);
    draw_character(renderer, character, camera);
    SDL_RenderPresent(renderer);
}

void mouvement(Map *map, Character *character)
{
    if (character->right == SDL_TRUE)
    {
        move_character_right(character);
    }
    if (character->left == SDL_TRUE)
    {
        move_character_left(character);
    }
    if (character->up == SDL_TRUE)
    {
        move_character_up(character);
    }
    if (character->down == SDL_TRUE)
    {
        move_character_down(character);
    }
    collision(character, map);
    move_character(character, character->x + character->dx, character->y + character->dy);
    // character->dx = 0;
    // character->dy = 0;
}

void gravity(Character *character)
{
    if (character->on_ground == SDL_FALSE)
    {
        if (character->dy < 10)
        {
            character->dy += 1;
        }
    }
}

void collision(Character *character, Map *map)
{
    int x = character->x;
    int y = character->y;
    int width = character->width;
    int height = character->height;
    int feet = y + height;
    int center = y + height / 2;
    int x_tile = x / width;
    // int y_tile = y / height;
    int y_tile_feet = feet / height;
    int y_tile_center = center / height;
    int y_tile_head = (y - height / 15) / height;
    int x_tile_width = (x + width) / width;
    int x_tile_right = (x + width * 1.05) / width;
    int x_tile_left = (x - width / 15) / width;
    SDL_bool on_ground_right = SDL_TRUE;
    SDL_bool on_ground_left = SDL_TRUE;
    // printf("x_tile: %d, y_tile: %d\n", x_tile, y_tile);
    if (map->tiles[y_tile_feet][x_tile] > 0)
    {
        if (character->dy > 0)
        {
            character->dy = 0;
            character->on_ground = SDL_TRUE;
        }
    }
    else
    {
        on_ground_right = SDL_FALSE;
    }
    if (map->tiles[y_tile_feet][x_tile_width] > 0)
    {
        if (character->dy > 0)
        {
            character->dy = 0;
            character->on_ground = SDL_TRUE;
        }
    }
    else
    {
        on_ground_left = SDL_FALSE;
    }
    if (on_ground_right == SDL_FALSE && on_ground_left == SDL_FALSE)
    {
        character->on_ground = SDL_FALSE;
    }
    if (map->tiles[y_tile_center][x_tile_right] > 0)
    {
        if (character->dx > 0)
        {
            character->dx = 0;
        }
    }
    if (map->tiles[y_tile_center][x_tile_left] > 0)
    {
        if (character->dx < 0)
        {
            character->dx = 0;
        }
    }
    if (map->tiles[y_tile_head][x_tile] > 0)
    {
        if (character->dy < 0)
        {
            character->dy = 0;
        }
    }
    if (map->tiles[y_tile_head][x_tile_width] > 0)
    {
        if (character->dy < 0)
        {
            character->dy = 0;
        }
    }
}

void create_camera(camera *camera, int x, int y, int width, int height)
{
    camera->x = x;
    camera->y = y;
    camera->width = width;
    camera->height = height;
}

void move_camera(camera *camera, Character *character, Map *map)
{
    int tile_width = SCREEN_WIDTH / camera->width;
    if (character->x < SCREEN_WIDTH / 2)
    {
        camera->x = 0;
    }
    else if (character->x + SCREEN_WIDTH / 2 > map->width * tile_width)
    {
        camera->x = map->width * tile_width - SCREEN_WIDTH;
    }
    else
    {
        camera->x = character->x - SCREEN_WIDTH / 2;
    }
    int tile_height = SCREEN_HEIGHT / camera->height;
    if (character->y < SCREEN_HEIGHT / 2)
    {
        // camera->y = 0;
        camera->y = 0;
    }
    else if (character->y + SCREEN_HEIGHT / 2 > map->height * tile_height)
    {
        camera->y = map->height * tile_height - SCREEN_HEIGHT;
    }
    else
    {
        camera->y = character->y - SCREEN_HEIGHT / 2;
    }
    // camera->x = character->x - SCREEN_WIDTH / 2;
    // camera->y = character->y - SCREEN_HEIGHT / 2;
}
// data *draw_thread(SDL_Renderer *renderer, SDL_Color bleu, Map *map, int tile_width, int tile_height, Character *character)
// {
//     struct data *data = malloc(sizeof(data));
//     data->renderer = renderer;gravity
//     data->bleu = bleu;
//     // Crée une texture à partir d'une image BMP
//     data->list_images[0] = loadImage("tile1.bmp", renderer);
//     data->map = map;
//     data->tile_width = tile_width;
//     data->tile_height = tile_height;
//     data->character = character;
//     data->running = 1;
//     printf("draw_thread\n");
//     // SDL_CreateThread(draw, "draw", data);
//     return data;
// }
