#include "example_2d.h"

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
    // Crée la map
    Map *map = create_map("map.txt");
    camera camera;
    create_camera(&camera, 0, 0, 13, 7);
    int tile_width = SCREEN_WIDTH / camera.width;
    int tile_height = SCREEN_HEIGHT / camera.height;
    Character *character = create_character("Textures/korigan", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, tile_width * 1.5, tile_height * 1.5, 1, renderer);
    // DEBUG MAP
    print_map(map);
    // Boucle principale
    int running = 1;
    // Chargement des textures
    SDL_Texture *list_images[100];
    // Chargement des textures
    char *list_strings[] = {"Textures/texture.png", "Textures/terre.png", "Textures/texture_limite_gauche.png", "Textures/texture_limite_droite.png", "Textures/nuage.png", "Textures/nuage_gauche.png", "Textures/nuage_droite.png", "Textures/gate.png", "Textures/gate_top.png"};
    for (int i = 0; i < 9; i++)
    {
        list_images[i] = loadImage(list_strings[i], renderer);
    }
    // Affiche la première image
    draw(renderer, bleu, list_images, map, tile_width, tile_height, character, &camera);
    printf("main\n");
    // Initialise la variable qui contient le dernier temps
    int last_time = 0;
    int last_time_sec = 0;
    while (running)
    {
        // Boucle de gestion des événements
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            // Si l'événement est de type SDL_QUIT (clic sur la croix de la fenêtre) on met fin à la boucle
            case SDL_QUIT:
                running = 0;
                break;
            // Si l'événement est de type SDL_KEYDOWN (appui sur une touche)
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
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                case SDLK_p:
                    character->speed += 0.5;
                    break;
                case SDLK_o:
                    character->speed -= 0.5;
                    break;
                case SDLK_LSHIFT:
                    character->dash = SDL_TRUE;
                }
                break;
            // Si l'événement est de type SDL_KEYUP (relachement d'une touche)
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    character->up = SDL_FALSE;
                    if (character->dy < 0)
                    {
                        character->dy = 0;
                    }
                    break;
                case SDLK_s:
                    character->down = SDL_FALSE;
                    if (character->dy < 0)
                    {
                        character->dy = 0;
                    }
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
        if (SDL_GetTicks() - last_time_sec > 1000)
        {
            last_time_sec = SDL_GetTicks();
            printf("fps: %d\n", camera.fps);
            camera.fps = 0;
        }
        // Si le temps écoulé depuis le dernier appel à SDL_GetTicks est supérieur à 16 ms
        if (SDL_GetTicks() - last_time > 1000 / MAX_FPS)
        {
            last_time = SDL_GetTicks();
            if (character->alive == SDL_FALSE)
                running = 0;
            // Applique la gravité au personnage
            gravity(character);
            // Applique le mouvement au personnage
            mouvement(map, character, tile_width, tile_height);
            // Affiche la map et le personnage dans la fenêtre
            draw(renderer, bleu, list_images, map, tile_width, tile_height, character, &camera);
            camera.fps++;
        }
    }
    printf("x: %d, y: %d \n", character->x, character->y);
    statut = EXIT_SUCCESS;
    free(map);
    free_character(character);
Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    IMG_Quit();
    // SDL_Quit();
    return statut;
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
    // Ouvre le fichier en mode lecture
    FILE *file = fopen(path, "r");
    char ch;
    int width = 0;
    int height = 0;
    int max_width = 0;
    Map *map = malloc(sizeof(Map));
    for (int i = 0; i < MAX_TILES; i++)
    {
        for (int j = 0; j < MAX_TILES; j++)
        {
            map->tiles[i][j] = 0;
        }
    }
    do
    {
        ch = fgetc(file);
        // printf("%c", ch);
        char tile_mapping[] = " #@GDN[]!T";

        for (int i = 0; i < sizeof(tile_mapping) - 1; i++)
        {
            if (ch == tile_mapping[i])
            {
                map->tiles[height][width] = i;
                width++;
                break;
            }
        }
        if (ch == '\n')
        {
            max_width = max(max_width, width);
            width = 0;
            height++;
        }
    } while (ch != EOF);
    // Ferme le fichier
    fclose(file);
    // Quand on arrive à la fin du fichier on ajoute la dernière ligne
    max_width = max(max_width, width);
    width = 0;
    height++;
    // On met à jour la largeur et la hauteur de la map
    map->width = max_width;
    map->height = height;
    // On retourne la map
    return map;
}

