#include "../include/main.h"
#include "../include/controls.h"
#include "../include/init.h"
#include "../include/movement.h"
#include "../include/procedural_generation.h"
#include <time.h>

int SCREEN_WIDTH = 1300;
int SCREEN_HEIGHT = 700;

int main(void) {
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
    // Initialise la bibliothèque SDL_ttf en dehors de la fonction init car elle ne fonctionne pas dans la fonction init
    // TODO: Trouver pourquoi
    TTF_Init();
    // Récupère la taille de l'écran
    SDL_Rect screen_size;
    if (0 != SDL_GetDisplayBounds(0, &screen_size)) {
        fprintf(stderr, "Erreur SDL_GetDisplayBounds : %s", SDL_GetError());
        goto Quit;
    }
    // Récupère la taille de l'écran
    SCREEN_WIDTH = screen_size.w;
    SCREEN_HEIGHT = screen_size.h;
    // Met à jour la taille de la fenêtre
    SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
    // Met la fenêtre en plein écran
    // On fait ça parce que le fullscreen ne met pas à jour la taille de la fenêtre
    // mais trigger l'event SDL_WINDOWEVENT_RESIZED
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    // printf("screen_size: %d, %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    //  Remplit la fenêtre de blanc
    setWindowColor(renderer, bleu);
    // Crée la map
    Map *map = create_map("map.txt");
    Camera camera;
    // Crée la caméra en fonction de la taille de la fenêtre
    int camera_width = (int) (SCREEN_WIDTH / 100);
    int camera_height = (int) (SCREEN_HEIGHT / 100);
    // Crée la caméra en position 0, 0 et de taille camera_width, camera_height
    create_camera(&camera, 0, 0, camera_width, camera_height);
    int tile_width = SCREEN_WIDTH / camera_width;
    int tile_height = SCREEN_HEIGHT / camera_height;
    // printf("tile_width: %d, tile_height: %d\n", tile_width, tile_height);
    Character *character = create_character(map->tile_start_x * tile_width, map->tile_start_y * tile_height,
                                            tile_width * 0.9, tile_height * 1.5, 2, renderer);
    // DEBUG MAP
    print_map(map);
    // Initialise la seed pour le random
    srand(time(NULL));
    // Boucle principale
    int running = 1;
    // Chargement des textures
    Texture *texture = create_texture(renderer);
    // Affiche la première image
    draw(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
    // printf("main\n");
    //  Initialise la variable qui contient le dernier temps
    long long last_time = 0;
    long long last_time_fps = 0;
    long long last_time_sec = 0;
    // Initialise la variable qui contient les contrôles
    Controls *controls = init_controls();

    while (running) {
        // Boucle de gestion des événements
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                // Si l'événement est de type SDL_QUIT (clic sur la croix de la fenêtre) on met fin à la boucle
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        // Si la fenêtre est redimensionnée on met à jour la taille de la fenêtre
                        // et la taille des tuiles et la taille de la caméra et la taille du personnage
                        // On récupère la nouvelle taille de la fenêtre
                        SCREEN_WIDTH = event.window.data1;
                        SCREEN_HEIGHT = event.window.data2;
                        // On limite la taille de la fenêtre à 100x100 pour éviter les crashs (valeur testé empiriquement)
                        if (SCREEN_WIDTH < 100) {
                            SCREEN_WIDTH = 100;
                        }
                        if (SCREEN_HEIGHT < 100) {
                            SCREEN_HEIGHT = 100;
                        }
                        printf("SCREEN_WIDTH: %d, SCREEN_HEIGHT: %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
                        // Modification de la taille de la camera pour que la taille des tuiles soit environ de 100x100
                        // Car ça permet d'avoir une physique constante
                        // Problème de gestion de la gravité qui ne dépend pas de la taille des tuiles (Compliqué à gérer)
                        camera.width = (int) (SCREEN_WIDTH / 100);
                        camera.height = (int) (SCREEN_HEIGHT / 100);
                        int old_tile_width = tile_width;
                        int old_tile_height = tile_height;
                        // On met à jour la taille des tuiles
                        tile_width = SCREEN_WIDTH / camera.width;
                        tile_height = SCREEN_HEIGHT / camera.height;
                        // Permet de garder la position du personnage dans la même case
                        int tile_x = character->x / old_tile_width;
                        int tile_y = character->y / old_tile_height;
                        // Permet de garder la position du personnage dans à la même position dans la case
                        int sub_tile_x = character->x % old_tile_width;
                        int sub_tile_y = character->y % old_tile_height;
                        // On met à jour la position du personnage en x
                        character->x =
                                tile_x * tile_width + (int) (sub_tile_x * (float) tile_width / (float) old_tile_width);
                        // On met à jour la position du personnage en y
                        character->y = tile_y * tile_height +
                                       (int) (sub_tile_y * (float) tile_height / (float) old_tile_height);
                        // printf("tile_width: %d, tile_height: %d\n", tile_width, tile_height);
                        // On met à jour la taille du personnage et sa taille d'origine
                        character->width = tile_width * 0.9;
                        character->height = tile_height * 1.5;
                        character->original_width = tile_width * 0.9;
                        character->original_height = tile_height * 1.5;
                        // Appel la fonction collision pour mettre à jour les collisions (pour mettre à jour la gravité)
                        collision(character, map, tile_width, tile_height);
                        // Affiche la map et le personnage dans la fenêtre avec la nouvelle taille
                        draw(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
                    }
                    break;
                    // Si l'événement est de type SDL_KEYDOWN (appui sur une touche)
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == controls->down) {
                        character->down = SDL_TRUE;
                    } else if (event.key.keysym.sym == controls->left) {
                        character->left = SDL_TRUE;
                    } else if (event.key.keysym.sym == controls->right) {
                        character->right = SDL_TRUE;
                    } else {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
                                character->up = SDL_TRUE;
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
                                action_dash(character, controls);
                                break;
                            case SDLK_KP_5:
                                character->dash->on_air = SDL_TRUE;
                                break;
                            case SDLK_KP_8:
                                character->dash->go_up = SDL_TRUE;
                                break;
                            case SDLK_F11:
                                // Si la fenêtre est en plein écran on la met en mode fenêtré et inversement
                                if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
                                    SDL_SetWindowFullscreen(window, 0);
                                } else {
                                    // Récupère la taille de l'écran
                                    SCREEN_WIDTH = screen_size.w;
                                    SCREEN_HEIGHT = screen_size.h;
                                    // Met à jour la taille de la fenêtre
                                    SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
                                    // Met la fenêtre en plein écran
                                    // On fait ça parce que le fullscreen ne met pas à jour la taille de la fenêtre
                                    // mais trigger l'event SDL_WINDOWEVENT_RESIZED
                                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                                }
                                break;
                            case SDLK_F3:
                                camera.show_fps = !camera.show_fps;
                                break;
                            case SDLK_TAB:
                                switchLayout(controls);
                                break;
                        }
                    }
                    break;
                    // Si l'événement est de type SDL_KEYUP (relachement d'une touche)
                case SDL_KEYUP:
                    if (event.key.keysym.sym == controls->down) {
                        character->down = SDL_FALSE;
                        if (character->dy < 0) {
                            character->dy = 0;
                        }
                        break;
                    } else if (event.key.keysym.sym == controls->left) {
                        character->left = SDL_FALSE;
                        character->dx = 0;
                        break;
                    } else if (event.key.keysym.sym == controls->right) {
                        character->right = SDL_FALSE;
                        character->dx = 0;
                        break;
                    } else {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
                                character->up = SDL_FALSE;
                                if (character->dy < 0) {
                                    character->dy = 0;
                                }
                                break;
                        }
                    }
                    break;
            }
        }
        if (getCurrentTimeInMicroseconds() - last_time_sec >= 1000000) {
            last_time_sec = getCurrentTimeInMicroseconds();
            // printf("fps: %d\n", camera.fps);
            camera.avg_fps = camera.fps;
            camera.fps = 0;
        }
        // Si le temps écoulé depuis le dernier appel à SDL_GetTicks est supérieur à 16 ms
        // C'est la condition qui donne le game tick (60 fois par seconde) cad 16 ms par tick
        if (getCurrentTimeInMicroseconds() - last_time >= 1000000 / 60) {
            last_time = getCurrentTimeInMicroseconds();
            if (character->alive == SDL_FALSE)
                running = 0;
            // Applique la gravité au personnage
            gravity(character);
            // Applique le mouvement au personnage
            mouvement(map, character, tile_width, tile_height);
            // Affiche la map et le personnage dans la fenêtre
            // draw(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
            // camera.fps++;
        }
        // C'est la condition qui donne le FPS
        if (getCurrentTimeInMicroseconds() - last_time_fps >= 1000000 / MAX_FPS) {
            last_time_fps = getCurrentTimeInMicroseconds();
            draw(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
            camera.fps++;
        }
    }
    // printf("x: %d, y: %d \n", character->x, character->y);
    statut = EXIT_SUCCESS;
    free(controls);
    free(map);
    free(character->dash);
    free(character);
    free_texture(texture);
    Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return statut;
}

