#include "../include/map.h"

Collision gen_tile_collision(int type) {
    // Génère les collisions des tiles
    Collision collision;
    collision.up = SDL_TRUE;
    collision.down = SDL_TRUE;
    collision.left = SDL_TRUE;
    collision.right = SDL_TRUE;
    collision.traversableUp = SDL_FALSE;
    collision.traversableDown = SDL_FALSE;
    collision.traversableLeft = SDL_FALSE;
    collision.traversableRight = SDL_FALSE;
    collision.WallJumpable = SDL_FALSE;
    if (type < 0) {
        collision.up = SDL_FALSE;
        collision.down = SDL_FALSE;
        collision.left = SDL_FALSE;
        collision.right = SDL_FALSE;
    } else {
        switch (type / 10) {
            case 0:
                collision.up = SDL_FALSE;
                collision.down = SDL_FALSE;
                collision.left = SDL_FALSE;
                collision.right = SDL_FALSE;
                break;
            case 29:
                collision.up = SDL_TRUE;
                collision.down = SDL_FALSE;
                collision.left = SDL_FALSE;
                collision.right = SDL_FALSE;
                collision.traversableUp = SDL_TRUE;
                break;
            case 45:
                collision.WallJumpable = SDL_TRUE;
                break;
        }
    }
    return collision;
}

Tile create_tile(int x, int y, int width, int height, int type, int rotation) {
    // Crée une tile
    Tile tile;
    tile.x = x;
    tile.y = y;
    tile.width = width;
    tile.height = height;
    tile.type = type;
    tile.collision = gen_tile_collision(type);
    tile.rotation = rotation;
    return tile;
}

