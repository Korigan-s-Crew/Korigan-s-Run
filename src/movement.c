#include "../include/movement.h"
#include "../include/main.h"
#include "../include/map.h"
#include "../include/dash_effect.h"

void mouvement(Map *map, Character *character) {
//    printf("character x: %d y: %d\n", character->x, character->y);
    // Gère le mouvement du personnage

    // handle slide
    handle_slide(character, map);
    // handle being in dash
    handle_dash(character, map);

    if (!in_action(character)) {

        if (character->right == SDL_TRUE && !character->wall_right) {
            move_character_right(character, map->tile_width);
        }
        if (character->left == SDL_TRUE && !character->wall_left) {
            move_character_left(character, map->tile_width);
        }
        if (character->down == SDL_TRUE && character->up == SDL_TRUE) {
            move_character_up(character, map, map->tile_width, map->tile_height, 0);
        } else {
            if (character->up == SDL_TRUE) {
                move_character_up(character, map, map->tile_width, map->tile_height, 0);
            }
            if (character->down == SDL_TRUE && character->height == character->original_height) {
                move_character_down(character, map->tile_height);
            }
        }

        if (character->wall_jump_right == SDL_TRUE) {
            if (character->right == SDL_TRUE) {
                character->dy = 0;
            } else {
                character->dy = min(character->dy, 30);
            }
        }
        if (character->wall_jump_left == SDL_TRUE) {
            if (character->left == SDL_TRUE) {
                character->dy = 0;
            } else {
                character->dy = min(character->dy, 30);
            }
        }

        if (character->on_ground && character->down && !character->crouch && !in_action(character)) {
            go_crouch(character, map);
        }

        if (character->crouch && (!character->down || !character->on_ground)) {
            cancel_crouch(character, map);
        }


        // Si le personnage va sur la droite et sur la gauche en même temps on annule sa vitesse horizontale
        if (character->right == SDL_TRUE && character->left == SDL_TRUE) {
            slow_down(character);
        }
        if (character->right == SDL_FALSE && character->left == SDL_FALSE) {
            slow_down(character);
        }

    }

    // Si le personnage ne bouge pas on n'effectue pas de déplacement ET DE COLLISION !!!
    if ((character->dx != 0 || character->dy != 0) && character->dash->duration == 0)
        move_character(character, character->dx, character->dy, map);
    // Si le personnage vient de faire un dash on divise sa vitesse par 20
}


SDL_bool in_action(Character *character) {
    return character->dash->duration > 0 || character->slide->duration > 0;
}


void go_crouch(Character *character, Map *map) {
    if  (character->wall_jump_left == SDL_FALSE && character->wall_jump_right == SDL_FALSE && change_size_collision(character, map, character->original_width, character->original_width)) {
        character->crouch = SDL_TRUE;
        character->height = character->original_width;
        character->width = character->original_width;
    }
}

void cancel_crouch(Character *character, Map *map) {
    if (change_size_collision(character, map, character->original_width, character->original_height)) {
        character->crouch = SDL_FALSE;
        character->height = character->original_height;
        character->width = character->original_width;
    }
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
    if (!in_action(character)) {
        if (character->just_landed && character->on_ground && character->down) {
            character->just_landed = SDL_FALSE;
            character->crouch = SDL_FALSE;
            //printf("right: %d left: %d\n", character->right, character->left);
            if (character->right == SDL_TRUE && character->left == SDL_FALSE) {
                if (change_size_collision(character, map, character->original_height, character->original_width)) {
                    character->height = character->original_width;
                    character->width = character->original_height;
                    slide->duration = 50;
                    slide->go_right = SDL_TRUE;
                } else {
                    printf("can't slide here\n");
                }
            } else if (character->left == SDL_TRUE && character->right == SDL_FALSE) {
                if (change_size_collision(character, map, character->original_height, character->original_width)) {
                    character->height = character->original_width;
                    character->width = character->original_height;
                    slide->duration = 50;
                    slide->go_left = SDL_TRUE;
                } else {
                    printf("can't slide here\n");
                }
            }
            // printf("slide duration: %d %d %d \n", slide->duration, slide->go_right, slide->go_left);

            character->just_landed = SDL_FALSE;
        }
    }
}

