#include "../include/main.h"

#include <time.h>

#include "../include/controls.h"
#include "../include/init.h"
#include "../include/map.h"
#include "../include/movement.h"
#include "../include/procedural_generation.h"
#include "../include/dash_effect.h"
#include "../include/animation.h"

int SCREEN_WIDTH = 1300;
int SCREEN_HEIGHT = 700;

int main(void) {
    // start a timer //bullshit
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
    // Initialise la variable qui contient les contrôles
    Controls *controls = init_controls();
    // Crée la camera
    Camera camera;
    // Crée la caméra en fonction de la taille de la fenêtre
    int camera_width = (int)(SCREEN_WIDTH / 100);
    int camera_height = (int)(SCREEN_HEIGHT / 100);
    // Crée la caméra en position 0, 0 et de taille camera_width, camera_height
    create_camera(&camera, 0, 0, camera_width, camera_height);
    int tile_width = SCREEN_WIDTH / camera_width;
    int tile_height = SCREEN_HEIGHT / camera_height;
	// Initialise la seed pour le random
	srand(time(NULL));	// srand(8675612346585);
	// Crée la map
    Pattern pat = pattern_initialisation();
    create_map_txt(pat, "test.txt");
    Map *map = create_map("test.txt", tile_width, tile_height);

    // printf("tile_width: %d, tile_height: %d\n", tile_width, tile_height);
    Character *character = create_character(map->tile_start_x * tile_width, map->tile_start_y * tile_height,
                                            (int)(tile_width * 0.9), (int)(tile_height * 1.5), 2, renderer);
    //réccupere les coordonnées de la souris
    Mouse *mouse = malloc(sizeof(Mouse));
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    mouse->x = mouseX;
    mouse->y = mouseY;
    mouse->on_boutton = SDL_FALSE;
    mouse->num_boutton=0;
    // DEBUG Character
    //print_character(character);
    // DEBUG MAP
    // print_map(map);
    // Chargement des textures
    Texture *texture = create_texture(renderer);
    init_character_animations_buffer();
    // Affiche la première image
    //draw_ingame(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
    draw_homepage(renderer, bleu, texture, &camera, mouse);
    // printf("main\n");
    double best_time= load_settings("settings.txt");
    //  Initialise la variable qui contient le dernier temps
    long long last_time = 0;
    long long last_time_fps = 0;
    long long last_time_sec = 0;
    //  Initialise le tutoriel
    int tutorial_step = 0;
    SDL_Keycode key_for_tuto[]={
            SDLK_F15,
            SDLK_q,
            SDLK_d,
            SDLK_SPACE,
            SDLK_s,
            SDLK_F14,
            SDLK_F14,
            SDLK_F14,
            SDLK_LSHIFT,
            SDLK_F14,
            SDLK_F14,
            SDLK_F14,
            SDLK_F14,
            SDLK_e,
            SDLK_F15
    };
    char *text_for_tuto[50]={
            "",
            "appuyez sur q et d pour vous deplacer",
            "appuyez sur q et d pour vous deplacer",
            "appuyez sur espace pour sauter",
            "appuyez sur s pour vous baisser",
            "maintenant, sautez contre un mur",
            "",
            "appuyez sur 5 puis 8 pour activer le dash",
            "appuyez sur shift pour faire un dash",
            "vous pouvez dash en sautant pour passer ce mur",
            "appuyez sur s en l'air pour slide",
            "apres un slide, vous pouvez sauter plus haut",
            "allez sur la porte",
            "appuyez sur e",
            "",
            "END"
    };
    SDL_Texture *text_for_tuto_texture[50];
    // Chargement des textures de suggestions
    for (int i = 0; strcmp(text_for_tuto[i], "END"); i++) {
        TTF_SetFontStyle(texture->font, TTF_STYLE_NORMAL);
        SDL_Color color = {192, 190, 193, 255};
        if (strcmp(text_for_tuto[i], "") != 0) {
            // Créer la surface à partir du texte
            SDL_Surface *surface = TTF_RenderText_Solid(texture->font, text_for_tuto[i], color);
            if (surface == NULL) {
                printf("Erreur lors de la création de la surface pour le texte %d : %s\n", i, SDL_GetError());
                text_for_tuto_texture[i] = NULL;
                break;
            }
            SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (text_texture == NULL) {
                printf("Erreur lors de la création de la texture pour le texte %d : %s\n", i, SDL_GetError());
                text_for_tuto_texture[i] = NULL;
                break;
            }
            text_for_tuto_texture[i] = text_texture;
            SDL_FreeSurface(surface);
        } else {
            text_for_tuto_texture[i] = NULL;
        }
    }

#define next_step_tuto() ( \
(tutorial_step == 5 && (character->wall_jump_right==SDL_TRUE || character->wall_jump_left==SDL_TRUE)) ||                   \
(tutorial_step == 6 && character->x > 3367) ||                                                                           \
(tutorial_step == 7 && character->dash->go_up == SDL_TRUE) ||                                                            \
(tutorial_step == 9 && character->x > 4752) ||                                                                             \
(tutorial_step == 10 && character->slide->duration > 0) ||                                                                 \
(tutorial_step == 11 && character->x > 6869) || \
(tutorial_step == 12 && character->on_portal == SDL_TRUE))
    // Boucle principale
    int running = 1;
    int game_playing=0;
    double timer_start;
    int menu=1;
    printf("init done in %lld\n", getCurrentTimeInMicroseconds() - start);
    while (running==1){
        if (game_playing == 0) {
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
                            // printf("SCREEN_WIDTH: %d, SCREEN_HEIGHT: %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
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
                            //draw_ingame(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
                            if (menu==1){
                                draw_homepage(renderer, bleu, texture, &camera, mouse);
                            } else {
                                draw_endpage(renderer, bleu, texture, &camera, mouse, character, best_time);
                            }
                        }
                        break;
                        // Si l'événement est de type SDL_KEYDOWN (appui sur une touche)
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_ESCAPE:
                                printf("escape\n");
                                running = 0;
                                break;
                            case SDLK_t:
                                game_playing = 1;
                                tutorial_step = 1;
                                character->num_map=1;
                                character->key_suggestion = key_for_tuto[tutorial_step];
                                character->text_suggestion = text_for_tuto_texture[tutorial_step];
                                map = change_map(map, "map_tuto.txt", character, &camera, map->tile_width, map->tile_height);
                                camera.show_timer = SDL_FALSE;
                                break;
                            case SDLK_e:
                                game_playing = 1;
                                tutorial_step = 0;
                                character->num_map=1;
                                character->key_suggestion = SDLK_F15;
                                character->text_suggestion = NULL;
                                camera.show_timer = SDL_TRUE;
								create_map_txt(pat, "test.txt");
								map = change_map(map, "test.txt", character, &camera, map->tile_width, map->tile_height);
                                timer_start = (double)getCurrentTimeInMicroseconds() ;
                                break;

                        }
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (mouse->on_boutton == SDL_TRUE ) {
                            if (mouse->num_boutton == 0) {
                                create_map_txt(pat, "test.txt");
                                game_playing = 1;
                                tutorial_step=0;
                                character->num_map=1;
                                character->text_suggestion = NULL;
                                character->key_suggestion=SDLK_F15;
                                camera.show_timer = SDL_TRUE;
								create_map_txt(pat, "test.txt");
								map = change_map(map, "test.txt", character, &camera, map->tile_width, map->tile_height);
                                timer_start = (double)getCurrentTimeInMicroseconds() ;
                                break;
                            } else if (mouse->num_boutton == 1) {
                                if (menu == 1){
                                    game_playing = 1;
                                    tutorial_step = 1;
                                    character->num_map=1;
                                    character->key_suggestion=key_for_tuto[tutorial_step];
                                    character->text_suggestion = text_for_tuto_texture[tutorial_step];
                                    map = change_map(map, "map_tuto.txt", character, &camera, map->tile_width, map->tile_height);
                                    camera.show_timer = SDL_FALSE;
                                    break;
                                } else {
                                    game_playing = 0;
                                    menu=1;
                                    break;
                                }
                            }
                        }
                    case SDL_MOUSEMOTION:
                        // Récupérer les coordonnées de la souris
                        mouse->x = event.motion.x;
                        mouse->y = event.motion.y;
                        break;
                }
            }
            // C'est la condition qui donne le FPS
            if (getCurrentTimeInMicroseconds() - last_time_fps >= 1000000 / MAX_FPS) {
                last_time_fps = getCurrentTimeInMicroseconds();
                //draw_ingame(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
                if (menu == 1){
                    draw_homepage(renderer, bleu, texture, &camera, mouse);
                } else {
                    draw_endpage(renderer, bleu, texture, &camera, mouse, character, best_time);
                }

                camera.fps++;
            }
        }
        //boucle de jeu
        if (game_playing == 1) {
            if (tutorial_step != 0){
                if next_step_tuto() {
                    character->key_suggestion = key_for_tuto[tutorial_step + 1];
                    character->text_suggestion = text_for_tuto_texture[tutorial_step + 1];
                    tutorial_step += 1;
                    // if (key_for_tuto[tutorial_step] == SDLK_F15) {
                    //     tutorial_step = 0;
                    // }
                }
            }
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
                            // printf("SCREEN_WIDTH: %d, SCREEN_HEIGHT: %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
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
                            draw_ingame(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
                        }
                        break;
                        // Si l'événement est de type SDL_KEYDOWN (appui sur une touche)
                    case SDL_KEYDOWN:
                        if (tutorial_step !=0 && (event.key.keysym.sym == key_for_tuto[tutorial_step] || event.key.keysym.sym == SDLK_k)) {
                            character->key_suggestion = key_for_tuto[tutorial_step + 1];
                            character->text_suggestion = text_for_tuto_texture[tutorial_step + 1];
                            tutorial_step += 1;
                            // if (key_for_tuto[tutorial_step] == SDLK_F15) {
                            //     tutorial_step = 0;
                            // }
                        }
                        if (event.key.keysym.sym == SDLK_e && character->on_portal==SDL_TRUE) {
                            character->next_map=SDL_TRUE;
                        }
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
                                    game_playing = 0;
                                    menu=1;
                                    break;
                                case SDLK_p:
                                    character->speed += 0.5;
                                    break;
                                case SDLK_o:
                                    character->speed -= 0.5;
                                    break;
                                case SDLK_LSHIFT:
                                    action_dash(character, controls, map);
                                    break;
                                case SDLK_KP_5:
                                    character->dash->on_air = SDL_TRUE;
                                    break;
                                case SDLK_RIGHTPAREN:
                                    character->dash->on_air = SDL_TRUE;
                                    break;
                                case SDLK_KP_8:
                                    character->dash->go_up = SDL_TRUE;
                                    break;
                                case SDLK_EQUALS:
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
                                case SDLK_l:
                                    printf("x : %d, y: %d\n", character->x, character->y);
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
                if (character->next_map == SDL_TRUE ) {
                    if (tutorial_step != 0){
                        game_playing=0;
                        menu=1;
                    }
                    if (character->num_map < NUMBER_MAPS){
                        create_map_txt(pat, "test.txt");
                        character->num_map+=1;
                        map = change_map(map, "test.txt", character, &camera, map->tile_width, map->tile_height);
                    } else {
                        game_playing = 0;
                        menu = 2;
                        if (character->timer < best_time){
                            save_time("settings.txt", character->timer);
                            best_time=character->timer;
                        }

                    }

                }
                // Applique la gravité au personnage
                gravity(character);
                // Applique le mouvement au personnage
                mouvement(map, character);
                // Affiche la map et le personnage dans la fenêtre
                // draw(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
                // camera.fps++;
            }
            // C'est la condition qui donne le FPS
            if (getCurrentTimeInMicroseconds() - last_time_fps >= 1000000 / MAX_FPS) {
                last_time_fps = getCurrentTimeInMicroseconds();
                if(game_playing == 1){
                    character->timer = ((double)getCurrentTimeInMicroseconds() - timer_start)/1000000.0;
                }
                draw_ingame(renderer, bleu, texture, map, tile_width, tile_height, character, &camera);
                camera.fps++;
            }
        }
    }

    // printf("x: %d, y: %d \n", character->x, character->y);
    statut = EXIT_SUCCESS;
    free(controls);
    free(map);
    free(character->dash);
    free(character->slide);
    free(character);
    free(mouse);
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