Map *create_map(char *path, int tile_width, int tile_height) {
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
    map->tile_width = tile_width;
    map->tile_height = tile_height;
    // Remplit le tableau avec des 0 pour éviter les problèmes de mémoire
    for (int i = 0; i < MAX_TILES; i++) {
        for (int j = 0; j < MAX_TILES; j++) {
            map->tiles[i][j] = create_tile(i, j, tile_width, tile_height, 0, 0);
        }
    }
    do {
        ch = fgetc(file);
        // printf("%c", ch);
        // Remplit le tableau avec les valeurs correspondantes aux caractères du fichier texte (voir map.txt pour plus d'infos)
        char tile_mapping[] = " #---------------------------@---------------|";
        if (ch == 'S') {
            map->tiles[height][width] = create_tile(height, width, tile_width, tile_height,
                                                    -50, 0);
            width++;

        }  // Si c'est un 0 on met -1 dans le tableau (case de départ du personnage)
        else if (ch == '0') {
            map->tiles[height][width] = create_tile(height, width, tile_width, tile_height, -1, 0);
            map->tile_start_x = width;
            map->tile_start_y = height;
            width++;
            // Si c'est un retour à la ligne on met à jour la largeur et la hauteur de la map
        } else if (ch == '\n') {
            max_width = max(max_width, width);
            width = 0;
            height++;
        } else {
            for (int i = 0; i < sizeof(tile_mapping) - 1; i++) {
                if (ch == tile_mapping[i]) {
                    int random_texture = rand() % 10;
                    srand(rand());
                    map->tiles[height][width] = create_tile(height, width, tile_width, tile_height,
                                                            i * 10 + random_texture,
                                                            0);

                    // exemple d'herbe/texture transparente relative
                    if (i == 1 && height > 0 && map->tiles[height - 1][width].type == 0) {
                        random_texture = rand() % 10;
                        srand(rand());
                        map->tiles[height - 1][width] = create_tile(height, width, tile_width, tile_height,
                                                                    -20 - random_texture, 0);
                    }
                    width++;
                    break;
                }
            }
        }
    } while (ch != EOF);
    // Quand on arrive à la fin du fichier on ajoute la dernière ligne
    max_width = max(max_width, width);
    width = 0;
    height++;
    // On met à jour la largeur et la hauteur de la map
    map->width = max_width;
    map->height = height;
    // Ferme le fichier
    fclose(file);
    //    printf("map width : %d, map height : %d\n", map->width, map->height);
    //    printf("MAX_TILES : %d\n", MAX_TILES);
    for (int i = 0; i < map->width; i++) {
        for (int j = 0; j < map->height; j++) {
            if (i > 0 && j > 0 && map->tiles[j - 1][i].type != -50) {
                // Texture de sol (#)
                if (i > 0 && j > 0 && (map->tiles[j][i].type) >= 10 && (map->tiles[j][i].type) < 20) {
                    if (map->tiles[j - 1][i].type < 10) {
                        if (map->tiles[j][i - 1].type < 10) {  // coin ext gauche
                            if (i < map->width - 1 && map->tiles[j][i + 1].type < 10) {
                                map->tiles[j][i].type += 500;
                            } else {
                                map->tiles[j][i].type = 50;
                                map->tiles[j - 1][i].type = -30;
                            }

                        } else if (i < map->width - 1 && map->tiles[j][i + 1].type < 10) {    // coin ext droit
                            map->tiles[j][i].type = 90;
                            map->tiles[j - 1][i].type = -40;
                        } else {
                            map->tiles[j - 1][i].type = -20 - (map->tiles[j][i].type) % 10;     // Herbe
                        }
                    } else if (map->tiles[j - 1][i - 1].type < 10 &&
                               map->tiles[j - 1][i].type >= 10 &&
                               map->tiles[j - 1][i].type < 290 &&
                               map->tiles[j][i - 1].type >= 10 &&
                               map->tiles[j][i - 1].type < 290) {  // coin interieur gauche
                        if (j > 1 && (map->tiles[j - 2][i].type < 10 || map->tiles[j - 2][i].type >= 290)) {
                            map->tiles[j][i].type += 160;
                        } else {
                            map->tiles[j][i].type += 240;
                        }
                    } else if (i < map->width &&
                               (map->tiles[j - 1][i + 1].type < 10 || map->tiles[j - 1][i + 1].type >= 290) &&
                               map->tiles[j - 1][i].type >= 10 &&
                               map->tiles[j - 1][i].type < 290 &&
                               map->tiles[j][i + 1].type >= 10 &&
                               map->tiles[j][i + 1].type < 290) {  // coin interieur droit
                        if (j > 1 && (map->tiles[j - 2][i].type < 10 || map->tiles[j - 2][i].type >= 290)) {
                            map->tiles[j][i].type += 120;
                        } else {
                            map->tiles[j][i].type += 200;
                        }
                    }
                }
                    // Textures de nuages (@)
                else if (i > 0 && j > 0 && (map->tiles[j][i].type) >= 290 && (map->tiles[j][i].type) < 300) {
                    // debut de nuage
                    if ((map->tiles[j][i - 1].type) < 290 || (map->tiles[j][i - 1].type) >= 360) {
                        if ((map->tiles[j][i + 1].type) < 290 || (map->tiles[j][i + 1].type) >= 300) {
                            map->tiles[j][i].type += 70;  // nuage seul
                            map->tiles[j - 1][i].type = map->tiles[j][i].type + 80;
                        } else if ((map->tiles[j][i + 2].type) < 290 || (map->tiles[j][i + 2].type) >= 300) {
                            map->tiles[j][i].type += 10;  // nuage double
                            map->tiles[j][i + 1].type += 40;
                        } else {
                            // printf("%d", rand());
                            if (rand() % 2 == 0) {
                                map->tiles[j][i].type += 10;
                            } else {
                                map->tiles[j][i].type += 20;
                                map->tiles[j][i + 1].type += 30;
                            }
                            srand(rand());
                        }
                    }
                        // Fin de nuage
                    else {
                        if ((map->tiles[j][i + 1].type) < 290 || (map->tiles[j][i + 1].type) > 300) {
                            map->tiles[j][i].type += 40;
                        } else if ((map->tiles[j][i + 2].type) < 290 || (map->tiles[j][i + 2].type) > 300) {
                            if (rand() % 2 == 0) {
                                map->tiles[j][i].type += 50;
                                map->tiles[j][i + 1].type += 60;
                            }
                            srand(rand());
                        }
                    }
                }
                    // Textures de murs (|)
                else if (i > 0 && j > 0 && (map->tiles[j][i].type) >= 450 && (map->tiles[j][i].type) < 460) {
                    if ((map->tiles[j + 1][i].type) >= 450 &&
                        (map->tiles[j + 1][i].type) < 490) {        // teste si c'est un mid
                        if ((map->tiles[j - 1][i].type) < 450 ||
                            (map->tiles[j - 1][i].type) >= 490) {    // teste si c'est un top
                            map->tiles[j][i].type += 10;
                        } else {
                            map->tiles[j][i].type = map->tiles[j - 1][i].type % 10 + 470;
                        }
                    } else {
                        map->tiles[j][i].type = map->tiles[j - 1][i].type % 10 + 480;
                    }
                }
            }
        }
    }
    // Textures dependantes
#define not_bottom(i, j) (                                                        \
    (map->tiles[(j) - 1][i].type >= 10 && map->tiles[(j) - 1][i].type <= 29) ||   \
    (map->tiles[(j) - 1][i].type >= 50 && map->tiles[(j) - 1][i].type <= 79) ||   \
    (map->tiles[(j) - 1][i].type >= 90 && map->tiles[(j) - 1][i].type <= 119) ||  \
    (map->tiles[(j) - 1][i].type >= 130 && map->tiles[(j) - 1][i].type <= 159) || \
    (map->tiles[(j) - 1][i].type >= 170 && map->tiles[(j) - 1][i].type <= 199) || \
    (map->tiles[(j) - 1][i].type >= 210 && map->tiles[(j) - 1][i].type <= 239) || \
    (map->tiles[(j) - 1][i].type >= 250 && map->tiles[(j) - 1][i].type <= 279))

    for (int i = 0; i < map->width; i++) {
        for (int j = 0; j < map->height; j++) {
            // textures descendantes

            if (j > 0 && (map->tiles[j][i].type) >= 10 && map->tiles[j][i].type < 20 &&
                map->tiles[j - 1][i].type >= 10 && map->tiles[j - 1][i].type < 290) {
                if (not_bottom(i, j)) {
                    map->tiles[j][i].type = map->tiles[j - 1][i].type + 10;
                } else {
                    map->tiles[j][i].type = 40;
                }
            } else if (j > 0 && map->tiles[j][i].type < 10 && map->tiles[j][i].type >= 0 &&
                       map->tiles[j - 1][i].type >= 10 && map->tiles[j - 1][i].type < 290) {
                map->tiles[j][i].type += 520;
                if (map->tiles[j - 1][i].type >= 50 && map->tiles[j - 1][i].type < 90) {
                    map->tiles[j][i].type += 10;
                } else if (map->tiles[j - 1][i].type >= 90 && map->tiles[j - 1][i].type < 120) {
                    map->tiles[j][i].type += 20;
                }
            } else if ((map->tiles[j + 1][i].type) >= 290 && (map->tiles[j + 1][i].type) < 360 &&
                       (map->tiles[j][i].type < 10) && map->tiles[j][i].type != -50) {
                map->tiles[j][i].type += (map->tiles[j + 1][i].type / 10 + 8) * 10;     // partie transparente du nuage
            }
        }
    }
    // On retourne la map
    return map;
}