long long getCurrentTimeInMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long) tv.tv_sec * 1000000 + tv.tv_usec;
}

RdmTexture *load_from_dir(char *dir_path, SDL_Renderer *renderer){
    //initialisation de rdmtexture
    RdmTexture *Rdmtexture = malloc(sizeof(RdmTexture));
    for (int i=0; i<10; i++){
        Rdmtexture->Data[i]=NULL;
    }
    Rdmtexture->size=0;
    //Cas ou le path pointe sur un png
    if (strstr(dir_path, ".png") != NULL && strstr(dir_path, ".png") == dir_path + strlen(dir_path) - 4) {
        Rdmtexture->Data[0]= loadImage(strdup(dir_path), renderer);
        Rdmtexture->size=1;
    }
    //Cas dossier
    else {
        DIR *dir;
        struct dirent *entry;
        dir = opendir(dir_path);
        if (!dir) {
            printf("empty");
            return Rdmtexture;
        }
        int i = 0;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                char path[50];
                snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
                //printf("%s\n",str);
                Rdmtexture->Data[i] = loadImage(strdup(path), renderer);
                Rdmtexture->size = i + 1;
                i++;
                if (i >= 10) break; // to prevent buffer overflow
            }
        }
        closedir(dir);
    }
    return Rdmtexture;
}

