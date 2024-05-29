#include "../include/init.h"

int init(SDL_Window **window, SDL_Renderer **renderer, int w, int h) {
    if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return -1;
    }
    // Crée la fenêtre et le rendu
    if (0 != SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_RESIZABLE, window, renderer)) {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        return -1;
    }
    return 0;
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }
    if (0 != TTF_Init()) {
        fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
        return -1;
    }
}
long long getCurrentTimeInMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}

double load_settings(char *settings_path){
    FILE *file = fopen(settings_path, "r");
    double time;
    if (file != NULL) {
        fscanf(file, "%lf", &time);
        fclose(file);
    } else {
        time = 0; // Default value if file doesn't exist
    }
    return time;
}

void save_time(char *settings_path, double time) {
    FILE *file = fopen(settings_path, "w");
    if (file != NULL) {
        fprintf(file, "%lf\n", time);
        fclose(file);
    }
}

RdmTexture *load_from_dir(char *dir_path, SDL_Renderer *renderer) {
    // initialisation de rdmtexture
    RdmTexture *Rdmtexture = malloc(sizeof(RdmTexture));
    for (int i = 0; i < 10; i++) {
        Rdmtexture->Data[i] = NULL;
    }
    Rdmtexture->size = 1;
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
            printf("empty dir on %s\n", dir_path);
            Rdmtexture->Data[0] = loadImage("Textures/no_texture.jpg", renderer);
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
        texture->key_suggestion[i] = NULL;
        texture->bouttons[i] = NULL;
    }
    for (int i = 0; i < 11; i++) {
        texture->timer[i] = NULL;
        texture->background[i] = NULL;
    }

    // Liste des noms des images de la map (collisables) avec "END" A la fin
    char *list_strings[] = {
            "Textures/Terrain/nuage",
            "Textures/Terrain/sol",
            "Textures/Terrain/ss1",
            "Textures/Terrain/ss2",
            "Textures/Terrain/marron_cave.png",
            "Textures/Terrain/sol_gauche.png",
            "Textures/Terrain/ss1_gauche.png",
            "Textures/Terrain/ss2_gauche.png",
            "Textures/Terrain/ss3_gauche.png",
            "Textures/Terrain/sol_droite.png",
            "Textures/Terrain/ss1_droite.png",
            "Textures/Terrain/ss2_droite.png",
            "Textures/Terrain/ss3_droite.png",
            "Textures/Terrain/sol_gauche_p_ss1",
            "Textures/Terrain/ss1_gauche_p.png",
            "Textures/Terrain/ss2_gauche_p.png",
            "Textures/Terrain/ss3_gauche_p.png",
            "Textures/Terrain/sol_droite_p_ss1",
            "Textures/Terrain/ss1_droite_p.png",
            "Textures/Terrain/ss2_droite_p.png",
            "Textures/Terrain/ss3_droite_p.png",
            "Textures/Terrain/sol_gauche_p2.png",
            "Textures/Terrain/ss1_gauche_p.png",
            "Textures/Terrain/ss2_gauche_p.png",
            "Textures/Terrain/ss3_gauche_p.png",
            "Textures/Terrain/sol_droite_p2.png",
            "Textures/Terrain/ss1_droite_p.png",
            "Textures/Terrain/ss2_droite_p.png",
            "Textures/Terrain/ss3_droite_p.png",
            "Textures/Terrain/nuage_bas",
            "Textures/Terrain/nuage_g.png",
            "Textures/Terrain/nuage_ge.png",
            "Textures/Terrain/nuage_gi.png",
            "Textures/Terrain/nuage_d.png",
            "Textures/Terrain/nuage_di.png",
            "Textures/Terrain/nuage_de.png",
            "Textures/Terrain/nuage_seul_bas.png",
            "Textures/Terrain/nuage_top",
            "Textures/Terrain/nuage_gt.png",
            "Textures/Terrain/nuage_get.png",
            "Textures/Terrain/nuage_git.png",
            "Textures/Terrain/nuage_dt.png",
            "Textures/Terrain/nuage_dit.png",
            "Textures/Terrain/nuage_det.png",
            "Textures/Terrain/nuage_seul_haut.png",
            "Textures/Terrain/wall_1",
            "Textures/Terrain/wall_top",
            "Textures/Terrain/wall_mid",
            "Textures/Terrain/wall_bot",
            "Textures/Terrain/gate/gate.png",
            "Textures/Terrain/gate/gate_top.png",
            "Textures/Terrain/sol_seul.png",
            "Textures/Terrain/roof",
            "Textures/Terrain/roof_gauche.png",
            "Textures/Terrain/roof_droite.png",
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
            "wall.png",
            "walk_crouch.png",
            "crouch.png",
            "slide.png",
            "character_cooldown.png",
            "character_run_cooldown.png",
            "jump_cooldown.png",
            "jump_right_cooldown.png",
            "jump_right_fall_cooldown.png",
            "wall_cooldown.png",
            "walk_crouch_cooldown.png",
            "crouch_cooldown.png",
            "slide_cooldown.png",
            "END"};
    // Chargement des textures du personnage
    for (int i = 0; strcmp(imageNames[i], "END"); i++) {
        char imagePath[100];
        addcat(imagePath, "Textures/korigan", imageNames[i]);
        texture->main_character[i] = loadImage(imagePath, renderer);
    }
    // Initialisation du tableau de textures du personnage à NULL pour éviter les problèmes de mémoire
    for (int i = 0; i < 100; i++)
        texture->trail_frames[i] = NULL;
    // Liste des noms des images du personnage
    char *trailNames[] = {
            "dash/dash_000.png",
            "dash/dash_001.png",
            "dash/dash_002.png",
            "dash/dash_003.png",
            "dash/dash_004.png",
            "dash/dash_005.png",
            "dash/dash_006.png",
            "dash/dash_007.png",
            "dash/dash_008.png",
            "dash/dash_009.png",
            "END"};
    // Chargement des textures du personnage
    for (int i = 0; strcmp(trailNames[i], "END"); i++) {
        char imagePath[100];
        addcat(imagePath, "Textures/korigan", trailNames[i]);
        texture->trail_frames[i] = loadImage(imagePath, renderer);
        // printf("%s\n", imagePath);
    }
    // Liste des noms des images de suggestion de touche
    char *key_images[] = {
            "q.png",
            "d.png",
            "space.png",
            "s.png",
            "e.png",
            "END"};
    // Chargement des textures de suggestions
    for (int i = 0; strcmp(key_images[i], "END"); i++) {
        char imagePath[100];
        addcat(imagePath, "Textures/key_suggestion", key_images[i]);
        texture->key_suggestion[i] = loadImage(imagePath, renderer);
    }
    // Liste des noms des images de boutons
    char *bouttons_images[] = {
            "start.png",
            "tutoriel.png",
            "restart.png",
            "menu.png",
            "END"};
    // Chargement des textures de bouttons
    for (int i = 0; strcmp(bouttons_images[i], "END"); i++) {
        char imagePath[100];
        addcat(imagePath, "Textures/bouttons", bouttons_images[i]);
        texture->bouttons[i] = loadImage(imagePath, renderer);
    }

    char *background_images[] = {
            "blured_foreground.png",
            "blured_midground.png",
            "background.png",
            "homescreen.png",
            "END"};
    // Chargement des textures de bouttons
    for (int i = 0; strcmp(background_images[i], "END"); i++) {
        char imagePath[100];
        // printf("i : %d\n", i);
        addcat(imagePath, "Textures/fond", background_images[i]);
        texture->background[i] = loadImage(imagePath, renderer);
    }


    // Crée une police de caractère avec le fichier arial.ttf de taille 28
    texture->font = TTF_OpenFont("Fonts/Pixel_Sans_Serif.ttf", 28);
    if (texture->font == NULL)
        fprintf(stderr, "Erreur TTF_OpenFont : %s", TTF_GetError());
    //chargement des nombres pour le timer
    for (int i = 0; i < 10; i++) {
        char num[2];
        num[0] = i + '0'; // Convertir le chiffre en caractère
        num[1] = '\0'; // Terminer la chaîne de caractères avec le caractère nul

        TTF_SetFontStyle(texture->font, TTF_STYLE_NORMAL);
        SDL_Color color = {192, 190, 193, 255};

        // Créer la surface à partir du texte
        SDL_Surface *surface = TTF_RenderText_Solid(texture->font, num, color);
        SDL_Texture *num_texture = SDL_CreateTextureFromSurface(renderer, surface);
        texture->timer[i] = num_texture;
        SDL_FreeSurface(surface);
    }
    TTF_SetFontStyle(texture->font, TTF_STYLE_NORMAL);
    SDL_Color color = {192, 190, 193, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(texture->font, ".", color);
    SDL_Texture *num_texture = SDL_CreateTextureFromSurface(renderer, surface);
    texture->timer[10] = num_texture;
    SDL_FreeSurface(surface);
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
        if (NULL != texture->trail_frames[i]) {
            SDL_DestroyTexture(texture->trail_frames[i]);
        }
        if (NULL != texture->key_suggestion[i]) {
            SDL_DestroyTexture(texture->key_suggestion[i]);
        }
        if (NULL != texture->bouttons[i]) {
            SDL_DestroyTexture(texture->bouttons[i]);
        }
        for (int i = 0; i < 11; i++) {
            SDL_DestroyTexture(texture->timer[i]);
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
        fprintf(stderr, "Erreur IMG_Load : %s\n", SDL_GetError());
        tmp = IMG_Load("Textures/no_texture.jpg");
        if (NULL == tmp) {
            fprintf(stderr, "Erreur IMG_Load even no texture can't be loaded: %s\n", SDL_GetError());
            return NULL;
        }
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
                // printf("(texture->collision[num_texture]->size) : %d num_texture : %d\n", texture->collision[num_texture]->size, num_texture);
                int num_image = (k % 10) % (texture->collision[num_texture]->size);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, texture->collision[num_texture]->Data[num_image], NULL,
                                   &dst) < 0) {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s num_texture : %d num_image : %d \n", SDL_GetError(), num_texture, num_image);
                }
            } else if (map->tiles[i][j].type <= -20) {
                // Si la case contient un nombre négatif on affiche la texture correspondante (transparente)
                int num_texture = k / 10;
                int num_image = (k % 10) % (texture->transparent[num_texture]->size);
                SDL_Rect dst = {j * tile_width - camera->x, i * tile_height - camera->y, tile_width, tile_height};
                if (SDL_RenderCopy(renderer, texture->transparent[num_texture]->Data[num_image], NULL,
                                   &dst) < 0) {
                    fprintf(stderr, "Erreur SDL_RenderCopy : %s num_texture : %d num_image : %d \n", SDL_GetError(), num_texture, num_image);
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
    character->wall_jump_right = SDL_FALSE;
    character->wall_jump_left = SDL_FALSE;
    character->next_map = SDL_FALSE;
    character->on_portal = SDL_FALSE;
    character->dash = init_dash();
    character->slide = init_slide();
    character->key_suggestion=SDLK_F15;
    character->text_suggestion = NULL;
    character->timer = 543.21;
    character->num_map=1;
    return character;
}

void reset_character(Character *character,Map *map, int tile_width, int tile_height){
    character->x = map->tile_start_x * tile_width;
    character->y = map->tile_start_y * tile_height;
    character->width = character->original_width;
    character->height = character->original_height;
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
    character->wall_jump_right = SDL_FALSE;
    character->wall_jump_left = SDL_FALSE;
    character->next_map = SDL_FALSE;
    character->on_portal = SDL_FALSE;
    free(character->dash);
    free(character->slide);
    character->dash = init_dash();
    character->slide = init_slide();
}

void print_character(Character *character) {
    // Affiche les informations du personnage dans la console
    printf("x: %d, y: %d, dx: %d, dy: %d \n width: %d, height: %d, speed: %f, up: %d, down: %d, left: %d, right: %d, alive: %d \n on_ground: %d, wall_right: %d, wall_left: %d\n wall_jump_right: %d, wall_jump_left: %d",
           character->x, character->y, character->dx, character->dy, character->width, character->height,
           character->speed,
           character->up, character->down, character->left, character->right, character->alive, character->on_ground,
           character->wall_right, character->wall_left, character->wall_jump_right, character->wall_jump_left);
    // Affiche les informations du dash
    print_dash(character->dash);
}

char *addcat(char *result, char *path, char *name) {
    strcpy(result, path);
    strcat(result, "/");
    strcat(result, name);
    return result;
}