void handle_slide(Character *character, Map *map) {
    if (character->slide->duration > 0) {
        if (character->slide->go_right) {
            character->dx = (map->tile_width / 1.2) * character->speed;
        } else if (character->slide->go_left) {
            character->dx = -(map->tile_width / 1.2) * character->speed;
        } else {
            slide_cancel(character, map);
            return;
        }
        if (character->up && 0 <  character->slide->duration &&character->slide->duration < 20) {
            slide_cancel(character, map);
            move_character_up(character, map, map->tile_width, map->tile_height, 25);
        }
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
        //printf("cancel slide\n");
        character->height = character->original_height;
        character->width = character->original_width;
    } else {
        //printf("can't cancel slide\n");
        go_crouch(character, map);
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
    dash->on_air = SDL_TRUE;
    dash->go_up = SDL_TRUE;
    return dash;
}

void action_dash(Character *character, Controls *controls, Map *map) {
    // Handle dash key pressed
    Dash *dash = character->dash;
    if (!in_action(character)) {
        if (dash->cooldown == 0 && character->crouch == SDL_FALSE && (character->dx != 0 || character->dy != 0) ) {
            if (dash->on_air == SDL_TRUE || character->on_ground == SDL_TRUE) {
                if (change_size_collision(character, map, character->width, character->height/2)){
                    dash->duration = 25;
                    dash->cooldown = 300;
                    character->height=character->height/2;
                }

                if (character->right == SDL_TRUE && character->left == SDL_FALSE) {
                    dash->direction.x = 1;
                } else if (character->left == SDL_TRUE && character->right == SDL_FALSE) {
                    dash->direction.x = -1;
                } else if (character->right == SDL_TRUE && character->left == SDL_TRUE) {
                    if (character->dx > 0) {
                        dash->direction.x = 1;
                    } else if (character->dx < 0) {
                        dash->direction.x = -1;
                    } else {
                        dash->direction.x = 0;
                    }
                } else {
                    dash->direction.x = 0;
                }
                if (character->up == SDL_TRUE) {
                    dash->direction.y = -1;
                } else if (character->down == SDL_TRUE) {
                    dash->direction.y = 1;
                } else {
                    dash->direction.y = 0;
                }
            }
        }
        // printf("dash duration: %d %d %d \n", dash->duration, dash->direction.x, dash->direction.y);
    }
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
            if (change_size_collision(character, map, character->original_width, character->original_height)){
                character->height = character->original_height;
            } else {
                go_crouch(character, map);
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

void move_character_up(Character *character, Map *map, int tile_width, int tile_height, int bonus) {
    if (bonus != 0){
        printf("bonus: %d\n", bonus);
    }
    if (character->on_ground == SDL_TRUE) {// classic jump
        if (character->crouch){
            cancel_crouch(character, map);
        }
        if (character->slide->duration < 20 && change_size_collision(character, map, character->original_width, character->original_height)) {
            character->dy = -(tile_height * 2) - bonus;
            character->on_ground = SDL_FALSE;
            character->just_landed = SDL_FALSE;
        }
    } else if (character->wall_jump_right == SDL_TRUE) {
        if (!character->right) {//right wall jump (jump to the left)
            character->dy = -(tile_height * 2);
            character->dx = -(tile_width / 1.40) * character->speed;
            character->wall_right = SDL_FALSE;
            character->wall_jump_right = SDL_FALSE;
        }
        if (character->left) {
            character->wall_right = SDL_FALSE;
            character->wall_jump_right = SDL_FALSE;
        }
    } else if (character->wall_jump_left == SDL_TRUE) {
        if (!character->left) {//left wall jump (jump to the right)
            character->dy = -(tile_height * 2);
            character->dx = (tile_width / 1.40) * character->speed;
            character->wall_jump_left = SDL_FALSE;
            character->wall_left = SDL_FALSE;
        }
        if (character->right) {
            character->wall_left = SDL_FALSE;
            character->wall_jump_left = SDL_FALSE;
        }
    }

}

void move_character_down(Character *character, int tile_height) {
    character->dy = 250;// fall faster than max gravity
}

void move_character_left(Character *character, int tile_width) {
    if (character->dx > 0 && character->on_ground) {// if the character has right inertia
        character->dx = character->dx - 7;
    } else {
        if (character->crouch == SDL_TRUE) {
            character->dx = max(character->dx - 5, -(tile_width / 4) * character->speed);
        } else {
            character->dx = max(character->dx - 5, -(tile_width / 1.5) * character->speed);
        }
    }
}

void move_character_right(Character *character, int tile_width) {
    if (character->dx < 0 && character->on_ground) {// if the character has left inertia
        character->dx = character->dx + 7;
    } else {
        if (character->crouch == SDL_TRUE) {
            character->dx = min(character->dx + 5, (tile_width / 4) * character->speed);
        } else {
            character->dx = min(character->dx + 5, (tile_width / 1.5) * character->speed);
        }
    }
}

void reset_character_move(Character *character) {
    character->dx = 0;
    character->dy = 0;
    character->wall_right = SDL_FALSE;
    character->wall_left = SDL_FALSE;
    character->wall_jump_right = SDL_FALSE;
    character->wall_jump_left = SDL_FALSE;
}

void cancel_animations(Character *character) {
    slide_cancel(character, NULL);
}