Texture *create_texture(SDL_Renderer *renderer) {
    Texture *texture = malloc(sizeof(Texture));
    // Initialisation du tableau de textures à NULL pour éviter les problèmes de mémoire
    for (int i = 0; i < 100; i++) {
        texture->collision[i] = NULL;
        texture->transparent[i] = NULL;
        texture->main_character[i] = NULL;
    }
    // Liste des noms des images de la map (collisables) avec "END" A la fin
    char *list_strings[] = {"Textures/Terrain/sol",//19
                            "Textures/Terrain/ss1",//29
                            "Textures/Terrain/ss2",//39
                            "Textures/Terrain/marron_cave.png",//49
                            "Textures/Terrain/sol_gauche.png",//59
                            "Textures/Terrain/ss1_gauche.png",
                            "Textures/Terrain/ss2_gauche.png",
                            "Textures/Terrain/ss3_gauche.png",
                            "Textures/Terrain/sol_droite.png",
                            "Textures/Terrain/ss1_droite.png",
                            "Textures/Terrain/ss2_droite.png",
                            "Textures/Terrain/ss3_droite.png",
                            "END"};
    // Charge les textures des images de la map (collisables)
    for (int i = 0; list_strings[i]!= "END"; i++) {
        texture->collision[i] = load_from_dir(list_strings[i], renderer);
    }
    // Liste des noms des images de la map (transparentes)
    /*char *list_strings_bis[] = {"Textures/Terrain/nuage","Textures/Terrain/herbe",
                                "Textures/Terrain/herbe_gauche.png",
                                "Textures/Terrain/herbe_droite.png",
                                "END"};
    // Charge les textures des images de la map (transparentes)
    for (int i = 0; list_strings[i]!= "END"; i++) {
        texture->transparent[i] = load_from_dir(list_strings_bis[i], renderer);
    }*/
    // Initialisation du tableau de textures du personnage à NULL pour éviter les problèmes de mémoire
    for (int i = 0; i < 100; i++)
        texture->main_character[i] = NULL;
    // Liste des noms des images du personnage
    char *imageNames[] = {
            "character.png",
            "character_run.png",
            "jump.png",
            "jump_right.png",
            "jump_right_fall.png"};
    // Chargement des textures du personnage
    for (int i = 0; i < 5; i++) {
        char imagePath[100];
        addcat(imagePath, "Textures/korigan", imageNames[i]);
        texture->main_character[i] = loadImage(imagePath, renderer);
    }
    // Crée une police de caractère avec le fichier arial.ttf de taille 28
    texture->font = TTF_OpenFont("Fonts/arial.ttf", 28);
    if (texture->font == NULL)
        fprintf(stderr, "Erreur TTF_OpenFont : %s", TTF_GetError());
    return texture;
}