int check_out_of_bounds(int width, int bounds) {
    // Prevent heap-buffer-overflow in the function collision
    if (width < 0) {
        //        printf("width out of bound: %d\n", width);
        return 0;
    } else if (width > bounds) {
        //        printf("width out of bound: %d\n", width);
        return bounds;
    }
    return width;
}

SDL_bool expand_right(Character *character, Map *map, int width, int height) {
    // Check if we can expand the character width to the right

    int x = character->x;
    int y = character->y;
    int tile_width = map->tile_width;
    int tile_height = map->tile_height;

    int cur_true_right = check_out_of_bounds((x + character->width - 1) / tile_width, map->width - 1);
    int next_true_right = check_out_of_bounds((x + width - 1) / tile_width, map->width - 1);

    if (next_true_right <= cur_true_right) {
        return SDL_TRUE;
    }

    // all of the different heights of the character
    int feet = check_out_of_bounds((y + height - 1) / tile_height, map->height - 1);
    // bottom of the character
    int body = check_out_of_bounds((y + height / 2) / tile_height, map->height - 1);
    // middle of the character i height
    int head = check_out_of_bounds(y / tile_height, map->height - 1);
    // a bit above the head of the character

    SDL_bool wall_right_body = SDL_FALSE;
    SDL_bool wall_right_head = SDL_FALSE;
    SDL_bool wall_right_feet = SDL_FALSE;

    if (map->tiles[body][next_true_right].collision.left) {
        if (map->tiles[body][next_true_right].collision.traversableLeft) {
            if (character->right) {     // if user is pressing right he can go through
                wall_right_body = SDL_TRUE;
            }
        }
    } else {
        wall_right_body = SDL_TRUE;
    }

    if (map->tiles[head][next_true_right].collision.left) {
        if (map->tiles[head][next_true_right].collision.traversableLeft) {
            if (character->right) {     // if user is pressing right he can go through
                wall_right_head = SDL_TRUE;
            }
        }
    } else {
        wall_right_head = SDL_TRUE;
    }

    if (map->tiles[feet][next_true_right].collision.left) {
        if (map->tiles[feet][next_true_right].collision.traversableLeft) {
            if (character->right) {     // if user is pressing right he can go through
                wall_right_feet = SDL_TRUE;
            }
        }
    } else {
        wall_right_feet = SDL_TRUE;
    }

    return wall_right_body && wall_right_head && wall_right_feet;
}

