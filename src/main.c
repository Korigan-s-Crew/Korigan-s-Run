#include "../include/main.h"

#include <time.h>

#include "../include/controls.h"
#include "../include/init.h"
#include "../include/map.h"
#include "../include/movement.h"
#include "../include/procedural_generation.h"

int SCREEN_WIDTH = 1300;
int SCREEN_HEIGHT = 700;

int main(void) {
    // start a timer
    long long start = getCurrentTimeInMicroseconds();
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
    Camera camera;
    // Crée la caméra en fonction de la taille de la fenêtre
    int camera_width = (int)(SCREEN_WIDTH / 100);
    int camera_height = (int)(SCREEN_HEIGHT / 100);
    // Crée la caméra en position 0, 0 et de taille camera_width, camera_height
    create_camera(&camera, 0, 0, camera_width, camera_height);
    int tile_width = SCREEN_WIDTH / camera_width;
    int tile_height = SCREEN_HEIGHT / camera_height;
	// Initialise le graphe
	Pattern pat = pattern_initialisation();
    create_map_txt(pat, 5, "test.txt");
	Map *map = create_map("test.txt", tile_width, tile_height);
	// Map *map = create_map("map.txt", tile_width, tile_height);
    int nb_map = 1;
    // printf("tile_width: %d, tile_height: %d\n", tile_width, tile_height);
    Character *character = create_character(map->tile_start_x * tile_width, map->tile_start_y * tile_height,
                                            (int)(tile_width * 0.9), (int)(tile_height * 1.5), 2, renderer);
    print_character(character);
    // DEBUG MAP
    print_map(map);
    // Initialise la seed pour le random
    srand(time(NULL));  // srand(8675612346585);
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
	printf("init done in %lld\n", getCurrentTimeInMicroseconds() - start);

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
                        map->tile_width = tile_width;
                        map->tile_height = tile_height;
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
                        collision(character, map);
                        // Affiche la map et le personnage dans la fenêtre avec la nouvelle taille
                        draw(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
                    }
                    break;
                    // Si l'événement est de type SDL_KEYDOWN (appui sur une touche)
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == controls->down) {
//                        printf("down\n");
                        character->down = SDL_TRUE;
                    } else if (event.key.keysym.sym == controls->left) {
//                        printf("left\n");
                        character->left = SDL_TRUE;
                    } else if (event.key.keysym.sym == controls->right) {
//                        printf("right\n");
                        character->right = SDL_TRUE;
                    } else {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
//                                printf("space\n");
                                character->up = SDL_TRUE;
                                break;
                            case SDLK_ESCAPE:
                                printf("escape\n");
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
                            case SDLK_KP_0:
                                character->alive = SDL_FALSE;
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
//                        if (character->dy < 0) {
//                            character->dy = 0;
//                        }
                        break;
                    } else if (event.key.keysym.sym == controls->left) {
                        character->left = SDL_FALSE;
//                        character->dx = 0;
                        break;
                    } else if (event.key.keysym.sym == controls->right) {
                        character->right = SDL_FALSE;
//                        character->dx = 0;
                        break;
                    } else {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
                                character->up = SDL_FALSE;
//                                if (character->dy < 0) {
//                                    character->dy = 0;
//                                }
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
            if (character->alive == SDL_FALSE) {  // ide warning is an error
                character->x = map->tile_start_x * tile_width;
                character->y = map->tile_start_y * tile_height;
                character->alive = SDL_TRUE;
            }
            if (character->next_map == SDL_TRUE) {
				create_map_txt(pat, 5, "test.txt");
				map = change_map(map, "test.txt", character, &camera, map->tile_width, map->tile_height);
			}

            // if (character->next_map == SDL_TRUE && nb_map == 1) {
			// 	map = change_map(map, "map2.txt", character, &camera, map->tile_width, map->tile_height);
			// 	nb_map++;
            // }
            // if (character->next_map == SDL_TRUE && nb_map == 2) {
			// 	map = change_map(map, "map.txt", character, &camera, map->tile_width, map->tile_height);
			// 	nb_map--;
            // }
            // Applique la gravité au personnage
            gravity(character);
            // Applique le mouvement au personnage
            mouvement(map, character);
            // Affiche la map et le personnage dans la fenêtre
            // draw(renderer, bleu, texture, map, tile_width, tile_height, character, &camera, graphe);
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
    free(character->slide);
    free(character);
    free_texture(texture);
	free_pattern(pat);
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
    return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}