void free_texture(Texture *texture) {
    // Libère la mémoire allouée pour les textures
    for (int i = 0; i < 100; i++) {
        if (NULL != texture->collision[i]) {
            for (int j = 0; j < 10; j++) {
                if (NULL != texture->collision[i]->Data[j]){
                    SDL_DestroyTexture(texture->collision[i]->Data[j]);
                }
            }
        }
        if (NULL != texture->transparent[i]) {
            for (int j = 0; j < 10; j++) {
                if (NULL != texture->transparent[i]->Data[j]){
                    SDL_DestroyTexture(texture->transparent[i]->Data[j]);
                }
            }
        }
        if (NULL != texture->main_character[i])
            SDL_DestroyTexture(texture->main_character[i]);
    }
    // Libère la mémoire allouée pour la police de caractère
    TTF_CloseFont(texture->font);
    free(texture);
}

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer) {
    SDL_Surface *tmp = NULL;
    SDL_Texture *texture = NULL;
    // Charge l'image dans une surface temporaire
    tmp = IMG_Load(path);
    if (NULL == tmp) {
        fprintf(stderr, "Erreur IMG_Load : %s", SDL_GetError());
        return NULL;
    }
    // Crée une texture à partir de la surface temporaire
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    // Libère la surface temporaire
    SDL_FreeSurface(tmp);
    if (NULL == texture) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    // Retourne la texture
    return texture;
}

int setWindowColor(SDL_Renderer *renderer, SDL_Color color) {
    // Change la couleur du pinceau utiliser (un peu comme sur paint tu change la couleur du pinceau avant de dessiner)
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0)
        return -1;
    // Remplit toute la fenêtre de la couleur du pinceau
    if (SDL_RenderClear(renderer) < 0)
        return -1;
    return 0;
}

Map *create_map(char *path) {
    // Ouvre le fichier en mode lecture
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur fopen : le fichier %s n'a pas pu être ouvert\n", path);
        return NULL;
    }
    // Initialise les variables
    char ch;
    int width = 0;
    int height = 0;
    int max_width = 0;
    Map *map = malloc(sizeof(Map));
    map->full = SDL_FALSE;
    // Position de départ du personnage (case de départ) par défaut
    map->tile_start_x = 1;
    map->tile_start_y = 1;
    // Remplit le tableau avec des 0 pour éviter les problèmes de mémoire
    for (int i = 0; i < MAX_TILES; i++) {
        for (int j = 0; j < MAX_TILES; j++) {
            map->tiles[i][j] = 0;
        }
    }
    do {
        ch = fgetc(file);
        // printf("%c", ch);
        // Remplit le tableau avec les valeurs correspondantes aux caractères du fichier texte (voir map.txt pour plus d'infos)
        char tile_mapping[] = " #cC@G[{(D]})";

        for (int i = 0; i < sizeof(tile_mapping) - 1; i++) {
            if (ch == tile_mapping[i]) {
                int random_texture=rand() % 10;
                srand(rand());
                map->tiles[height][width] = i*10+random_texture;
                //exemple d'herbe/texture transparente relative
                if (i == 1 && height > 0 && map->tiles[height - 1][width] == 0) {
                    random_texture =rand() % 10;
                    srand(rand());
                    map->tiles[height - 1][width] = -20-random_texture;
                }
                width++;
                break;
            }
        }
        // Si c'est un 0 on met -1 dans le tableau (case de départ du personnage)
        if (ch == '0') {
            map->tiles[height][width] = -1;
            map->tile_start_x = width;
            map->tile_start_y = height;
            width++;
        }
        // Si c'est un retour à la ligne on met à jour la largeur et la hauteur de la map
        if (ch == '\n') {
            max_width = max(max_width, width);
            width = 0;
            height++;
        }
    } while (ch != EOF);
    // Ferme le fichier
    fclose(file);
    for (int i = 0; i < MAX_TILES; i++) {
        for (int j = 0; j < MAX_TILES; j++) {
            if (i>0 && (map->tiles[j][i])>= 10 && (map->tiles[j][i])<20 && (map->tiles[j][i-1])<10){
                map->tiles[j][i]=50;
            }
            else if (i>0 && (map->tiles[j][i])>= 10 && (map->tiles[j][i])<20 && (map->tiles[j][i+1])<10){
                map->tiles[j][i]=90;
            }
            if (j>0 && (map->tiles[j][i])>= 10 && (map->tiles[j-1][i])>=10){
                if ((map->tiles[j-1][i])>9 && map->tiles[j-1][i] <=29) {
                    map->tiles[j][i] = map->tiles[j-1][i]+10;
                }
                else if ((map->tiles[j-1][i])>=50 && map->tiles[j-1][i] <= 79) {
                    map->tiles[j][i] = map->tiles[j-1][i]+10;
                }
                else if ((map->tiles[j-1][i])>=90 && map->tiles[j-1][i] <= 119) {
                    map->tiles[j][i] = map->tiles[j-1][i]+10;
                }
                else {map->tiles[j][i]=40;}
            }
        }
    }
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

