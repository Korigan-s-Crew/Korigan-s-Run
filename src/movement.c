#include "../include/movement.h"
#include "../include/main.h"
#include "../include/map.h"

void mouvement(Map *map, Character *character) {
//    printf("character x: %d y: %d\n", character->x, character->y);
    // Gère le mouvement du personnage


    if (character->right == SDL_TRUE && !character->wall_right) {
        move_character_right(character, map->tile_width);
    }
    if (character->left == SDL_TRUE && !character->wall_left) {
        move_character_left(character, map->tile_width);
    }
    if (character->down == SDL_TRUE && character->up == SDL_TRUE) {
        move_character_up(character, map->tile_width, map->tile_height);
    } else {
        if (character->up == SDL_TRUE) {
            move_character_up(character, map->tile_width, map->tile_height);
        }
        if (character->down == SDL_TRUE && character->height == character->original_height) {
            move_character_down(character, map->tile_height);
        }
    }

    if (character->wall_right == SDL_TRUE) {
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

    // handle slide
    handle_slide(character, map);
    // handle being in dash
    handle_dash(character, map);


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

// ############## Slide part

Slide *init_slide() {
    // Initialise le dash du personnage
    Slide *slide = malloc(sizeof(Slide));
    if (!slide) {
        fprintf(stderr, "Failed to allocate memory for slide\n");
        return NULL;
    }
    slide->duration = 0;
    slide->go_left = SDL_FALSE;
    slide->go_right = SDL_FALSE;
    return slide;
}

void action_slide(Character *character, Map *map) {
    // Handle slide key pressed
    Slide *slide = character->slide;
    if (character->just_landed && character->down) {
        character->just_landed = SDL_FALSE;

        if (character->right == SDL_TRUE && character->left == SDL_FALSE) {
            if (change_size_collision(character, map, character->original_height, character->original_width)) {
                character->height = character->original_width;
                character->width = character->original_height;
                slide->duration = 50;
            } else {
                printf("can't slide here\n");
            }
            slide->go_right = SDL_TRUE;
        } else if (character->left == SDL_TRUE && character->right == SDL_FALSE) {
            if (change_size_collision(character, map, character->original_height, character->original_width)) {
                character->height = character->original_width;
                character->width = character->original_height;
                slide->duration = 50;
            } else {
                printf("can't slide here\n");
            }
            slide->go_left = SDL_TRUE;
        }
        printf("slide duration: %d %d %d \n", slide->duration, slide->go_right, slide->go_left);
    }
    character->just_landed = SDL_FALSE;
}

void handle_slide(Character *character, Map *map) {

    if (character->slide->duration > 0) {
        if (character->slide->go_right) {
            character->dx = (map->tile_width / 1.2) * character->speed;
        } else if (character->slide->go_left) {
            character->dx = -(map->tile_width / 1.2) * character->speed;
        } else { //todo if no direction is set just crouch
            slide_cancel(character, map);
        }
        if (character->up && character->slide->duration < 30) {
            slide_cancel(character, map);
            move_character_up(character, map->tile_width, map->tile_height);
        } else {}
        character->slide->duration -= 1;
        if (character->slide->duration == 0) {
            slide_cancel(character, map);
        }
    } else {
        action_slide(character, map);
    }
}

void slide_cancel(Character *character, Map *map) {
    // cancel slide
    character->slide->duration = 0;
    if (change_size_collision(character, map, character->original_width, character->original_height)) {
        character->height = character->original_height;
        character->width = character->original_width;
    }else{
        // crouch
    }
    character->slide->go_right = SDL_FALSE;
    character->slide->go_left = SDL_FALSE;
}

// ############## End Slide part

// ############## Dash part

Dash *init_dash() {
    // Initialise le dash du personnage
    Dash *dash = malloc(sizeof(Dash));
    if (!dash) {
        fprintf(stderr, "Failed to allocate memory for dash\n");
        return NULL;
    }
    dash->duration = 0;
    dash->cooldown = 0;
    dash->direction.x = 0;
    dash->direction.y = 0;
    dash->on_air = SDL_FALSE;
    dash->go_up = SDL_FALSE;
    return dash;
}

void action_dash(Character *character, Controls *controls) {
    // Handle dash key pressed
    Dash *dash = character->dash;
    if (dash->cooldown == 0) {
        if (dash->on_air == SDL_TRUE || character->on_ground == SDL_TRUE) {
            dash->duration = 20;
            dash->cooldown = 200;
            if (character->right == SDL_TRUE && character->left == SDL_FALSE) {
                character->dash->direction.x = 1;
            } else if (character->left == SDL_TRUE && character->right == SDL_FALSE) {
                character->dash->direction.x = -1;
            } else if (character->right == SDL_TRUE && character->left == SDL_TRUE) {
                if (character->dx > 0) {
                    character->dash->direction.x = 1;
                } else if (character->dx < 0) {
                    character->dash->direction.x = -1;
                } else {
                    character->dash->direction.x = 0;
                }
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
    printf("dash duration: %d %d %d \n", dash->duration, dash->direction.x, dash->direction.y);

}


void handle_dash(Character *character, Map *map) {
    // handle dash movement
    if (character->dash->duration > 0) {// inside dash animation
        character->dy = 0;
        character->dx = (map->tile_width / (3 * 0.166)) * character->speed * character->dash->direction.x;
        if (character->dash->go_up == SDL_TRUE) {
            character->dy = (map->tile_height / (5 * 0.166)) * character->speed * character->dash->direction.y;
        }
        move_character(character, character->dx, character->dy, map);
        character->dash->duration -= 1;
        if (character->dash->duration == 0) { // dash is finished
            if (character->dx > 0) {
                character->dx = min((map->tile_width / 1.5) * character->speed, character->dx);
            } else if (character->dx < 0) {
                character->dx = max(-(map->tile_width / 1.5) * character->speed, character->dx);
            }

            character->dy = 0;
        }

        character->dash->cooldown -= 1;
    } else {
        if (character->dash->cooldown > 0) { // if the dash in on cooldown
            if (!(character->dy == 0 && (character->wall_left || character->wall_right)) || character->on_ground) {
                character->dash->cooldown -= 1;// cooldown don't refresh if you stick to a wall
            }
        }
    }
}

void print_dash(Dash *dash) {
    printf("dash duration: %d\n", dash->duration);
    printf("dash cooldown: %d\n", dash->cooldown);
    printf("dash direction x: %d\n", dash->direction.x);
    printf("dash direction y: %d\n", dash->direction.y);
    printf("dash on air: %d\n", dash->on_air);
    printf("dash go up: %d\n", dash->go_up);
}

//############### End Dash part

void gravity(Character *character) {
    // gravity application
    if (character->on_ground == SDL_FALSE) {// if in the air
        if (character->dash->duration == 0 || character->dash->go_up == SDL_TRUE) {// if not in dash or in dash form 3
            if (character->up == SDL_FALSE) {// if not pressing up key then higher gravity
                character->dy = min(character->dy + 25, 180);
            } else {
                character->dy = min(character->dy + 8, 180);
            }
        }
    }
}

void move_character(Character *character, int x, int y, Map *map) {
//    printf("move of dx: %d dy: %d\n", x, y);
    // Déplace le personnage de x et y et gère les collisions avec la map
    if (x > 0) {
        for (int i = 0; i < x / 10; i++) {
            collision(character, map);
            if (character->dx == 0) {
                break;
            }
            character->x += 1;
        }
    } else {
        for (int i = 0; i < -x / 10; i++) {
            collision(character, map);
            if (character->dx == 0) {
                break;
            }
            character->x -= 1;
        }
    }
    if (y > 0) {
        for (int i = 0; i < y / 10; i++) {
            collision(character, map);
            if (character->dy == 0) {
                break;
            }
            character->y += 1;
        }
    } else {
        for (int i = 0; i < -y / 10; i++) {
            collision(character, map);
            if (character->dy == 0) {
                break;
            }
            character->y -= 1;
        }
    }
}

void slow_down(Character *character) {
    // slowly stop character
    if (character->dx > 0) {
        if (character->on_ground == SDL_FALSE) {// on air there is less friction
            character->dx -= 3;
        } else {
            character->dx = max(character->dx - 5, 0);
        }
    } else if (character->dx < 0) {
        if (character->on_ground == SDL_FALSE) {// same here
            character->dx += 3;
        } else {
            character->dx = min(character->dx + 5, 0);
        }
    }
}

void move_character_up(Character *character, int tile_width, int tile_height) {
    if (character->on_ground == SDL_TRUE) {// classic jump
        character->dy = -(tile_height * 2);
        character->on_ground = SDL_FALSE;
    } else if (character->wall_right == SDL_TRUE && character->dy > 0 && !character->right) {
        //right wall jump (jump to the left)
        character->dy = -(tile_height * 2);
        character->dx = -(tile_width / 1.40) * character->speed;
        character->wall_right = SDL_FALSE;
    } else if (character->wall_left == SDL_TRUE && character->dy > 0 && !character->left) {
        //left wall jump (jump to the right)
        character->dy = -(tile_height * 2);
        character->dx = (tile_width / 1.40) * character->speed;
        character->wall_left = SDL_FALSE;
    }
}

void move_character_down(Character *character, int tile_height) {
    character->dy = 250;// fall faster than max gravity
}

void move_character_left(Character *character, int tile_width) {
    if (character->dx > 0 && character->on_ground) {// if the character has right inertia
        character->dx = character->dx - 7;
    } else {
        character->dx = max(character->dx - 5, -(tile_width / 1.5) * character->speed);
    }
}

void move_character_right(Character *character, int tile_width) {
    if (character->dx < 0 && character->on_ground) {// if the character has left inertia
        character->dx = character->dx + 7;
    } else {
        character->dx = min(character->dx + 5, (tile_width / 1.5) * character->speed);
    }
}