SDL_bool expand_left(Character *character, Map *map, int width, int height) {
    // Check if we can expand the character width to the left

    int x = character->x;
    int y = character->y;
    int tile_width = map->tile_width;
    int tile_height = map->tile_height;

    int cur_true_left = x / tile_width;
    int next_true_left = check_out_of_bounds((x - (width - character->width)) / tile_width, map->width - 1);

    if (next_true_left >= cur_true_left) {
        return SDL_TRUE;
    }

    // all of the different heights of the character
    int feet = check_out_of_bounds((y + height - 1) / tile_height, map->height - 1);
    // bottom of the character
    int body = check_out_of_bounds((y + height / 2) / tile_height, map->height - 1);
    // middle of the character i height
    int head = check_out_of_bounds((y - height / 15) / tile_height, map->height - 1);
    // a bit above the head of the character

    SDL_bool wall_left_body = SDL_FALSE;
    SDL_bool wall_left_head = SDL_FALSE;
    SDL_bool wall_left_feet = SDL_FALSE;

    if (map->tiles[body][next_true_left].collision.right) {
        if (map->tiles[body][next_true_left].collision.traversableRight) {
            if (character->left) {    // if user is pressing left he can go through
                wall_left_body = SDL_TRUE;
            }
        }
    } else {
        wall_left_body = SDL_TRUE;
    }

    if (map->tiles[head][next_true_left].collision.right) {
        if (map->tiles[head][next_true_left].collision.traversableRight) {
            if (character->left) {    // if user is pressing left he can go through
                wall_left_head = SDL_TRUE;
            }
        }
    } else {
        wall_left_head = SDL_TRUE;
    }

    if (map->tiles[feet][next_true_left].collision.right) {
        if (map->tiles[feet][next_true_left].collision.traversableRight) {
            if (character->left) {    // if user is pressing left he can go through
                wall_left_feet = SDL_TRUE;
            }
        }
    } else {
        wall_left_feet = SDL_TRUE;
    }

    return wall_left_body && wall_left_head && wall_left_feet;
}

SDL_bool expand_up(Character *character, Map *map, int width, int height) {
    // Check if we can expand the character width to the left

    int x = character->x;
    int y = character->y;
    int tile_width = map->tile_width;
    int tile_height = map->tile_height;

    int cur_true_head = y / tile_height;
    int next_true_head = check_out_of_bounds((y - (height - character->height)) / tile_height, map->height - 1);
    if (next_true_head >= cur_true_head) {
        return SDL_TRUE;
    }

    int true_right = check_out_of_bounds((x + width - 1) / tile_width, map->width - 1);
    // exact right of the character
    int true_left = x / tile_width;
    // exact left of the character
    int center = check_out_of_bounds((x + width / 2) / tile_width, map->width - 1);
    // center of the character in width

    SDL_bool wall_left_head = SDL_FALSE;
    SDL_bool wall_center_head = SDL_FALSE;
    SDL_bool wall_right_head = SDL_FALSE;

    if (map->tiles[next_true_head][true_left].collision.down) {
        if (map->tiles[next_true_head][true_left].collision.traversableDown) {
            if (character->up) {  // if user is pressing left he can go through
                wall_left_head = SDL_TRUE;
            }
        }
    } else {
        wall_left_head = SDL_TRUE;
    }

    if (map->tiles[next_true_head][center].collision.down) {
        if (map->tiles[next_true_head][center].collision.traversableDown) {
            if (character->up) {  // if user is pressing left he can go through
                wall_center_head = SDL_TRUE;
            }
        }
    } else {
        wall_center_head = SDL_TRUE;
    }

    if (map->tiles[next_true_head][true_right].collision.down) {
        if (map->tiles[next_true_head][true_right].collision.traversableDown) {
            if (character->up) {  // if user is pressing left he can go through
                wall_right_head = SDL_TRUE;
            }
        }
    } else {
        wall_right_head = SDL_TRUE;
    }
    return wall_left_head && wall_center_head && wall_right_head;
}