void print_map(Map *map)
{
    // Affiche le tableau dans la console
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

void draw_map(SDL_Renderer *renderer, SDL_Texture *list_images[100], Map *map, int tile_width, int tile_height, camera *camera)
{
    // Affiche la map dans la fenêtre
    for (int i = 0; i < map->height; i++)
    {
        for (int j = 0; j < map->width; j++)
        {
            for (int k = 1; k < 100; k++)
            {
                if (map->tiles[i][j] == k)
                {
                    SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                    if (SDL_RenderCopy(renderer, list_images[k - 1], NULL, &dst) < 0)
                    {
                        fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                    }
                    break;
                }
            }
        }
    }
}

Character *create_character(char *path, int x, int y, int width, int height, int speed, SDL_Renderer *renderer)
{
    // Crée un personnage
    Character *character = malloc(sizeof(Character));
    character->x = x;
    character->y = y;
    character->width = width;
    character->height = height;
    character->original_width = width;
    character->original_height = height;
    character->speed = speed;
    character->dx = 0;
    character->dy = 0;
    character->up = SDL_FALSE;
    character->down = SDL_FALSE;
    character->left = SDL_FALSE;
    character->right = SDL_FALSE;
    character->dash = SDL_FALSE;
    character->alive = SDL_TRUE;
    character->on_ground = SDL_FALSE;
    for (int i = 0; i < 100; i++)
        character->images[i] = NULL;
    // strcpy(result, path);
    // strcat(result, "/character.png");

    char *imageNames[] = {
        "character.png",
        "character1.png",
        "character2.png",
        "character3.png",
        "character4.png",
        "character5.png",
        "character6.png",
        "character7.png",
        "jump.png",
        "jump_right.png",
        "jump_right_fall.png"};

    for (int i = 0; i < 11; i++)
    {
        char imagePath[100];
        addcat(imagePath, path, imageNames[i]);
        character->images[i] = loadImage(imagePath, renderer);
    }
    // SDL_QueryTexture(character->image, NULL, NULL, &character->width, &character->height);
    return character;
}

char *addcat(char *result, char *path, char *name)
{
    strcpy(result, path);
    strcat(result, "/");
    strcat(result, name);
    return result;
}

void free_character(Character *character)
{
    // Libère la mémoire du personnage
    for (int i = 0; i < 100; i++)
    {
        if (character->images[i] != NULL)
        {
            SDL_DestroyTexture(character->images[i]);
        }
    }
    // SDL_DestroyTexture(character->images[0]);
    free(character);
}

void move_character(Character *character, int x, int y, Map *map, int tile_width, int tile_height)
{
    // Déplace le personnage de x et y et gère les collisions avec la map
    if (x > 0)
    {
        for (int i = 0; i < x; i++)
        {
            collision(character, map, tile_width, tile_height);
            if (character->dx == 0)
            {
                break;
            }
            character->x += 1;
        }
    }
    else
    {
        for (int i = 0; i < -x; i++)
        {
            collision(character, map, tile_width, tile_height);
            if (character->dx == 0)
            {
                break;
            }
            character->x -= 1;
        }
    }
    if (y > 0)
    {
        for (int i = 0; i < y; i++)
        {
            collision(character, map, tile_width, tile_height);
            if (character->dy == 0)
            {
                break;
            }
            character->y += 1;
        }
    }
    else
    {
        for (int i = 0; i < -y; i++)
        {
            collision(character, map, tile_width, tile_height);
            if (character->dy == 0)
            {
                break;
            }
            character->y -= 1;
        }
    }
}

void move_character_up(Character *character, int tile_height)
{
    if (character->on_ground == SDL_TRUE)
    {
        character->dy = -(tile_height / 5);
        character->on_ground = SDL_FALSE;
    }
}

void move_character_down(Character *character, int tile_height)
{
    character->dy = (tile_height / 5);
}

void move_character_left(Character *character, int tile_width)
{
    character->dx = -(tile_width / 15) * character->speed;
}

void move_character_right(Character *character, int tile_width)
{
    character->dx = (tile_width / 15) * character->speed;
}

void draw_character(SDL_Renderer *renderer, Character *character, camera *camera)
{
    // Affiche le personnage dans la fenêtre
    SDL_Rect dst = {character->x - camera->x, character->y - camera->y, character->width, character->height};
    if (character->right == SDL_TRUE && character->left == SDL_TRUE && character->on_ground == SDL_TRUE)
    {
        SDL_RenderCopy(renderer, character->images[0], NULL, &dst);
    }
    else if (character->right == SDL_TRUE && character->on_ground == SDL_TRUE)
    {
        draw_character_animation(renderer, character, &dst, camera, 1, character->speed, 7);
    }
    else if (character->left == SDL_TRUE && character->on_ground == SDL_TRUE)
    {
        draw_character_animationEx(renderer, character, &dst, camera, 1, SDL_FLIP_HORIZONTAL, character->speed, 7);
    }
    else if (character->right == SDL_TRUE)
    {
        if (character->dy > 0 && character->on_ground == SDL_FALSE)
        {
            SDL_RenderCopy(renderer, character->images[10], NULL, &dst);
        }
        else
        {
            SDL_RenderCopy(renderer, character->images[9], NULL, &dst);
        }
    }
    else if (character->left == SDL_TRUE)
    {
        if (character->dy > 0 && character->on_ground == SDL_FALSE)
        {
            SDL_RenderCopyEx(renderer, character->images[10], NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        else
        {
            SDL_RenderCopyEx(renderer, character->images[9], NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    }
    else if (character->on_ground == SDL_FALSE)
    {
        SDL_RenderCopy(renderer, character->images[8], NULL, &dst);
    }
    else if (character->down == SDL_TRUE)
    {
        SDL_RenderCopy(renderer, character->images[0], NULL, &dst);
    }
    else if (character->dash == SDL_TRUE)
    {
        SDL_RenderCopy(renderer, character->images[0], NULL, &dst);
    }
    else if (character->on_ground == SDL_TRUE)
    {
        SDL_RenderCopy(renderer, character->images[0], NULL, &dst);
    }
    else if (character->up == SDL_TRUE)
    {
        SDL_RenderCopy(renderer, character->images[8], NULL, &dst);
    }
    else
    {
        SDL_RenderCopy(renderer, character->images[0], NULL, &dst);
    }
}

void draw_character_animation(SDL_Renderer *renderer, Character *character, SDL_Rect *dst, camera *camera, int index, float speed, int nb_frame)
{
    for (int i = 0; i < nb_frame; i++)
    {
        if (camera->fps % (int)(MAX_FPS / speed) < (MAX_FPS * (i + 1)) / (nb_frame * speed))
        {
            SDL_RenderCopy(renderer, character->images[index + i], NULL, dst);
            break;
        }
    }
}

void draw_character_animationEx(SDL_Renderer *renderer, Character *character, SDL_Rect *dst, camera *camera, int index, int SDL_angle, float speed, int nb_frame)
{
    for (int i = 0; i < nb_frame; i++)
    {
        if (camera->fps % (int)(MAX_FPS / speed) < (MAX_FPS * (i + 1)) / (nb_frame * speed))
        {
            SDL_RenderCopyEx(renderer, character->images[index + i], NULL, dst, 0, NULL, SDL_angle);
            break;
        }
    }
}

void draw(SDL_Renderer *renderer, SDL_Color bleu, SDL_Texture *list_images[100], Map *map, int tile_width, int tile_height, Character *character, camera *camera)
{
    // Afficher le arrière plan puis déplacer la camera, affiche la map, le personnage dans la fenêtre et met à jour l'affichage
    setWindowColor(renderer, bleu);
    move_camera(camera, character, map);
    draw_map(renderer, list_images, map, tile_width, tile_height, camera);
    draw_character(renderer, character, camera);
    SDL_RenderPresent(renderer);
}

void mouvement(Map *map, Character *character, int tile_width, int tile_height)
{
    // Gère le mouvement du personnage
    if (character->right == SDL_TRUE)
    {
        move_character_right(character, tile_width);
    }
    if (character->left == SDL_TRUE)
    {
        move_character_left(character, tile_width);
    }
    if (character->down == SDL_TRUE && character->up == SDL_TRUE)
    {
        move_character_up(character, tile_height);
    }
    else
    {
        if (character->up == SDL_TRUE)
        {
            move_character_up(character, tile_height);
        }
        if (character->down == SDL_TRUE && character->height == character->original_height)
        {
            move_character_down(character, tile_height);
        }
    }
    if (character->dash == SDL_TRUE)
    {
        character->dx *= 20;
        character->dy *= 20;
    }
    if (character->right == SDL_TRUE && character->left == SDL_TRUE)
    {
        character->dx = 0;
    }
    if (character->down == SDL_TRUE && character->on_ground == SDL_TRUE)
    {
        character->height = (int)(character->original_height / 2);
        character->width = (int)(character->original_width / 1.5);
    }
    if (character->down == SDL_FALSE && character->height < character->original_height)
    {
        int copy_dy = character->dy;
        int copy_dx = character->dx;
        character->dy = -(int)(character->original_height / 2);
        character->dx = (int)(character->original_width / 3);
        int copy_y = character->y;
        int copy_x = character->x;
        move_character(character, character->dx, character->dy, map, tile_width, tile_height);
        character->on_ground = SDL_TRUE;
        character->dy = copy_dy;
        character->dx = copy_dx;
        // printf("dy: %d\n", character->dy);
        if (character->y == copy_y - (int)character->original_height / 2 && character->x == copy_x + (int)character->original_width / 3)
        {
            character->height = character->original_height;
            character->width = character->original_width;
            character->x = copy_x;
        }
        else
        {
            character->y = copy_y;
            character->x = copy_x;
        }
    }
    if (character->dx != 0 || character->dy != 0)
        move_character(character, character->dx, character->dy, map, tile_width, tile_height);
    if (character->dash == SDL_TRUE)
    {
        character->dx /= 20;
        character->dy /= 20;
        character->dash = SDL_FALSE;
    }
}

void gravity(Character *character)
{
    // Applique la gravité au personnage
    // Si le personnage n'est pas sur le sol et que sa vitesse verticale est inférieure à 10
    if (character->on_ground == SDL_FALSE)
    {
        if (character->dy < 20)
        {
            character->dy += 1;
        }
    }
}

void collision(Character *character, Map *map, int tile_width, int tile_height)
{
    // Gère les collisions entre le personnage et la map
    int x = character->x;
    int y = character->y;
    int width = character->width;
    int height = character->height;
    int feet = y + height;
    int center = y + height / 2;
    int x_center = x + width / 2;
    int x_tile = x / tile_width;
    // int y_tile = y / height;
    int y_tile_feet = feet / tile_height;
    int y_tile_knee = (y + height * 0.95) / tile_height;
    int y_tile_center = center / tile_height;
    int y_tile_neck = (y + height / 15) / tile_height;
    int y_tile_head = (y - height / 15) / tile_height;
    int x_tile_width = (x + width) / tile_width;
    int x_tile_right = (x + width * 1.05) / tile_width;
    int x_tile_left = (x - width / 15) / tile_width;
    int x_tile_center = x_center / tile_width;
    SDL_bool on_ground_right = SDL_TRUE;
    SDL_bool on_ground_left = SDL_TRUE;
    SDL_bool on_ground_center = SDL_TRUE;

    // Prevent heap-buffer-overflow
    if (x_tile < 0)
    {
        x_tile = 0;
    }
    if (x_tile_width < 0)
    {
        x_tile_width = 0;
    }
    if (y_tile_feet < 0)
    {
        y_tile_feet = 0;
    }
    if (y_tile_knee < 0)
    {
        y_tile_knee = 0;
    }
    if (y_tile_center < 0)
    {
        y_tile_center = 0;
    }
    if (y_tile_neck < 0)
    {
        y_tile_neck = 0;
    }
    if (y_tile_head < 0)
    {
        y_tile_head = 0;
    }
    if (x_tile_center < 0)
    {
        x_tile_center = 0;
    }
    if (x_tile > map->width - 1)
    {
        x_tile = map->width - 1;
    }
    if (x_tile_width > map->width - 1)
    {
        x_tile_width = map->width - 1;
    }
    if (y_tile_feet > map->height - 1)
    {
        y_tile_feet = map->height - 1;
    }
    if (y_tile_knee > map->height - 1)
    {
        y_tile_knee = map->height - 1;
    }
    if (y_tile_center > map->height - 1)
    {
        y_tile_center = map->height - 1;
    }
    if (y_tile_neck > map->height - 1)
    {
        y_tile_neck = map->height - 1;
    }
    if (y_tile_head > map->height - 1)
    {
        y_tile_head = map->height - 1;
    }
    if (x_tile_center > map->width - 1)
    {
        x_tile_center = map->width - 1;
    }
    // printf("x_tile: %d, y_tile: %d\n", x_tile, y_tile);

    // Si le personnage à les pieds sur le sol côté gauche et que sa vitesse verticale est positive
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
    // Si le personnage à les pieds sur le sol côté droite et que sa vitesse verticale est positive
    if (map->tiles[y_tile_feet][x_tile_center] > 0)
    {
        if (character->dy > 0)
        {
            character->dy = 0;
            character->on_ground = SDL_TRUE;
        }
    }
    else
    {
        on_ground_center = SDL_FALSE;
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
    // Si le personnage n'est pas sur le sol côté gauche et côté droit alors il n'est pas sur le sol
    if (on_ground_right == SDL_FALSE && on_ground_center == SDL_FALSE &&on_ground_left == SDL_FALSE)
    {
        character->on_ground = SDL_FALSE;
    }
    // Si le centre du personnage rentre dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_center][x_tile_right] > 0)
    {
        if (character->dx > 0)
        {
            character->dx = 0;
        }
    }
    // Si les genoux du personnage rentrent dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_knee][x_tile_right] > 0)
    {
        if (character->dx > 0)
        {
            character->dx = 0;
        }
    }
    // Si le coup du personnage rentre dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_neck][x_tile_right] > 0)
    {
        if (character->dx > 0)
        {
            character->dx = 0;
        }
    }
    // Si le centre du personnage rentre dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_center][x_tile_left] > 0)
    {
        if (character->dx < 0)
        {
            character->dx = 0;
        }
    }
    // Si les genoux du personnage rentrent dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_knee][x_tile_left] > 0)
    {
        if (character->dx < 0)
        {
            character->dx = 0;
        }
    }
    // Si le coup du personnage rentre dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_neck][x_tile_left] > 0)
    {
        if (character->dx < 0)
        {
            character->dx = 0;
        }
    }
    // Si la tête côté droit du personnage alors on annule sa vitesse verticale
    if (map->tiles[y_tile_head][x_tile] > 0)
    {
        if (character->dy < 0)
        {
            character->dy = 0;
        }
    }
    // Si la tête côté gauche du personnage rentre dans le mur de gauche alors on annule sa vitesse verticale
    if (map->tiles[y_tile_head][x_tile_width] > 0)
    {
        if (character->dy < 0)
        {
            character->dy = 0;
        }
    }
    if (character->y > map->height * tile_height)
    {
        character->alive = SDL_FALSE;
        character->y = SCREEN_HEIGHT / 2;
    }
    if (character->x > map->width * tile_width)
    {
        character->alive = SDL_FALSE;
        character->x = SCREEN_WIDTH / 2;
    }
    if (character->x < -character->width)
    {
        character->alive = SDL_FALSE;
        character->x = SCREEN_WIDTH / 2;
    }
}

void create_camera(camera *camera, int x, int y, int width, int height)
{
    // Crée la camera
    camera->x = x;
    camera->y = y;
    camera->width = width;
    camera->height = height;
    camera->fps = 0;
}

void move_camera(camera *camera, Character *character, Map *map)
{
    // Déplace la camera par rapport au personnage
    int tile_width = SCREEN_WIDTH / camera->width;
    // Si le personnage est à gauche de l'écran alors la camera est en x est à 0
    if (character->x < SCREEN_WIDTH / 2)
    {
        camera->x = 0;
    }
    // Si le personnage est à droite de l'écran alors la camera est à la fin de la map
    else if (character->x + SCREEN_WIDTH / 2 > map->width * tile_width)
    {
        camera->x = map->width * tile_width - SCREEN_WIDTH;
    }
    // Sinon la camera est centré en x par rapport au personnage
    else
    {
        camera->x = character->x - SCREEN_WIDTH / 2;
    }
    int tile_height = SCREEN_HEIGHT / camera->height;
    // Si le personnage est en haut de l'écran alors la camera est en y est à 0
    if (character->y < SCREEN_HEIGHT / 2)
    {
        camera->y = 0;
    }
    // Si le personnage est en bas de l'écran alors la camera est en bas de la map
    else if (character->y + SCREEN_HEIGHT / 2 > map->height * tile_height)
    {
        camera->y = map->height * tile_height - SCREEN_HEIGHT;
    }
    // Sinon la camera est centré en y par rapport au personnage
    else
    {
        camera->y = character->y - SCREEN_HEIGHT / 2;
    }
}
