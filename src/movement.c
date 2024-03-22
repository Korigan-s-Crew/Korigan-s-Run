#include "../include/movement.h"
#include "../include/main.h"
#include "../include/map.h"

void mouvement(Map *map, Character *character, int tile_width, int tile_height) {
//    printf("character x: %d y: %d\n", character->x, character->y);
    // Gère le mouvement du personnage
    if (character->right == SDL_TRUE && !character->wall_right) {
        move_character_right(character, tile_width);
    }
    if (character->left == SDL_TRUE && !character->wall_left) {
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

    if (character->wall_right == SDL_TRUE)  {
        if (character->right == SDL_TRUE) {
            character->dy = 0;
        } else {
            character->dy = min(character->dy, 30);
        }
    }
    if (character->wall_left == SDL_TRUE) {
        if (character->left == SDL_TRUE) {
            character->dy = 0;
        } else {
            character->dy = min(character->dy, 30);
        }
    }

    // handle being in dash
    handle_dash(character, tile_width, tile_height, map);
    // Si le personnage va sur la droite et sur la gauche en même temps on annule sa vitesse horizontale
    if (character->right == SDL_TRUE && character->left == SDL_TRUE) {
        slow_down(character);
    }
    if (character->right == SDL_FALSE && character->left == SDL_FALSE) {
        slow_down(character);
    }
    // Si le personnage est au sol et qu'il appuie sur control_down il se baisse (réduit sa taille)
//    if (character->down == SDL_TRUE && character->on_ground == SDL_TRUE) {
//        character->height = (int) (character->original_height / 2);
//        character->width = (int) (character->original_width / 1.5);
////        move_character_down(character, tile_height);
//    }
//    // Si n'appuie plus sur control_down et qu'il est petit on le remet à sa taille d'origine
//    if (character->down == SDL_FALSE && character->height < character->original_height) {
//        int copy_dy = character->dy;
//        int copy_dx = character->dx;
//        character->dy = -(int) (character->original_height / 2);
//        character->dx = (int) (character->original_width / 3);
//        int copy_y = character->y;
//        int copy_x = character->x;
//        move_character(character, character->dx, character->dy, map, tile_width, tile_height);
//        character->on_ground = SDL_TRUE;
//        character->dy = copy_dy;
//        character->dx = copy_dx;
//
//        // Si le personnage n'a pas la place pour se redresser on le remet à sa position précédente et on ne change pas sa taille
//        // Si il a la place on remet sa taille d'origine
//
//        if (character->y == copy_y - (int) character->original_height / 2 &&
//            character->x == copy_x + (int) character->original_width / 3) {
//            character->height = character->original_height;
//            character->width = character->original_width;
//            // On vérifie qu'il a la place avec move_character puis on le remet à sa position précédente
//            character->x = copy_x;
//        } else {
//            character->y = copy_y;
//            character->x = copy_x;
//        }
//    }
    // Si le personnage ne bouge pas on n'effectue pas de déplacement ET DE COLLISION !!!
    if ((character->dx != 0 || character->dy != 0) && character->dash->duration == 0)
        move_character(character, character->dx, character->dy, map);
    // Si le personnage vient de faire un dash on divise sa vitesse par 20
}

// ############## Dash part

void init_dash(Character *character) {
    // Initialise le dash du personnage
    character->dash = malloc(sizeof(Dash));
    if (!character->dash) {
        fprintf(stderr, "Failed to allocate memory for dash\n");
        return;
    }
    character->dash->duration = 0;
    character->dash->cooldown = 0;
    character->dash->direction.x = 0;
    character->dash->direction.y = 0;
    character->dash->on_air = SDL_FALSE;
    character->dash->go_up = SDL_FALSE;
}

void action_dash(Character *character, Controls *controls) {
    // Handle dash key pressed
    Dash *dash = character->dash;
    if (dash->cooldown == 0) {
        if (dash->on_air == SDL_TRUE || character->on_ground == SDL_TRUE) {
            dash->duration = 20;
            dash->cooldown = 200;
            if (character->dx > 0) {
                character->dash->direction.x = 1;
            } else if (character->dx < 0) {
                character->dash->direction.x = -1;
            } else {
                character->dash->direction.x = 0;
            }
            if (character->up == SDL_TRUE) {
                character->dash->direction.y = -1;
            } else if (character->down == SDL_TRUE) {
                character->dash->direction.y = 1;
            } else {
                character->dash->direction.y = 0;
            }
        }
    }
    printf("dash duration: %d %d %d \n", dash->duration,dash->direction.x,dash->direction.y);

}


void handle_dash(Character *character, int tile_width, int tile_height, Map *map) {
    // handle dash movement
    if (character->dash->duration > 0) {
        character->dy = 0;
        character->dx = (tile_width / (3*0.166)) * character->speed * character->dash->direction.x;
        if (character->dash->go_up == SDL_TRUE) {
            character->dy = (tile_height / (5*0.166)) * character->speed * character->dash->direction.y;
        }
        move_character(character, character->dx, character->dy, map);
        character->dash->duration -= 1;
        if (character->dash->duration == 0) {
            character->dx = 0;
            character->dy = 0;
        }
        character->dash->cooldown -= 1;
    } else {
        if (character->dash->cooldown > 0) {
            character->dash->cooldown -= 1;
        }
    }
}

//############### End Dash part

void gravity(Character *character) {
    // Applique la gravité au personnage
    // Si le personnage n'est pas sur le sol et que sa vitesse verticale est inférieure à 10
    if (character->on_ground == SDL_FALSE) {
        if (character->dash->duration == 0 || character->dash->go_up == SDL_TRUE) {
            character->dy += 10;
        }
    }
}

void move_character(Character *character, int x, int y, Map *map) {
//    printf("move of dx: %d dy: %d\n", x, y);
    // Déplace le personnage de x et y et gère les collisions avec la map
    if (x > 0) {
        for (int i = 0; i < x/10; i++) {
            collision(character, map);
            if (character->dx == 0) {
                break;
            }
            character->x += 1;
        }
    } else {
        for (int i = 0; i < -x/10; i++) {
            collision(character, map);
            if (character->dx == 0) {
                break;
            }
            character->x -= 1;
        }
    }
    if (y > 0) {
        for (int i = 0; i < y/10; i++) {
            collision(character, map);
            if (character->dy == 0) {
                break;
            }
            character->y += 1;
        }
    } else {
        for (int i = 0; i < -y/10; i++) {
            collision(character, map);
            if (character->dy == 0) {
                break;
            }
            character->y -= 1;
        }
    }
}

void slow_down(Character *character) {
    // Ralentit le personnage
    if (character->dx > 0) {
        character->dx -= 3;
    } else if (character->dx < 0) {
        character->dx += 3;
    }
}

void move_character_up(Character *character, int tile_height) {
    if (character->on_ground == SDL_TRUE ){//|| character->wall_right == SDL_TRUE || character->wall_left == SDL_TRUE) {
        character->dy = -(tile_height * 2);
        // printf("dy: %d\n", character->dy);
        character->on_ground = SDL_FALSE;
    }
}

void move_character_down(Character *character, int tile_height) {
    character->dy = (tile_height *2);
}

void move_character_left(Character *character, int tile_width) {
    character->dx = max(character->dx - 5 , -(tile_width / 1.5) * character->speed);
}

void move_character_right(Character *character, int tile_width) {
    character->dx = min(character->dx + 5 , (tile_width / 1.5) * character->speed);
}