RdmTexture *load_from_dir(char *dir_path, SDL_Renderer *renderer) {
    // initialisation de rdmtexture
    RdmTexture *Rdmtexture = malloc(sizeof(RdmTexture));
    for (int i = 0; i < 10; i++) {
        Rdmtexture->Data[i] = NULL;
    }
    Rdmtexture->size = 0;
    // Cas ou le path pointe sur un png
    if (strstr(dir_path, ".png") != NULL && strstr(dir_path, ".png") == dir_path + strlen(dir_path) - 4) {
        Rdmtexture->Data[0] = loadImage(dir_path, renderer);
        Rdmtexture->size = 1;
    }
    // Cas dossier
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
                // printf("%s\n",str);
                Rdmtexture->Data[i] = loadImage(path, renderer);
                Rdmtexture->size = i + 1;
                i++;
                if (i >= 10)
                    break;  // to prevent buffer overflow
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
    char *list_strings[] = {
        "Textures/Terrain/nuage",
        "Textures/Terrain/sol",  // 19
        "Textures/Terrain/ss1",  // 29
        "Textures/Terrain/ss2",  // 39
        "Textures/Terrain/marron_cave.png",
        "Textures/Terrain/sol_gauche.png",
        "Textures/Terrain/ss1_gauche.png",
        "Textures/Terrain/ss2_gauche.png",
        "Textures/Terrain/ss3_gauche.png",  // 80-89
        "Textures/Terrain/sol_droite.png",
        "Textures/Terrain/ss1_droite.png",
        "Textures/Terrain/ss2_droite.png",
        "Textures/Terrain/ss3_droite.png",  // 120-129
        "Textures/Terrain/sol_gauche_p_ss1",
        "Textures/Terrain/ss1_gauche_p.png",
        "Textures/Terrain/ss2_gauche_p.png",
        "Textures/Terrain/ss3_gauche_p.png",  // 160-169
        "Textures/Terrain/sol_droite_p_ss1",
        "Textures/Terrain/ss1_droite_p.png",
        "Textures/Terrain/ss2_droite_p.png",
        "Textures/Terrain/ss3_droite_p.png",   // 200-209
        "Textures/Terrain/nuage/nuage.png",    // 210-219
        "Textures/Terrain/gate/gate.png",      // 220-229
        "Textures/Terrain/gate/gate_top.png",  // 230-239
        "END"};
    // Charge les textures des images de la map (collisables)
    for (int i = 0; strcmp(list_strings[i], "END"); i++) {
        texture->collision[i] = load_from_dir(list_strings[i], renderer);
    }

    // Liste des noms des images de la map (transparentes)
    char *list_strings_bis[] = {
        "Textures/Terrain/nuage",
        "Textures/Terrain/nuage",
        "Textures/Terrain/herbe",
        "Textures/Terrain/herbe_gauche.png",
        "Textures/Terrain/herbe_droite.png",
        "Textures/transparents/end_portal.png",
        "END"};
    // Charge les textures des images de la map (transparentes)
    for (int i = 0; strcmp(list_strings_bis[i], "END"); i++) {
        texture->transparent[i] = load_from_dir(list_strings_bis[i], renderer);
    }
    // Initialisation du tableau de textures du personnage à NULL pour éviter les problèmes de mémoire
    for (int i = 0; i < 100; i++)
        texture->main_character[i] = NULL;
    // Liste des noms des images du personnage
    char *imageNames[] = {
        "character.png",
        "character_run.png",
        "jump.png",
        "jump_right.png",
        "jump_right_fall.png",
        "character_cooldown.png",
        "character_run_cooldown.png",
        "jump_cooldown.png",
        "jump_right_cooldown.png",
        "jump_right_fall_cooldown.png",
        "END"};
    // Chargement des textures du personnage
    for (int i = 0; strcmp(imageNames[i], "END"); i++) {
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
                if (NULL != texture->collision[i]->Data[j]) {
                    SDL_DestroyTexture(texture->collision[i]->Data[j]);
                }
            }
            free(texture->collision[i]);
        }
        if (NULL != texture->transparent[i]) {
            for (int j = 0; j < 10; j++) {
                if (NULL != texture->transparent[i]->Data[j]) {
                    SDL_DestroyTexture(texture->transparent[i]->Data[j]);
                }
            }
            free(texture->transparent[i]);
        }
        if (NULL != texture->main_character[i]) {
            SDL_DestroyTexture(texture->main_character[i]);
        }
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