SDL_bool expand_down(Character *character, Map *map, int width, int height) {
    // Check if we can expand the character width to the left

    int x = character->x;
    int y = character->y;
    int tile_width = map->tile_width;
    int tile_height = map->tile_height;

    // printf("y: %d, height + y: %d, character->height + y : %d, tile_height : %d\n", y, height + y,character->height + y,map->tile_height);
    int cur_true_feet = (y + character->height - 1) / tile_height;
    int next_true_feet = check_out_of_bounds((y + height - 1) / tile_height, map->height - 1);
    // printf("cur_true_feet: %d, next_true_feet: %d\n", cur_true_feet, next_true_feet);
    if (next_true_feet <= cur_true_feet) {
        return SDL_TRUE;
    }

    int true_right = check_out_of_bounds((x + width - 1) / tile_width, map->width - 1);
    // exact right of the character
    int true_left = x / tile_width;
    // exact left of the character
    int center = check_out_of_bounds((x + width / 2) / tile_width, map->width - 1);
    // center of the character in width

    SDL_bool wall_left_feet = SDL_FALSE;
    SDL_bool wall_center_feet = SDL_FALSE;
    SDL_bool wall_right_feet = SDL_FALSE;

    if (map->tiles[next_true_feet][true_left].collision.up) {
        if (map->tiles[next_true_feet][true_left].collision.traversableUp) {
            if (character->down) {    // if user is pressing down he can go through
                wall_left_feet = SDL_TRUE;
            }
        }
    } else {
        wall_left_feet = SDL_TRUE;
    }

    if (map->tiles[next_true_feet][center].collision.up) {
        if (map->tiles[next_true_feet][center].collision.traversableUp) {
            if (character->down) {    // if user is pressing left he can go through
                wall_center_feet = SDL_TRUE;
            }
        }
    } else {
        wall_center_feet = SDL_TRUE;
    }

    if (map->tiles[next_true_feet][true_right].collision.up) {
        if (map->tiles[next_true_feet][true_right].collision.traversableUp) {
            if (character->down) {    // if user is pressing left he can go through
                wall_right_feet = SDL_TRUE;
            }
        }
    } else {
        wall_right_feet = SDL_TRUE;
    }
    // printf("wall_left_feet: %d bloc: %d,wall_center_feet: %d bloc: %d, wall_right_feet: %d bloc: %d\n", wall_left_feet, map->tiles[next_true_feet][true_left].type, wall_center_feet, map->tiles[next_true_feet][center].type, wall_right_feet, map->tiles[next_true_feet][true_right].type);
    return wall_left_feet && wall_center_feet && wall_right_feet;
}

SDL_bool change_size_collision(Character *character, Map *map, int width, int height) {
    if (expand_up(character, map, width, height)) {
        // printf("expand up\n");
        if (expand_right(character, map, width, height)) {
            character->y = (character->y - (height - character->height));
            // printf("expand right\n");
            return SDL_TRUE;
        } else if (expand_left(character, map, width, height)) {
            character->y = character->y - (height - character->height);
            character->x = character->x - (width - character->width);
            // printf("expand left\n");
            return SDL_TRUE;
        }
    } else if (expand_down(character, map, width, height)) {
        // printf("expand down\n");
        if (expand_right(character, map, width, height)) {
            // printf("expand right\n");
            return SDL_TRUE;
        } else if (expand_left(character, map, width, height)) {
            printf("expand left\n");
            character->x = character->x - (width - character->width);
            return SDL_TRUE;
        }
    }

    return SDL_FALSE;
}

