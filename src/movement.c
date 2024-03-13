#include "../include/movement.h"
#include "../include/main.h"

void mouvement(Map *map, Character *character, int tile_width, int tile_height) {
    // Gère le mouvement du personnage
    if (character->right == SDL_TRUE) {
        move_character_right(character, tile_width);
    }
    if (character->left == SDL_TRUE) {
        move_character_left(character, tile_width);
    }
    if (character->down == SDL_TRUE && character->up == SDL_TRUE) {
        move_character_up(character, tile_height);
    } else {
        if (character->up == SDL_TRUE) {
            move_character_up(character, tile_height);
        }
        if (character->down == SDL_TRUE && character->height == character->original_height) {
            move_character_down(character, tile_height);
        }
    }
    // Si le personnage vient de faire un dash on multiplie sa vitesse par 20 lorsqu'il n'est pas en train de tomber
    if (character->dash > 0) {
//        if (character->dy <= 0) {
//            character->dy += 30*(character->dy)/(abs(character->dy));
//        }
        character->dy = 0;
        if (character->dx >0){
            character->dx = (tile_width / 3) * character->speed;
        }else if (character->dx <0){
            character->dx = -(tile_width / 3) * character->speed;
        }
    }
    // Si le personnage va sur la droite et sur la gauche en même temps on annule sa vitesse horizontale
    if (character->right == SDL_TRUE && character->left == SDL_TRUE) {
        character->dx = 0;
    }
    // Si le personnage est au sol et qu'il appuie sur control_down il se baisse (réduit sa taille)
    if (character->down == SDL_TRUE && character->on_ground == SDL_TRUE) {
        character->height = (int)(character->original_height / 2);
        character->width = (int)(character->original_width / 1.5);
    }
    // Si n'appuie plus sur control_down et qu'il est petit on le remet à sa taille d'origine
    if (character->down == SDL_FALSE && character->height < character->original_height) {
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

        // Si le personnage n'a pas la place pour se redresser on le remet à sa position précédente et on ne change pas sa taille
        // Si il a la place on remet sa taille d'origine

        if (character->y == copy_y - (int)character->original_height / 2 &&
            character->x == copy_x + (int)character->original_width / 3) {
            character->height = character->original_height;
            character->width = character->original_width;
            // On vérifie qu'il a la place avec move_character puis on le remet à sa position précédente
            character->x = copy_x;
        } else {
            character->y = copy_y;
            character->x = copy_x;
        }
    }
    // Si le personnage ne bouge pas on n'effectue pas de déplacement ET DE COLLISION !!!
    if (character->dx != 0 || character->dy != 0)
        move_character(character, character->dx, character->dy, map, tile_width, tile_height);
    // Si le personnage vient de faire un dash on divise sa vitesse par 20
    if (character->dash >0) {
//        if (character->dy <= 0) {
//            character->dy -= 30*(character->dy)/(abs(character->dy));
//        }
        character->dash -= 1;
        if (character->dash == 0) {
            character->dx = 0;
        }
    }
}

void gravity(Character *character) {
    // Applique la gravité au personnage
    // Si le personnage n'est pas sur le sol et que sa vitesse verticale est inférieure à 10
    if (character->on_ground == SDL_FALSE && character->dash == 0) {
        if (character->dy < 20) {
            character->dy += 1;
        }
    }
}

void move_character(Character *character, int x, int y, Map *map, int tile_width, int tile_height) {
    // Déplace le personnage de x et y et gère les collisions avec la map
    if (x > 0) {
        for (int i = 0; i < x; i++) {
            collision(character, map, tile_width, tile_height);
            if (character->dx == 0) {
                break;
            }
            character->x += 1;
        }
    } else {
        for (int i = 0; i < -x; i++) {
            collision(character, map, tile_width, tile_height);
            if (character->dx == 0) {
                break;
            }
            character->x -= 1;
        }
    }
    if (y > 0) {
        for (int i = 0; i < y; i++) {
            collision(character, map, tile_width, tile_height);
            if (character->dy == 0) {
                break;
            }
            character->y += 1;
        }
    } else {
        for (int i = 0; i < -y; i++) {
            collision(character, map, tile_width, tile_height);
            if (character->dy == 0) {
                break;
            }
            character->y -= 1;
        }
    }
}

void move_character_up(Character *character, int tile_height) {
    if (character->on_ground == SDL_TRUE) {
        character->dy = -(tile_height / 5);
        // printf("dy: %d\n", character->dy);
        character->on_ground = SDL_FALSE;
    }
}

void move_character_down(Character *character, int tile_height) {
    character->dy = (tile_height / 5);
}

void move_character_left(Character *character, int tile_width) {
    character->dx = -(tile_width / 15) * character->speed;
}

void move_character_right(Character *character, int tile_width) {
    character->dx = (tile_width / 15) * character->speed;
}