void print_map(Map *map) {
    // Affiche le tableau dans la console
    printf("width: %d, height: %d\n", map->width, map->height);
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            // Si la case contient un nombre positif on affiche un espace avant le nombre
            if (map->tiles[i][j] >= 0)
                printf(" ");
            printf("%d", map->tiles[i][j]);
        }
        printf("\n");
    }
}



void draw_map(SDL_Renderer *renderer, Texture *texture, Map *map, int tile_width, int tile_height, Camera *camera) {
    // Affiche la map dans la fenêtre
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            for (int k = 1; k < 1000; k++) {
                if (map->tiles[i][j] == k && k>=10) {
                    // Si la case contient un nombre positif on affiche la texture correspondante (collisonable)
                    int num_texture = k / 10;
                    int num_image = (k % 10)%(texture->collision[num_texture-1]->size);
                    SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                    if (SDL_RenderCopy(renderer, texture->collision[num_texture-1]->Data[num_image], NULL,
                                       &dst) < 0) {
                        fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                    }
                    break;
                }
                /*else if (map->tiles[i][j] == -k && k != 1) {
                    // Si la case contient un nombre négatif on affiche la texture correspondante (transparente)
                    int num_image = k % 10;
                    int num_texture = k / 10;
                    SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                    if (SDL_RenderCopy(renderer, texture->transparent[num_texture - 1]->Data[num_image % (texture->transparent[num_texture - 1]->size)], NULL, &dst) < 0) {
                        fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                    }
                    break;
                }*/
            }
        }
    }
}

Character *create_character(int x, int y, int width, int height, int speed, SDL_Renderer *renderer) {
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
    character->alive = SDL_TRUE;
    character->on_ground = SDL_FALSE;
    init_dash(character);
    return character;
}

char *addcat(char *result, char *path, char *name) {
    strcpy(result, path);
    strcat(result, "/");
    strcat(result, name);
    return result;
}

void draw_character(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera) {
    // Affiche le personnage dans la fenêtre
    SDL_Rect dst = {character->x - camera->x, character->y - camera->y, character->width, character->height};
    if (character->right == SDL_TRUE && character->left == SDL_TRUE && character->on_ground == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[0], NULL, &dst);
    } else if (character->right == SDL_TRUE && character->on_ground == SDL_TRUE && character->dx != 0) {
        draw_character_animation(renderer, character, texture, &dst, camera, 1, character->speed, 7);
    } else if (character->left == SDL_TRUE && character->on_ground == SDL_TRUE && character->dx != 0) {
        draw_character_animationEx(renderer, character, texture, &dst, camera, 1, SDL_FLIP_HORIZONTAL, character->speed,
                                   7);
    } else if (character->right == SDL_TRUE && character->dx != 0) {
        if (character->dy > 0 && character->on_ground == SDL_FALSE) {
            SDL_RenderCopy(renderer, texture->main_character[4], NULL, &dst);
        } else {
            SDL_RenderCopy(renderer, texture->main_character[3], NULL, &dst);
        }
    } else if (character->left == SDL_TRUE && character->dx != 0) {
        if (character->dy > 0 && character->on_ground == SDL_FALSE) {
            SDL_RenderCopyEx(renderer, texture->main_character[4], NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            SDL_RenderCopyEx(renderer, texture->main_character[3], NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    } else if (character->on_ground == SDL_FALSE) {
        SDL_RenderCopy(renderer, texture->main_character[2], NULL, &dst);
    } else if (character->down == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[0], NULL, &dst);
    } else if (character->dash->duration > 0) {
        SDL_RenderCopy(renderer, texture->main_character[0], NULL, &dst);
    } else if (character->on_ground == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[0], NULL, &dst);
    } else if (character->up == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[2], NULL, &dst);
    } else {
        SDL_RenderCopy(renderer, texture->main_character[0], NULL, &dst);
    }
}