void print_map(Map *map) {
    // Affiche le tableau dans la console
    printf("width: %d, height: %d\n", map->width, map->height);
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            // Si la case contient un nombre positif on affiche un espace avant le nombre
            if (map->tiles[i][j].type >= 0)
                printf(" ");
            printf("%d", map->tiles[i][j].type);
        }
        printf("\n");
    }
    //    for (int i = 0; i < map->height; i++) {
    //        for (int j = 0; j < map->width; j++) {
    //            // Si la case contient un nombre positif on affiche un espace avant le nombre
    ////            if (map->tiles[i][j].type >= 0)
    ////                printf(" ");
    //            printf(" %d ", map->tiles[i][j].collision.up);
    //        }
    //        printf("\n");
    //    }
}

void draw_map(SDL_Renderer *renderer, Texture *texture, Map *map, int tile_width, int tile_height, Camera *camera) {
    // Affiche la map dans la fenêtre
    for (int i = (camera->y / tile_height); i < (camera->y / tile_height) + camera->height + 1; i++) {
        for (int j = (camera->x / tile_width); j < (camera->x / tile_width) + camera->width + 1; j++) {
            int k = abs(map->tiles[i][j].type);
            if (map->tiles[i][j].type >= 10) {
                // Si la case contient un nombre positif on affiche la texture correspondante (collisonable)
                int num_texture = k / 10;
                int num_image = (k % 10) % (texture->collision[num_texture]->size);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, texture->collision[num_texture]->Data[num_image], NULL,
                                   &dst) < 0) {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            } else if (map->tiles[i][j].type <= -20) {
                // Si la case contient un nombre négatif on affiche la texture correspondante (transparente)
                int num_texture = k / 10;
                int num_image = (k % 10) % (texture->transparent[num_texture]->size);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, texture->transparent[num_texture]->Data[num_image], NULL,
                                   &dst) < 0) {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s \n", SDL_GetError());
                }
            }
        }
    }
}

Character *create_character(int x, int y, int width, int height, int speed, SDL_Renderer *renderer) {
    // Crée un personnage
    Character *character = malloc(sizeof(Character));
    if (NULL == character) {
        fprintf(stderr, "Error allocating character memory");
        return NULL;
    }
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
    character->just_landed = SDL_FALSE;
    character->crouch = SDL_FALSE;
    character->on_ground = SDL_FALSE;
    character->wall_right = SDL_FALSE;
    character->wall_left = SDL_FALSE;
    character->next_map = SDL_FALSE;
    character->dash = init_dash();
    character->slide = init_slide();
    return character;
}