void draw_character_offset(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera, SDL_Rect dst,
                           int offset) {
    if (character->slide->duration > 0){
        if (character->slide->go_right) {
            SDL_RenderCopy(renderer, texture->main_character[8 + offset], NULL, &dst);
        } else {
            SDL_RenderCopyEx(renderer, texture->main_character[8 + offset], NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    } else if (character->dash->duration > 0) {
        dash_display(character, texture, renderer, camera, dst);
    } else if (character->right == SDL_TRUE && character->left == SDL_TRUE && character->on_ground == SDL_TRUE) {
        if (character->crouch == SDL_FALSE){
            SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
        } else {
            SDL_RenderCopy(renderer, texture->main_character[7 + offset], NULL, &dst);
        }
    }else if (character->right == SDL_TRUE && character->on_ground == SDL_TRUE && character->dx != 0) {
        //marche à droite
        if (character->crouch == SDL_FALSE){
            draw_character_animation(renderer, character, texture, &dst, camera, 1 + offset, character->speed, 7);
        } else {
            draw_character_animation(renderer, character, texture, &dst, camera, 6 + offset, character->speed, 3);
        }
    } else if (character->left == SDL_TRUE && character->on_ground == SDL_TRUE && character->dx != 0) {
        if (character->crouch == SDL_FALSE){
            draw_character_animationEx(renderer, character, texture, &dst, camera, 1 + offset, SDL_FLIP_HORIZONTAL, character->speed,7);
        } else {
            draw_character_animationEx(renderer, character, texture, &dst, camera, 6 + offset, SDL_FLIP_HORIZONTAL, character->speed,3);
        }
    } else if (character->wall_jump_right == SDL_TRUE){
        dst.x +=5;
        SDL_RenderCopy(renderer, texture->main_character[5 + offset], NULL, &dst);
    } else if (character->wall_jump_left == SDL_TRUE){
        dst.x -=5;
        SDL_RenderCopyEx(renderer, texture->main_character[5 + offset], NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
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
        if (character->crouch == SDL_FALSE){
            SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
        } else {
            SDL_RenderCopy(renderer, texture->main_character[7 + offset], NULL, &dst);
        }
    } else if (character->dash->duration > 0) {
        if (character->crouch == SDL_FALSE){
            SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
        } else {
            SDL_RenderCopy(renderer, texture->main_character[7 + offset], NULL, &dst);
        }
    } else if (character->on_ground == SDL_TRUE) {
        if (character->crouch == SDL_FALSE){
            SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
        } else {
            SDL_RenderCopy(renderer, texture->main_character[7 + offset], NULL, &dst);
        }
    } else if (character->up == SDL_TRUE) {
        SDL_RenderCopy(renderer, texture->main_character[2 + offset], NULL, &dst);
    } else {
        if (character->crouch == SDL_FALSE){
            SDL_RenderCopy(renderer, texture->main_character[0 + offset], NULL, &dst);
        } else {
            SDL_RenderCopy(renderer, texture->main_character[7 + offset], NULL, &dst);
        }
    }
}

void draw_character(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera) {
    // Affiche le personnage dans la fenêtre
    SDL_Rect dst = {character->x - camera->x, character->y - camera->y, character->width, character->height};
    SDL_Rect dst_indication_key = {character->x - camera->x, character->y - camera->y-50,100, 25};
    if (character->dash->cooldown > 0) {
        // for (int i = 0; i < 5; i++) {
        //     SDL_SetTextureColorMod(texture->main_character[i], 255, 0, 0);
        //     SDL_SetTextureAlphaMod(texture->main_character[i], 255);
        // }
        draw_character_offset(renderer, character, texture, camera, dst, 9);
        draw_indication(renderer, character, texture, camera, dst_indication_key);
    } else {
        // for (int i = 0; i < 5; i++) {
        //     SDL_SetTextureColorMod(texture->main_character[i], 255, 255, 255);
        //     SDL_SetTextureAlphaMod(texture->main_character[i], 255);
        // }
        draw_character_offset(renderer, character, texture, camera, dst, 0);
        draw_indication(renderer, character, texture, camera, dst_indication_key);
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

void draw_indication(SDL_Renderer *renderer, Character *character, Texture *texture, Camera *camera, SDL_Rect dst_key){
    if (character->key_suggestion != SDLK_F15) {
        if (character->key_suggestion == SDLK_q) {
            SDL_RenderCopy(renderer, texture->key_suggestion[0], NULL, &dst_key);
        } else if (character->key_suggestion == SDLK_d) {
            SDL_RenderCopy(renderer, texture->key_suggestion[1], NULL, &dst_key);
        } else if (character->key_suggestion == SDLK_SPACE) {
            SDL_RenderCopy(renderer, texture->key_suggestion[2], NULL, &dst_key);
        } else if (character->key_suggestion == SDLK_s) {
            SDL_RenderCopy(renderer, texture->key_suggestion[3], NULL, &dst_key);
        } else if (character->key_suggestion == SDLK_e) {
            SDL_RenderCopy(renderer, texture->key_suggestion[4], NULL, &dst_key);
        }
    }
    if (character->text_suggestion != NULL) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(character->text_suggestion, NULL, NULL, &textureWidth, &textureHeight);
        SDL_Rect dst_text = {camera->width * 100 /2,camera->height * 100 /3, textureWidth, textureHeight};
        dst_text.x -= textureWidth/2;
        dst_text.y -= textureHeight/2;

        SDL_RenderCopy(renderer, character->text_suggestion, NULL, &dst_text);
    }

}

void draw_time(SDL_Renderer *renderer, Character *character, Camera *camera, Texture *texture) {
    if (camera->show_timer == SDL_TRUE) {
        int m1, m2, s1, s2, d1, d2;
        if (character->timer >= 999.99) {
            m1 = 9;
            m2 = 9;
            s1 = 9;
            s2 = 9;
            d1 = 9;
            d2 = 9;
        } else if (character->timer == 0.) {
            m1 = 0;
            m2 = 0;
            s1 = 0;
            s2 = 0;
            d1 = 0;
            d2 = 0;
        } else {
            int secondes = (int) character->timer;
            int minutes = secondes / 60;
            secondes = secondes % 60;
            m1 = minutes %10;
            m2 = (minutes / 10) % 10;
            s1 = (secondes) % 10;
            s2 = (secondes / 10) % 10;
            d1 = ((int)(character->timer * 100)) % 10;
            d2 = ((int)(character->timer * 100)) / 10 % 10;
        }
        int numWidth, numHeight;
        int comaWidth, comaHeight;
        SDL_QueryTexture(texture->timer[8], NULL, NULL, &numWidth, &numHeight);
        SDL_QueryTexture(texture->timer[10], NULL, NULL, &comaWidth, &comaHeight);
        SDL_Rect dst = {camera->width *100 - numWidth * 2, 0, 100, 100};
        dst.w = numWidth * 2;
        dst.h = numHeight * 2;
        SDL_RenderCopy(renderer, texture->timer[d1], NULL, &dst);
        dst.x -= numWidth * 2;
        SDL_RenderCopy(renderer, texture->timer[d2], NULL, &dst);
        dst.x -= comaWidth * 2;
        dst.x -= comaWidth * 2;
        dst.w = comaWidth *2;
        SDL_RenderCopy(renderer, texture->timer[10], NULL, &dst);
        dst.w = numWidth * 2;
        dst.x -= numWidth * 2;
        SDL_RenderCopy(renderer, texture->timer[s1], NULL, &dst);
        dst.x -= numWidth * 2;
        SDL_RenderCopy(renderer, texture->timer[s2], NULL, &dst);
        dst.x -= comaWidth * 2;
        dst.w = comaWidth *2;
        SDL_RenderCopy(renderer, texture->timer[10], NULL, &dst);
        dst.w = numWidth * 2;
        dst.x -= numWidth * 2;
        SDL_RenderCopy(renderer, texture->timer[m1], NULL, &dst);
        dst.x -= numWidth * 2;
        SDL_RenderCopy(renderer, texture->timer[m2], NULL, &dst);
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

void draw_ingame(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Map *map, int tile_width, int tile_height,
          Character *character, Camera *camera) {
    // Afficher le arrière plan puis déplacer la camera, affiche la map, le personnage dans la fenêtre et met à jour l'affichage
    setWindowColor(renderer, bleu);
    draw_background(renderer, texture, camera, map);
    move_camera(camera, character, map);
    draw_map(renderer, texture, map, tile_width, tile_height, camera);
    render_character_animations(renderer, camera);
    update_character_animations();
    draw_character(renderer, character, texture, camera);
    draw_fps(renderer, camera, texture);
    draw_time(renderer, character, camera, texture);
    SDL_RenderPresent(renderer);
}

void draw_background(SDL_Renderer *renderer, Texture *texture, Camera *camera, Map *map ){
    int tile_width = SCREEN_WIDTH / camera->width;
    int max_size=map->width * tile_width - (camera->width * tile_width);
    int width=1024*2;
    int height= 512 *2;
    int total_width_midground=0;
    int total_width_foreground=0;
    while (total_width_midground <= max_size/3 + width){
        SDL_Rect dst = {-camera->x /3 + total_width_midground, (camera->height +1)*100-1024, width, height};
        SDL_RenderCopy(renderer, texture->background[1], NULL, &dst);
        total_width_midground += width;
    }
    while (total_width_foreground <= max_size/2 + width){
        SDL_Rect dst = {-camera->x /2 + total_width_foreground, (camera->height +1)*100-1024, width, height};
        SDL_RenderCopy(renderer, texture->background[0], NULL, &dst);
        total_width_foreground += width;
    }
}

void draw_homepage(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Camera *camera, Mouse *mouse) {
    // Afficher le arrière plan puis déplacer la camera, affiche la map, le personnage dans la fenêtre et met à jour l'affichage
    setWindowColor(renderer, bleu);
    SDL_Rect dst_n1 = {0, (camera->height +1)*100-1024, 2048, 1024};
    SDL_RenderCopy(renderer, texture->background[1], NULL, &dst_n1);
    SDL_Rect dst_n2 = {0, (camera->height +1)*100-1024, 2048, 1024};
    SDL_RenderCopy(renderer, texture->background[0], NULL, &dst_n2);
    SDL_Rect dst_bouton_start = {(camera->width * 100 / 2) - 500, camera->height * 100 / 5, 1000, 250};
    SDL_Rect dst_bouton_tutorial = {(camera->width * 100 / 2) - 500, camera->height * 200 / 3, 1000, 250};
    SDL_RenderCopy(renderer, texture->bouttons[0], NULL, &dst_bouton_start);
    SDL_RenderCopy(renderer, texture->bouttons[1], NULL, &dst_bouton_tutorial);
    draw_fps(renderer, camera, texture);

    if (mouse->x >= dst_bouton_start.x && mouse->x < dst_bouton_start.x + dst_bouton_start.w &&
        mouse->y >= dst_bouton_start.y && mouse->y < dst_bouton_start.y + dst_bouton_start.h) {
        mouse->on_boutton = SDL_TRUE;
        mouse->num_boutton = 0;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // Dessiner un rectangle blanc légèrement transparent sur le bouton "Tutorial" si la souris est dessus
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100); // Blanc semi-transparent
        SDL_Rect highlightRect = dst_bouton_start; // Créer un rectangle de highlight sur le bouton "Tutorial"
        SDL_RenderFillRect(renderer, &highlightRect);
    } else if (mouse->x >= dst_bouton_tutorial.x && mouse->x < dst_bouton_tutorial.x + dst_bouton_tutorial.w &&
               mouse->y >= dst_bouton_tutorial.y && mouse->y < dst_bouton_tutorial.y + dst_bouton_tutorial.h) {
        mouse->on_boutton = SDL_TRUE;
        mouse->num_boutton = 1;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // Dessiner un rectangle blanc légèrement transparent sur le bouton "Tutorial" si la souris est dessus
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100); // Blanc semi-transparent
        SDL_Rect highlightRect = dst_bouton_tutorial; // Créer un rectangle de highlight sur le bouton "Tutorial"
        SDL_RenderFillRect(renderer, &highlightRect);
    } else {
        mouse->on_boutton = SDL_FALSE;
    }
    SDL_RenderPresent(renderer);
}

void draw_endpage(SDL_Renderer *renderer, SDL_Color bleu, Texture *texture, Camera *camera, Mouse *mouse, Character *character, double best_time){
    SDL_Color green= {0,122, 129, 36};
    setWindowColor(renderer,green);
    SDL_Rect dst_n1 = {0, (camera->height +1)*100-1024, 2048, 1024};
    SDL_RenderCopy(renderer, texture->background[1], NULL, &dst_n1);
    SDL_Rect dst_n2 = {0, (camera->height +1)*100-1024, 2048, 1024};
    SDL_RenderCopy(renderer, texture->background[0], NULL, &dst_n2);
    SDL_Rect dst_bouton_restart = {(camera->width * 100 / 2) - 500, camera->height * 200 / 5, 1000, 250};
    SDL_Rect dst_bouton_menu = {(camera->width * 100 / 2) - 500, camera->height * 300 / 4, 1000, 250};
    SDL_RenderCopy(renderer, texture->bouttons[2], NULL, &dst_bouton_restart);
    SDL_RenderCopy(renderer, texture->bouttons[3], NULL, &dst_bouton_menu);
    draw_fps(renderer, camera, texture);

    if (mouse->x >= dst_bouton_restart.x && mouse->x < dst_bouton_restart.x + dst_bouton_restart.w &&
        mouse->y >= dst_bouton_restart.y && mouse->y < dst_bouton_restart.y + dst_bouton_restart.h) {
        mouse->on_boutton = SDL_TRUE;
        mouse->num_boutton = 0;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // Dessiner un rectangle blanc légèrement transparent sur le bouton "Tutorial" si la souris est dessus
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100); // Blanc semi-transparent
        SDL_Rect highlightRect = dst_bouton_restart; // Créer un rectangle de highlight sur le bouton "Tutorial"
        SDL_RenderFillRect(renderer, &highlightRect);
    } else if (mouse->x >= dst_bouton_menu.x && mouse->x < dst_bouton_menu.x + dst_bouton_menu.w &&
               mouse->y >= dst_bouton_menu.y && mouse->y < dst_bouton_menu.y + dst_bouton_menu.h) {
        mouse->on_boutton = SDL_TRUE;
        mouse->num_boutton = 1;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // Dessiner un rectangle blanc légèrement transparent sur le bouton "Tutorial" si la souris est dessus
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100); // Blanc semi-transparent
        SDL_Rect highlightRect = dst_bouton_menu; // Créer un rectangle de highlight sur le bouton "Tutorial"
        SDL_RenderFillRect(renderer, &highlightRect);
    } else {
        mouse->on_boutton = SDL_FALSE;
    }
    int c;
    draw_time(renderer, character, camera, texture);
    if (best_time == character->timer){
        printf("RECOOOOOORD DU MOOOONDE\n");
        double t=getCurrentTimeInMicroseconds();
        c = (((int)(t))/100000) % 6;
        if (c < 3){
            c=0;
        } else {c =1;}
        printf("%d", c);
    } else {c=0;}
    int seconds = (int) best_time;
    int minutes = seconds / 60;
    seconds = seconds % 60;
    int centiseconds = ((int) (best_time* 100))%100;
    char best_score_text[30];
    sprintf(best_score_text, "best time : %02d:%02d:%02d", minutes, seconds, centiseconds);
    TTF_SetFontStyle(texture->font, TTF_STYLE_NORMAL);

    SDL_Color color = {192+40*c, 190+40*c, 193+40*c, 255};
    // Créer la surface à partir du texte
    SDL_Surface *surface = TTF_RenderText_Solid(texture->font, best_score_text, color);
    SDL_Texture *best_score_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dst_best_score = {(camera->width * 100 / 2) - 440, camera->height * 100 / 5, 1000, 250};
    SDL_QueryTexture(best_score_texture, NULL, NULL, &dst_best_score.w, &dst_best_score.h);
    dst_best_score.w = dst_best_score.w *2;
    dst_best_score.h = dst_best_score.h *2;
    SDL_RenderCopy(renderer, best_score_texture, NULL, &dst_best_score);
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
    camera->show_timer = SDL_TRUE;
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
            // Map *pattern = generated_pattern(camera, character, map);
            // // Si la map n'est pas pleine alors on ajoute le pattern à droite de la map
            // if (map->full == SDL_FALSE) {
            //     add_right_pattern_to_map(pattern, map);
            //     camera->x = character->x - pixel_width + (character->width / 2);
            // } else {
            //     // Sinon on force la camera à être à la fin de la map
            //     camera->x = map->width * tile_width - (camera->width * tile_width);
            //     free(pattern);
            // }
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