void draw_character_animation(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera,
                         int index, float speed, int nb_frame) {
    // Affiche une animation du personnage dans la fenêtre (déplacement vers la droite)
    SDL_Rect src = {0, 0, 0, 0};
    SDL_QueryTexture(texture->main_character[index], NULL, NULL, &src.w, &src.h);
    src.w /= nb_frame;
    for (int i = 0; i < nb_frame; i++) {
        if (camera->fps % (int) (MAX_FPS / speed) < (MAX_FPS * (i + 1)) / (nb_frame * speed)) {
            src.x = i * src.w;
            SDL_RenderCopy(renderer, texture->main_character[index], &src, dst);
            break;
        }
    }
}

void draw_character_animationEx(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst,
                                Camera *camera, int index, int SDL_angle, float speed, int nb_frame) {
    // Affiche une animation du personnage dans la fenêtre avec une rotation (déplacement vers la gauche)
    SDL_Rect src = {0, 0, 0, 0};
    SDL_QueryTexture(texture->main_character[index], NULL, NULL, &src.w, &src.h);
    src.w /= nb_frame;
    for (int i = 0; i < nb_frame; i++) {
        if (camera->fps % (int) (MAX_FPS / speed) < (MAX_FPS * (i + 1)) / (nb_frame * speed)) {
            src.x = i * src.w;
            SDL_RenderCopyEx(renderer, texture->main_character[index], &src, dst, 0, NULL, SDL_angle);
            break;
        }
    }
}