void collision(Character *character, Map *map) {
    // Gère les collisions entre le personnage et la map
    int x = character->x;
    int y = character->y;
    int width = character->width;
    int height = character->height;
    int tile_width = map->tile_width;
    int tile_height = map->tile_height;
    // int y_tile = y / height;
    //    printf("x: %d, y: %d, width: %d, height: %d\n", x, y, width, height);
    // all of the different heights of the character
    int feet = check_out_of_bounds((y + height) / tile_height, map->height - 1);
    // bottom of the character
    int ankle = check_out_of_bounds((y + height - 1) / tile_height, map->height - 1);
    // 1 pixel above the bottom of the character
    int knee = check_out_of_bounds((int) (y + height * 0.95) / tile_height, map->height - 1);
    int body = check_out_of_bounds((y + height / 2) / tile_height, map->height - 1);
    // middle of the character i height
    int neck = check_out_of_bounds((y + height / 15) / tile_height, map->height - 1);
    // a bit under the head of the character
    int head = check_out_of_bounds((y - height / 15) / tile_height, map->height - 1);
    // a bit above the head of the character

    //    printf("feet: %d, knee: %d, body: %d, neck: %d, head: %d\n", feet, knee, body, neck, head);

    // all of the different widths of the character
    int true_right = check_out_of_bounds((x + width - 1) / tile_width, map->width - 1);
    // exact right of the character
    int true_left = x / tile_width;
    // exact left of the character
    int right = check_out_of_bounds((int) (x + width * 1.05) / tile_width, map->width - 1);
    // 5% more to the right of the character
    int left = check_out_of_bounds((x - width / 15) / tile_width, map->width - 1);
    // 5% more to the left of the character
    int center = check_out_of_bounds((x + width / 2) / tile_width, map->width - 1);
    // center of the character in width

    //    printf("true_right: %d, true_left: %d, right: %d, left: %d, center: %d\n", true_right, true_left, right, left, center);

    SDL_bool on_ground_right = SDL_TRUE;
    SDL_bool on_ground_left = SDL_TRUE;
    SDL_bool on_ground_center = SDL_TRUE;
    SDL_bool was_on_ground = character->on_ground;

    // printf("x_tile: %d, y_tile: %d\n", x_tile, y_tile);

    // Si le personnage à les pieds sur le sol côté gauche et que sa vitesse verticale est positive
    if (map->tiles[feet][true_left].collision.up) {
        if (map->tiles[feet][true_left].collision.traversableUp) {
            if (character->down || map->tiles[ankle][true_left].collision.traversableUp) {
                if (!map->tiles[ankle][true_left].collision.traversableUp) {
                    printf("character go through down left dy : %d\n", character->dy);
                }
                on_ground_left = SDL_FALSE;
            } else {
                if (character->dy > 0) {
                    character->dy = 0;
                }
                if (was_on_ground == SDL_FALSE) {
                    character->just_landed = SDL_TRUE;
                } else {
                    character->just_landed = SDL_FALSE;
                }
                character->on_ground = SDL_TRUE;
            }
        } else {
            if (character->dy > 0) {
                character->dy = 0;
            }
            if (was_on_ground == SDL_FALSE) {
                character->just_landed = SDL_TRUE;
            } else {
                character->just_landed = SDL_FALSE;
            }
            character->on_ground = SDL_TRUE;
        }
    } else {
        on_ground_left = SDL_FALSE;
    }
    // Si le personnage à les pieds sur le sol au centre et que sa vitesse verticale est positive
    if (map->tiles[feet][center].collision.up) {
        if (map->tiles[feet][center].collision.traversableUp) {
            if (character->down || map->tiles[ankle][center].collision.traversableUp) {
                if (!map->tiles[ankle][center].collision.traversableUp) {
                    printf("character go through down center dy : %d\n", character->dy);
                }
                on_ground_center = SDL_FALSE;
            } else {
                if (character->dy > 0) {
                    character->dy = 0;
                }
                if (was_on_ground == SDL_FALSE) {
                    character->just_landed = SDL_TRUE;
                } else {
                    character->just_landed = SDL_FALSE;
                }
                character->on_ground = SDL_TRUE;
            }
        } else {
            if (character->dy > 0) {
                character->dy = 0;
            }
            if (was_on_ground == SDL_FALSE) {
                character->just_landed = SDL_TRUE;
            } else {
                character->just_landed = SDL_FALSE;
            }
            character->on_ground = SDL_TRUE;
        }
    } else {
        on_ground_center = SDL_FALSE;
    }
    // Si le personnage à les pieds sur le sol côté droite et que sa vitesse verticale est positive
    if (map->tiles[feet][true_right].collision.up) {
        if (map->tiles[feet][true_right].collision.traversableUp) {
            if (character->down || map->tiles[ankle][true_right].collision.traversableUp) {
                if (!map->tiles[ankle][true_right].collision.traversableUp) {
                    printf("character go through down right dy : %d\n", character->dy);
                }
                on_ground_right = SDL_FALSE;
            } else {
                if (character->dy > 0) {
                    character->dy = 0;
                }
                if (was_on_ground == SDL_FALSE) {
                    character->just_landed = SDL_TRUE;
                } else {
                    character->just_landed = SDL_FALSE;
                }
                character->on_ground = SDL_TRUE;
            }
        } else {
            if (character->dy > 0) {
                character->dy = 0;
            }
            if (was_on_ground == SDL_FALSE) {
                character->just_landed = SDL_TRUE;
            } else {
                character->just_landed = SDL_FALSE;
            }
            character->on_ground = SDL_TRUE;
        }
    } else {
        on_ground_right = SDL_FALSE;
    }
    // Si le personnage n'est pas sur le sol côté gauche et côté droit alors il n'est pas sur le sol
    if (on_ground_right == SDL_FALSE && on_ground_center == SDL_FALSE && on_ground_left == SDL_FALSE) {
        character->on_ground = SDL_FALSE;
    }
    SDL_bool wall_right_feet = SDL_TRUE;
    SDL_bool wall_right_knee = SDL_TRUE;
    SDL_bool wall_right_body = SDL_TRUE;
    SDL_bool wall_right_neck = SDL_TRUE;
    SDL_bool wall_right_head = SDL_TRUE;

    if (wall_right_body == SDL_FALSE && wall_right_knee == SDL_FALSE && wall_right_neck == SDL_FALSE &&
        wall_right_head == SDL_FALSE && wall_right_feet == SDL_FALSE) {
        // just to avoid warning
    }
    // Si le centre du personnage rentre dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[body][right].collision.left) {
        if (map->tiles[body][right].collision.traversableLeft) {
            if (character->right) {     // if user is pressing right he can go through
                wall_right_body = SDL_FALSE;
            } else {
                if (character->dx > 0) {
                    character->dx = 0;
                }
                character->wall_right = SDL_TRUE;
                if (map->tiles[body][right].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                    character->wall_jump_right = SDL_TRUE;
                }
            }
        } else {
            if (character->dx > 0) {
                character->dx = 0;
            }
            character->wall_right = SDL_TRUE;
            if (map->tiles[body][right].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                character->wall_jump_right = SDL_TRUE;
            }
        }
    } else {
        wall_right_body = SDL_FALSE;
    }
    // Si les genoux du personnage rentrent dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[knee][right].collision.left) {
        if (map->tiles[knee][right].collision.traversableLeft) {
            if (character->right) {     // if user is pressing right he can go through
                wall_right_knee = SDL_FALSE;
            } else {
                if (character->dx > 0) {
                    character->dx = 0;
                }
                character->wall_right = SDL_TRUE;
                if (map->tiles[body][right].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                    character->wall_jump_right = SDL_TRUE;
                }
            }
        } else {
            if (character->dx > 0) {
                character->dx = 0;
            }
            character->wall_right = SDL_TRUE;
            if (map->tiles[body][right].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                character->wall_jump_right = SDL_TRUE;
            }
        }
    } else {
        wall_right_knee = SDL_FALSE;
    }
    // Si le coup du personnage rentre dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[neck][right].collision.left) {
        if (map->tiles[neck][right].collision.traversableLeft) {
            if (character->right) {     // if user is pressing right he can go through
                wall_right_neck = SDL_FALSE;
            } else {
                if (character->dx > 0) {
                    character->dx = 0;
                }
                character->wall_right = SDL_TRUE;
                if (map->tiles[body][right].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                    character->wall_jump_right = SDL_TRUE;
                }
            }
        } else {
            if (character->dx > 0) {
                character->dx = 0;
            }
            character->wall_right = SDL_TRUE;
            if (map->tiles[body][right].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                character->wall_jump_right = SDL_TRUE;
            }
        }
    } else {
        wall_right_neck = SDL_FALSE;
    }

    if (wall_right_body == SDL_FALSE) {
        character->wall_right = SDL_FALSE;
        character->wall_jump_right = SDL_FALSE;
    }

    SDL_bool wall_left_feet = SDL_TRUE;
    SDL_bool wall_left_knee = SDL_TRUE;
    SDL_bool wall_left_body = SDL_TRUE;
    SDL_bool wall_left_neck = SDL_TRUE;
    SDL_bool wall_left_head = SDL_TRUE;

    if (wall_left_body == SDL_FALSE && wall_left_knee == SDL_FALSE && wall_left_neck == SDL_FALSE &&
        wall_left_head == SDL_FALSE && wall_left_feet == SDL_FALSE) {
        // just to avoid warning
    }

    // Si le centre du personnage rentre dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[body][left].collision.right) {
        if (map->tiles[body][left].collision.traversableRight) {
            if (character->left) {    // if user is pressing left he can go through
                wall_left_body = SDL_FALSE;
            } else {
                if (character->dx < 0) {
                    character->dx = 0;
                }
                character->wall_left = SDL_TRUE;
                if (map->tiles[body][left].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                    character->wall_jump_left = SDL_TRUE;
                }
            }
        } else {
            if (character->dx < 0) {
                character->dx = 0;
            }
            character->wall_left = SDL_TRUE;
            if (map->tiles[body][left].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                character->wall_jump_left = SDL_TRUE;
            }
        }
    } else {
        wall_left_body = SDL_FALSE;
    }
    // Si les genoux du personnage rentrent dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[knee][left].collision.right) {
        if (map->tiles[knee][left].collision.traversableRight) {
            if (character->left) {    // if user is pressing left he can go through
                wall_left_knee = SDL_FALSE;
            } else {
                if (character->dx < 0) {
                    character->dx = 0;
                }
                character->wall_left = SDL_TRUE;
                if (map->tiles[body][left].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                    character->wall_jump_left = SDL_TRUE;
                }
            }
        } else {
            if (character->dx < 0) {
                character->dx = 0;
            }
            character->wall_left = SDL_TRUE;
            if (map->tiles[body][left].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                character->wall_jump_left = SDL_TRUE;
            }
        }
    } else {
        wall_left_knee = SDL_FALSE;
    }
    // Si le coup du personnage rentre dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[neck][left].collision.right) {
        if (map->tiles[neck][left].collision.traversableRight) {
            if (character->left) {    // if user is pressing left he can go through
                wall_left_neck = SDL_FALSE;
            } else {
                if (character->dx < 0) {
                    character->dx = 0;
                }
                character->wall_left = SDL_TRUE;
                if (map->tiles[body][left].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                    character->wall_jump_left = SDL_TRUE;
                }
            }
        } else {
            if (character->dx < 0) {
                character->dx = 0;
            }
            character->wall_left = SDL_TRUE;
            if (map->tiles[body][left].collision.WallJumpable && character->on_ground == SDL_FALSE) {
                character->wall_jump_left = SDL_TRUE;
            }
        }
    } else {
        wall_left_neck = SDL_FALSE;
    }

    if (wall_left_body == SDL_FALSE) {
        character->wall_left = SDL_FALSE;
        character->wall_jump_left = SDL_FALSE;
    }

    // Si la tête côté droit du personnage alors on annule sa vitesse verticale
    if (map->tiles[head][true_right].collision.down) {
        if (map->tiles[head][true_right].collision.traversableDown) {
            if (character->up) {  // if user is pressing up he can go through
                // may be usefull for wallslide
            } else {
                if (character->dy < 0) {
                    character->dy = 0;
                }
                // todo wallslide
            }
        } else {
            if (character->dy < 0) {
                character->dy = 0;
            }
            // wallslide
        }
    } else {
        // wallslide
    }
    // Si la tête côté gauche du personnage rentre dans le mur de gauche alors on annule sa vitesse verticale
    if (map->tiles[head][true_left].collision.down) {
        if (map->tiles[head][true_left].collision.traversableDown) {
            if (character->up) {  // if user is pressing right he can go through
                // may be usefull for wallslide
            } else {
                if (character->dy < 0) {
                    character->dy = 0;
                }
            }
        } else {
            if (character->dy < 0) {
                character->dy = 0;
            }
            // wallslide
        }
    } else {
        // wallslide
    }
    // Si le personnage est en dehors de la map en sortant par le bas (c'est à dire tombé dans un trou) alors on annule sa vitesse vertical
    if (character->y + character->height > map->height * tile_height) {     // todo handle fall
        if (character->dy > 0) {
            character->alive = SDL_FALSE;
            character->dy = 0;
        }
    }
    // Si le personnage est en dehors de la map par la droite alors on annule sa vitesse horizontal
    if (character->x > map->width * tile_width) {  // todo not working
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

    if (map->tiles[feet][left].type / 10 == -5) {
        character->on_portal = SDL_TRUE;
        character->key_suggestion = SDLK_e;
    } else if (map->tiles[head][left].type / 10 == -5) {
        character->on_portal = SDL_TRUE;
        character->key_suggestion = SDLK_e;
    } else if (map->tiles[feet][right].type / 10 == -5) {
        character->on_portal = SDL_TRUE;
        character->key_suggestion = SDLK_e;
    } else if (map->tiles[head][right].type / 10 == -5) {
        character->on_portal = SDL_TRUE;
        character->key_suggestion = SDLK_e;
    } else if ((map->tiles[body][center].type / 10 == -5)) {
        character->on_portal = SDL_TRUE;
        character->key_suggestion = SDLK_e;
    } else {
        character->on_portal = SDL_FALSE;
        if (character->key_suggestion == SDLK_e) {
            character->key_suggestion = SDLK_F15;
        }
    }
}