void print_character(Character *character) {
    // Affiche les informations du personnage dans la console
    printf("x: %d, y: %d, dx: %d, dy: %d, width: %d, height: %d, speed: %f, up: %d, down: %d, left: %d, right: %d, alive: %d, on_ground: %d, wall_right: %d, wall_left: %d\n",
           character->x, character->y, character->dx, character->dy, character->width, character->height,
           character->speed,
           character->up, character->down, character->left, character->right, character->alive, character->on_ground,
           character->wall_right, character->wall_left);
    // Affiche les informations du dash
    print_dash(character->dash);
}

char *addcat(char *result, char *path, char *name) {
    strcpy(result, path);
    strcat(result, "/");
    strcat(result, name);
    return result;
}

void draw_character_offset(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera, SDL_Rect dst,
                           int offset) {
    if (character->right == SDL_TRUE && character->left == SDL_TRUE && character->on_ground == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
    } else if (character->right == SDL_TRUE && character->on_ground == SDL_TRUE && character->dx != 0) {
        draw_character_animation(renderer, character, texture, &dst, camera, 1 + offset, character->speed, 7);
    } else if (character->left == SDL_TRUE && character->on_ground == SDL_TRUE && character->dx != 0) {
        draw_character_animationEx(renderer, character, texture, &dst, camera, 1 + offset, SDL_FLIP_HORIZONTAL, character->speed,
                                   7);
    } else if (character->right == SDL_TRUE && character->dx != 0) {
        if (character->dy > 0 && character->on_ground == SDL_FALSE) {
            SDL_RenderCopy(renderer, texture->main_character[4 + offset], NULL, &dst);
        } else {
            SDL_RenderCopy(renderer, texture->main_character[3 + offset], NULL, &dst);
        }
    } else if (character->left == SDL_TRUE && character->dx != 0) {
        if (character->dy > 0 && character->on_ground == SDL_FALSE) {
            SDL_RenderCopyEx(renderer, texture->main_character[4 + offset], NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
        } else {
            SDL_RenderCopyEx(renderer, texture->main_character[3 + offset], NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    } else if (character->on_ground == SDL_FALSE) {
        SDL_RenderCopy(renderer, texture->main_character[2 + offset], NULL, &dst);
    } else if (character->down == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
    } else if (character->dash->duration > 0) {
        SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
    } else if (character->on_ground == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
    } else if (character->up == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[2 + offset], NULL, &dst);
    } else {
        SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
    }
}

void draw_character(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera) {
    // Affiche le personnage dans la fenêtre
    SDL_Rect dst = {character->x - camera->x, character->y - camera->y, character->width, character->height};
    if (character->dash->cooldown > 0) {
        // for (int i = 0; i < 5; i++) {
        //     SDL_SetTextureColorMod(texture->main_character[i], 255, 0, 0);
        //     SDL_SetTextureAlphaMod(texture->main_character[i], 255);
        // }
        draw_character_offset(renderer, character, texture, camera, dst, 5);
    } else {
        // for (int i = 0; i < 5; i++) {
        //     SDL_SetTextureColorMod(texture->main_character[i], 255, 255, 255);
        //     SDL_SetTextureAlphaMod(texture->main_character[i], 255);
        // }
        draw_character_offset(renderer, character, texture, camera, dst, 0);
    }

}

void draw_character_animation(SDL_Renderer *renderer, Character *character, Texture *texture, SDL_Rect *dst, Camera *camera,
                              int index, float speed, int nb_frame) {
    // Affiche une animation du personnage dans la fenêtre (déplacement vers la droite)
    SDL_Rect src = {0, 0, 0, 0};
    SDL_QueryTexture(texture->main_character[index], NULL, NULL, &src.w, &src.h);
    src.w /= nb_frame;
    for (int i = 0; i < nb_frame; i++) {
        if (camera->fps % (int)(MAX_FPS / speed) < (MAX_FPS * (i + 1)) / (nb_frame * speed)) {
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
        if (camera->fps % (int)(MAX_FPS / speed) < (MAX_FPS * (i + 1)) / (nb_frame * speed)) {
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
    int tile_height = SCREEN_HEIGHT / camera->height;
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