void draw_fps(SDL_Renderer *renderer, Camera *camera, Texture *texture) {
    if (camera->show_fps == SDL_TRUE) {
        // Affiche le nombre d'images par seconde dans la fenêtre
        char fps[100];
        sprintf(fps, "FPS: %d", camera->avg_fps);
        // Met le texte en gras et en italique
        TTF_SetFontStyle(texture->font, TTF_STYLE_ITALIC | TTF_STYLE_BOLD);
        // Met le texte en blanc
        SDL_Color color = {255, 255, 255, 255};
        // Crée une surface à partir du texte
        SDL_Surface *surface = TTF_RenderText_Solid(texture->font, fps, color);
        // Crée une texture à partir de la surface
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        // Positionne le texte dans la fenêtre les valeurs 100 et 100 ne servent à rien car modifier juste après
        SDL_Rect dst = {0, 0, 100, 100};
        // Récurpére la taille du texte et modifier la taille de la zone d'affichage
        SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void draw(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Map *map, int tile_width, int tile_height,
          Character *character, Camera *camera) {
    // Afficher le arrière plan puis déplacer la camera, affiche la map, le personnage dans la fenêtre et met à jour l'affichage
    setWindowColor(renderer, bleu);
    move_camera(camera, character, map);
    draw_map(renderer, texture, map, tile_width, tile_height, camera);
    draw_character(renderer, character, texture, camera);
    draw_fps(renderer, camera, texture);
    SDL_RenderPresent(renderer);
}


void collision(Character *character, Map *map, int tile_width, int tile_height) {
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
    if (x_tile < 0) {
        x_tile = 0;
    }
    if (x_tile_width < 0) {
        x_tile_width = 0;
    }
    if (y_tile_feet < 0) {
        y_tile_feet = 0;
    }
    if (y_tile_knee < 0) {
        y_tile_knee = 0;
    }
    if (y_tile_center < 0) {
        y_tile_center = 0;
    }
    if (y_tile_neck < 0) {
        y_tile_neck = 0;
    }
    if (y_tile_head < 0) {
        y_tile_head = 0;
    }
    if (x_tile_center < 0) {
        x_tile_center = 0;
    }
    if (x_tile > map->width - 1) {
        x_tile = map->width - 1;
    }
    if (x_tile_width > map->width - 1) {
        x_tile_width = map->width - 1;
    }
    if (y_tile_feet > map->height - 1) {
        y_tile_feet = map->height - 1;
    }
    if (y_tile_knee > map->height - 1) {
        y_tile_knee = map->height - 1;
    }
    if (y_tile_center > map->height - 1) {
        y_tile_center = map->height - 1;
    }
    if (y_tile_neck > map->height - 1) {
        y_tile_neck = map->height - 1;
    }
    if (y_tile_head > map->height - 1) {
        y_tile_head = map->height - 1;
    }
    if (x_tile_center > map->width - 1) {
        x_tile_center = map->width - 1;
    }
    // printf("x_tile: %d, y_tile: %d\n", x_tile, y_tile);

    // Si le personnage à les pieds sur le sol côté gauche et que sa vitesse verticale est positive
    if (map->tiles[y_tile_feet][x_tile] > 0) {
        if (character->dy > 0) {
            character->dy = 0;
            character->on_ground = SDL_TRUE;
        }
    } else {
        on_ground_right = SDL_FALSE;
    }
    // Si le personnage à les pieds sur le sol au centre et que sa vitesse verticale est positive
    if (map->tiles[y_tile_feet][x_tile_center] > 0) {
        if (character->dy > 0) {
            character->dy = 0;
            character->on_ground = SDL_TRUE;
        }
    } else {
        on_ground_center = SDL_FALSE;
    }
    // Si le personnage à les pieds sur le sol côté droite et que sa vitesse verticale est positive
    if (map->tiles[y_tile_feet][x_tile_width] > 0) {
        if (character->dy > 0) {
            character->dy = 0;
            character->on_ground = SDL_TRUE;
        }
    } else {
        on_ground_left = SDL_FALSE;
    }
    // Si le personnage n'est pas sur le sol côté gauche et côté droit alors il n'est pas sur le sol
    if (on_ground_right == SDL_FALSE && on_ground_center == SDL_FALSE && on_ground_left == SDL_FALSE) {
        character->on_ground = SDL_FALSE;
    }
    // Si le centre du personnage rentre dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_center][x_tile_right] > 0) {
        if (character->dx > 0) {
            character->dx = 0;
        }
    }
    // Si les genoux du personnage rentrent dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_knee][x_tile_right] > 0) {
        if (character->dx > 0) {
            character->dx = 0;
        }
    }
    // Si le coup du personnage rentre dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_neck][x_tile_right] > 0) {
        if (character->dx > 0) {
            character->dx = 0;
        }
    }
    // Si le centre du personnage rentre dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_center][x_tile_left] > 0) {
        if (character->dx < 0) {
            character->dx = 0;
        }
    }
    // Si les genoux du personnage rentrent dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_knee][x_tile_left] > 0) {
        if (character->dx < 0) {
            character->dx = 0;
        }
    }
    // Si le coup du personnage rentre dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[y_tile_neck][x_tile_left] > 0) {
        if (character->dx < 0) {
            character->dx = 0;
        }
    }
    // Si la tête côté droit du personnage alors on annule sa vitesse verticale
    if (map->tiles[y_tile_head][x_tile] > 0) {
        if (character->dy < 0) {
            character->dy = 0;
        }
    }
    // Si la tête côté gauche du personnage rentre dans le mur de gauche alors on annule sa vitesse verticale
    if (map->tiles[y_tile_head][x_tile_width] > 0) {
        if (character->dy < 0) {
            character->dy = 0;
        }
    }
    // Si le personnage est en dehors de la map en sortant par le bas (c'est à dire tombé dans un trou) alors on annule sa vitesse vertical
    if (character->y > map->height * tile_height) {
        if (character->dy < 0) {
            character->dy = 0;
        }
    }
    // Si le personnage est en dehors de la map par la droite alors on annule sa vitesse horizontal
    if (character->x > map->width * tile_width) {
        if (character->dx > 0) {
            character->dx = 0;
        }
    }
    // Si le personnage est en dehors de la map par la gauche alors on annule sa vitesse horizontal
    if (character->x < 0) {
        if (character->dx < 0) {
            character->dx = 0;
        }
    }
    // Le personnage peut sortir par le haut de la map car la gravité va le ramener vers le bas
}

void create_camera(Camera *camera, int x, int y, int width, int height) {
    // Crée la camera
    camera->x = x;
    camera->y = y;
    camera->width = width;
    camera->height = height;
    camera->fps = 0;
    camera->avg_fps = 0;
    // Utilisation de show_camera comme un debug mode comme sur minecraft pour les trucs que t'as besoin de print tous les tours de boucle
    camera->show_fps = SDL_FALSE;
    camera->pattern_generated_history[0] = -1;
    for (int i = 1; i < 100; i++) {
        camera->pattern_generated_history[i] = -1;
    }
}