void add_right_pattern_to_map(Map *pattern, Map *map) {
    // Si la map est pleine alors on ajoute une autre map à droite qui est la dernière
    if (MAX_TILES - 2 < map->width + pattern->width) {
        map->full = SDL_TRUE;
        printf("map is full\n");
        free(pattern);
        Map *last_pattern = create_map("Patterns/last.txt", map->tile_width, map->tile_height);
        // Si la map est complétement pleine alors on ne fait rien (cas si le dernier pattern est exactement la taille de la map)
        print_map(map);
        if (MAX_TILES < map->width + last_pattern->width) {
            printf("map full tiles: %d width : %d patternwidth : %d", MAX_TILES, map->width, last_pattern->width);
            free(last_pattern);
            return;
        } else {
            // Sinon on ajoute le dernier pattern à droite de la map
            // Si la hauteur du dernier pattern est plis petite que la hauteur de la map alors on complète le dernier pattern avec des 0
            if (map->height > last_pattern->height) {
                for (int i = last_pattern->height; i < map->height; i++) {
                    for (int j = 0; j < last_pattern->width; j++) {
                        last_pattern->tiles[i][j].type = 0;
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
                    pattern->tiles[i][j].type = 0;
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

Map *change_map(Map *map, char *map_name, Character *character, Camera *camera, int tile_width, int tile_height) {
    // Change la map et remet le personnage à la position de départ de la nouvelle map et déplace la camera
    free(map);
    map = create_map(map_name, tile_width, tile_height);
    character->x = map->tile_start_x * tile_width;
    character->y = map->tile_start_y * tile_height;
    move_camera(camera, character, map);
    collision(character, map);
    character->next_map = SDL_FALSE;
    return map;
}
