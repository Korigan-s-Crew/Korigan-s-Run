#include "../include/draw.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

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

void load_best_time(SDL_Renderer *renderer, Texture *texture, Character *character, double best_time){
    int seconds = (int) best_time;
    int minutes = seconds / 60;
    seconds = seconds % 60;
    int centiseconds = ((int) (best_time* 100))%100;
    char best_score_text[30];
    sprintf(best_score_text, "Best time : %02d:%02d:%02d", minutes, seconds, centiseconds);
    TTF_SetFontStyle(texture->font, TTF_STYLE_NORMAL);

    SDL_Color r = {232, 50, 24, 255};
    SDL_Color b = {0, 105, 167, 255};
    SDL_Color g = {90, 196, 57, 255};
    SDL_Color color = {192, 190, 193, 255};
    // Créer la surface à partir du texte
    SDL_Surface *surface = TTF_RenderText_Solid(texture->font, best_score_text, color);
    SDL_Texture *best_score_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Créer la surface à partir du texte
    SDL_Surface *surfacer = TTF_RenderText_Solid(texture->font, best_score_text, r);
    SDL_Texture *best_score_texturer = SDL_CreateTextureFromSurface(renderer, surfacer);
    SDL_Surface *surfaceg = TTF_RenderText_Solid(texture->font, best_score_text, g);
    SDL_Texture *best_score_textureg = SDL_CreateTextureFromSurface(renderer, surfaceg);
    SDL_Surface *surfaceb = TTF_RenderText_Solid(texture->font, best_score_text, b);
    SDL_Texture *best_score_textureb = SDL_CreateTextureFromSurface(renderer, surfaceb);
    SDL_FreeSurface(surfacer);
    SDL_FreeSurface(surfaceg);
    SDL_FreeSurface(surfaceb);
    character->best_time[0]=best_score_texture;
    character->best_time[1]=best_score_texturer;
    character->best_time[2]=best_score_textureg;
    character->best_time[3]=best_score_textureb;
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
    SDL_Rect dst_fond={0,0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, texture->background[3], NULL, &dst_fond);
    SDL_Rect dst_bouton_start = {(SCREEN_WIDTH / 2) - 400,  SCREEN_HEIGHT*2 / 5, 800, 200};
    SDL_Rect dst_bouton_tutorial = {(SCREEN_WIDTH / 2) - 400, camera->height * 210 / 3, 800, 200};
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
    SDL_Rect dst_bouton_restart = {(SCREEN_WIDTH / 2) - 500, camera->height * 200 / 5, 1000, 250};
    SDL_Rect dst_bouton_menu = {(SCREEN_WIDTH / 2) - 500, camera->height * 300 / 4, 1000, 250};
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
        //printf("New record !\n");
        double t=getCurrentTimeInMicroseconds();
        c = abs(((int)(t))/100000) % 3 +1;
    } else {c=0;}
    //printf("c : %d\n", c);
    SDL_Rect dst_best_score = {(camera->width * 100 / 2) - 440, camera->height * 100 / 5, 1000, 250};
    SDL_QueryTexture(character->best_time[c], NULL, NULL, &dst_best_score.w, &dst_best_score.h);
    dst_best_score.w = dst_best_score.w *2;
    dst_best_score.h = dst_best_score.h *2;
    SDL_RenderCopy(renderer, character->best_time[c], NULL, &dst_best_score);
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