void move_camera(Camera *camera, Character *character, Map *map) {
    // Déplace la camera par rapport au personnage
    int tile_width = SCREEN_WIDTH / camera->width;
    // Si le personnage est à gauche de l'écran alors la camera est en x est à 0
    int pixel_width = ((camera->width * tile_width) / 2);  //+ (character->width / 2);
    if (character->x < pixel_width - (character->width / 2)) {
        camera->x = 0;
    }
        // Si le personnage est à droite de l'écran alors la camera est à la fin de la map
    else if (character->x + pixel_width + (character->width / 2) > map->width * tile_width) {
        // On ajoute une map à droite
        // Map *pattern = create_map("pattern.txt");
        // On appelle generated_pattern qui sera la fonction qui donne le prochain pattern à mettre à droite
        Map *pattern = generated_pattern(camera, character, map);
        // Si la map n'est pas pleine alors on ajoute le pattern à droite de la map
        if (map->full == SDL_FALSE) {
            add_right_pattern_to_map(pattern, map);
            camera->x = character->x - pixel_width + (character->width / 2);
        } else {
            // Sinon on force la camera à être à la fin de la map
            camera->x = map->width * tile_width - (camera->width * tile_width);
            free(pattern);
        }
        // camera->x = map->width * tile_width - SCREEN_WIDTH;
    }
        // Sinon la camera est centré en x par rapport au personnage
    else {
        camera->x = character->x - pixel_width + (character->width / 2);
    }
    int tile_height = SCREEN_HEIGHT / camera->height;
    // Si le personnage est en haut de l'écran alors la camera est en y est à 0
    int pixel_height = ((camera->height * tile_height) / 2);  //+ (character->height / 2);
    if (character->y < pixel_height - (character->height / 2)) {
        camera->y = 0;
    }
        // Si le personnage est en bas de l'écran alors la camera est en bas de la map
    else if (character->y + pixel_height + (character->height / 2) > map->height * tile_height) {
        camera->y = map->height * tile_height - camera->height * tile_height;
    }
        // Sinon la camera est centré en y par rapport au personnage
    else {
        camera->y = character->y - pixel_height + (character->height / 2);
    }
}

void add_right_pattern_to_map(Map *pattern, Map *map) {
    // Si la map est pleine alors on ajoute une autre map à droite qui est la dernière
    if (MAX_TILES < map->width + pattern->width) {
        map->full = SDL_TRUE;
        printf("map is full\n");
        free(pattern);
        Map *last_pattern = create_map("Patterns/last_pattern.txt");
        // Si la map est complétement pleine alors on ne fait rien (cas si le dernier pattern est exactement la taille de la map)
        if (MAX_TILES < map->width + last_pattern->width) {
            return;
        } else {
            // Sinon on ajoute le dernier pattern à droite de la map
            // Si la hauteur du dernier pattern est plis petite que la hauteur de la map alors on complète le dernier pattern avec des 0
            if (map->height > last_pattern->height) {
                for (int i = last_pattern->height; i < map->height; i++) {
                    for (int j = 0; j < last_pattern->width; j++) {
                        last_pattern->tiles[i][j] = 0;
                    }
                }
                last_pattern->height = map->height;
            }
            // On ajoute le dernier pattern à droite de la map
            for (int i = 0; i < map->height; i++) {
                for (int j = 0; j < last_pattern->width; j++) {
                    map->tiles[i][j + map->width] = last_pattern->tiles[i][j];
                }
            }
            map->width += last_pattern->width;
            // On affiche la map dans la console quand la map est pleine
            print_map(map);
            free(last_pattern);
        }
        // Sinon on ajoute le pattern à droite de la map
    } else {
        // Si la hauteur du pattern est plis petite que la hauteur de la map alors on complète le pattern avec des 0
        if (map->height > pattern->height) {
            for (int i = pattern->height; i < map->height; i++) {
                for (int j = 0; j < pattern->width; j++) {
                    pattern->tiles[i][j] = 0;
                }
            }
            pattern->height = map->height;
        }
        // On ajoute le pattern à droite de la map
        for (int i = 0; i < map->height; i++) {
            for (int j = 0; j < pattern->width; j++) {
                map->tiles[i][j + map->width] = pattern->tiles[i][j];
            }
        }
        map->width += pattern->width;
        free(pattern);
    }
}
