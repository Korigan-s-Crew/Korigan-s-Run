#include "../include/main.h"

#include <time.h>

#include "../include/controls.h"
#include "../include/init.h"
#include "../include/map.h"
#include "../include/movement.h"
#include "../include/procedural_generation.h"
#include "../include/dash_effect.h"
#include "../include/animation.h"
#include "../include/music.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int DASH_COOLDOWN;

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
    create_map_txt(pat, "./Patterns/test.txt");
    Map *map = create_map("./Patterns/test.txt", tile_width, tile_height);

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
    load_best_time(renderer, texture, character, best_time);
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
    // Initialisation de la musique
    Mix_Music* music = read_audio_file("Music/Transforyou.mp3");

#define next_step_tuto() ( \
(tutorial_step == 5 && (character->wall_jump_right==SDL_TRUE || character->wall_jump_left==SDL_TRUE)) ||                   \
(tutorial_step == 6 && character->x > 3367) ||                                                                           \
(tutorial_step == 7 && character->dash->go_up == SDL_TRUE) ||                                                            \
(tutorial_step == 9 && character->x > 4752) ||                                                                             \
(tutorial_step == 10 && character->slide->duration > 0) ||                                                                 \
(tutorial_step == 11 && character->x > 7190) || \
(tutorial_step == 12 && character->on_portal == SDL_TRUE))
    // Boucle principale
    int running = 1;
    int game_playing=0;
    double timer_start;
    int menu=1;
    play_music(music);
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
                                // printf("escape main\n");
                                running = 0;
                                break;
                            case SDLK_t:
                                game_playing = 1;
                                tutorial_step = 1;
                                character->num_map=1;
                                character->key_suggestion = key_for_tuto[tutorial_step];
                                character->text_suggestion = text_for_tuto_texture[tutorial_step];
                                map = change_map(map, "./Patterns/map_tuto.txt", character, &camera, map->tile_width, map->tile_height);
                                camera.show_timer = SDL_FALSE;
                                break;
                            case SDLK_e:
                                game_playing = 1;
                                tutorial_step = 0;
                                character->num_map=1;
                                character->key_suggestion = SDLK_F15;
                                character->text_suggestion = NULL;
                                camera.show_timer = SDL_TRUE;
								create_map_txt(pat, "./Patterns/test.txt");
								map = change_map(map, "./Patterns/test.txt", character, &camera, map->tile_width, map->tile_height);
                                timer_start = (double)getCurrentTimeInMicroseconds() ;
                                break;
                            case SDLK_k:
								play_music(music);
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
						}
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (mouse->on_boutton == SDL_TRUE ) {
                            if (mouse->num_boutton == 0) {
                                create_map_txt(pat, "./Patterns/test.txt");
                                game_playing = 1;
                                tutorial_step=0;
                                character->num_map=1;
                                character->text_suggestion = NULL;
                                character->key_suggestion=SDLK_F15;
                                camera.show_timer = SDL_TRUE;
								create_map_txt(pat, "./Patterns/test.txt");
								map = change_map(map, "./Patterns/test.txt", character, &camera, map->tile_width, map->tile_height);
                                timer_start = (double)getCurrentTimeInMicroseconds() ;
                                break;
                            } else if (mouse->num_boutton == 1) {
                                if (menu == 1){
                                    game_playing = 1;
                                    tutorial_step = 1;
                                    character->num_map=1;
                                    character->key_suggestion=key_for_tuto[tutorial_step];
                                    character->text_suggestion = text_for_tuto_texture[tutorial_step];
                                    map = change_map(map, "./Patterns/map_tuto.txt", character, &camera, map->tile_width, map->tile_height);
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
                            reset_character(character,map,tile_width,tile_height);
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
                                    // printf("escape\n");
                                    reset_character(character,map,tile_width,tile_height);
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
                                    break;
                                case SDLK_TAB:
                                    switchLayout(controls);
                                    break;
                                case SDLK_l:
                                    printf("Cheat: Dash with 0 cooldown\n");
                                    if (DASH_COOLDOWN == 300){
                                        DASH_COOLDOWN = 26;
                                    } else {DASH_COOLDOWN = 300;}
                                    break;
                                case SDLK_k:
									play_music(music);
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
                    reset_character(character, map, tile_width, tile_height);
                }
                if (character->next_map == SDL_TRUE ) {
                    if (tutorial_step != 0){
                        menu=1;
                        game_playing=0;
                    } else if (character->num_map < NUMBER_MAPS){
                        create_map_txt(pat, "./Patterns/test.txt");
                        character->num_map+=1;
                        map = change_map(map, "./Patterns/test.txt", character, &camera, map->tile_width, map->tile_height);
                    } else {
                        game_playing = 0;
                        menu = 2;
                        if (character->timer < best_time){
                            save_time("settings.txt", character->timer);
                            best_time=character->timer;
                            load_best_time(renderer, texture, character, best_time);
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
    free_music(music);

Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	return statut;
}
