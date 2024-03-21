#include "../include/map.h"

Collision gen_tile_collision(int type) {
    // Génère les collisions des tiles
    Collision collision;
    collision.up = SDL_FALSE;
    collision.down = SDL_FALSE;
    collision.left = SDL_FALSE;
    collision.right = SDL_FALSE;
    collision.traversableUp = SDL_FALSE;
    collision.traversableDown = SDL_FALSE;
    collision.traversableLeft = SDL_FALSE;
    collision.traversableRight = SDL_FALSE;
    switch (type / 10) {
        case 1:
            collision.up = SDL_TRUE;
            collision.down = SDL_TRUE;
            collision.left = SDL_TRUE;
            collision.right = SDL_TRUE;
            break;
        case 4:
            collision.up = SDL_TRUE;
            collision.traversableUp = SDL_TRUE;
            break;
    }
    return collision;
}

Tile create_tile(int x, int y, int width, int height, int type, SDL_Texture *texture, int rotation) {
    // Crée une tile
    Tile tile;
    tile.x = x;
    tile.y = y;
    tile.width = width;
    tile.height = height;
    tile.type = type;
    tile.texture = texture;
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
            map->tiles[i][j] = create_tile(i, j, tile_width, tile_height, 0, NULL, 0);
        }
    }
    do {
        ch = fgetc(file);
        // printf("%c", ch);
        // Remplit le tableau avec les valeurs correspondantes aux caractères du fichier texte (voir map.txt pour plus d'infos)
        char tile_mapping[] = " #cC@G[{(D]})";

        for (int i = 0; i < sizeof(tile_mapping) - 1; i++) {
            if (ch == tile_mapping[i]) {
                int random_texture = rand() % 10;
                srand(rand());
                map->tiles[height][width] = create_tile(height, width, tile_width, tile_height, i * 10 + random_texture,
                                                        NULL, 0);

                //exemple d'herbe/texture transparente relative
                if (i == 1 && height > 0 && map->tiles[height - 1][width].type == 0) {
                    random_texture = rand() % 10;
                    srand(rand());
                    map->tiles[height - 1][width] = create_tile(height, width, tile_width, tile_height,
                                                                -20 - random_texture, NULL, 0);
                }
                width++;
                break;
            }
        }
        // Si c'est un 0 on met -1 dans le tableau (case de départ du personnage)
        if (ch == '0') {
            map->tiles[height][width] = create_tile(height, width, tile_width, tile_height, -1, NULL, 0);
            map->tile_start_x = width;
            map->tile_start_y = height;
            width++;
        }
        // Si c'est un retour à la ligne on met à jour la largeur et la hauteur de la map
        if (ch == '\n') {
            max_width = max(max_width, width);
            width = 0;
            height++;
        }
    } while (ch != EOF);
    // Ferme le fichier
    fclose(file);
    for (int i = 0; i < MAX_TILES; i++) {
        for (int j = 0; j < MAX_TILES; j++) {
            if (i > 0 && (map->tiles[j][i].type) >= 10 && (map->tiles[j][i].type) < 20 &&
                (map->tiles[j][i - 1].type) < 10) {
                map->tiles[j][i].type = 50;
            } else if (i > 0 && (map->tiles[j][i].type) >= 10 && (map->tiles[j][i].type) < 20 &&
                       (map->tiles[j][i + 1].type) < 10) {
                map->tiles[j][i].type = 90;
            }
            if (j > 0 && (map->tiles[j][i].type) >= 10 && (map->tiles[j - 1][i].type) >= 10) {
                if ((map->tiles[j - 1][i].type) > 9 && map->tiles[j - 1][i].type <= 29) {
                    map->tiles[j][i].type = map->tiles[j - 1][i].type + 10;
                } else if ((map->tiles[j - 1][i].type) >= 50 && map->tiles[j - 1][i].type <= 79) {
                    map->tiles[j][i].type = map->tiles[j - 1][i].type + 10;
                } else if ((map->tiles[j - 1][i].type) >= 90 && map->tiles[j - 1][i].type <= 119) {
                    map->tiles[j][i].type = map->tiles[j - 1][i].type + 10;
                } else { map->tiles[j][i].type = 40; }
            }
        }
    }
    // Quand on arrive à la fin du fichier on ajoute la dernière ligne
    max_width = max(max_width, width);
    width = 0;
    height++;
    // On met à jour la largeur et la hauteur de la map
    map->width = max_width;
    map->height = height;
    // On retourne la map
    return map;
}

//CharHitbox get_char_hitbox(Character *character, int tile_width, int tile_height) {
// Récupère la hitbox du personnage
//    CharHitbox hitbox;
//    int x = character->x;
//    int y = character->y;
//    int width = character->width;
//    int height = character->height;
//    int feet = y + height;
//    int body = y + height / 2;
//    int x_center = x + width / 2;
//    int x_tile = x / tile_width;
//    // int y_tile = y / height;
//    int  feet = feet / tile_height;
//    int  knee = (int) (y + height * 0.95) / tile_height;
//    int y_tile_center = body / tile_height;
//    int  neck = (y + height / 15) / tile_height;
//    int  head = (y - height / 15) / tile_height;
//    int  true_right = (x + width) / tile_width;
//    int  right = (int) (x + width * 1.05) / tile_width;
//    int  left = (x - width / 15) / tile_width;
//    int  center = x_center / tile_width;
//    hitbox.head = {{ left, head}, { center, head}, { right, head}};
//    SDL_bool on_ground_right = SDL_TRUE;
//    SDL_bool on_ground_left = SDL_TRUE;
//    SDL_bool on_ground_center = SDL_TRUE;
//    return hitbox;
//}

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
    int feet = check_out_of_bounds((y + height) / tile_height, map->height - 1); // bottom of the character
    int knee = check_out_of_bounds((int) (y + height * 0.95) / tile_height, map->height - 1); // knees of the character
    int body = check_out_of_bounds((y + height / 2) / tile_height, map->height - 1);
    int neck = check_out_of_bounds((y + height / 15) / tile_height, map->height - 1);
    int head = check_out_of_bounds((y - height / 15) / tile_height, map->height - 1);
//    printf("feet: %d, knee: %d, body: %d, neck: %d, head: %d\n", feet, knee, body, neck, head);
    // all of the different widths of the character
    int true_right = check_out_of_bounds((x + width) / tile_width, map->width - 1);
    int true_left = x / tile_width;
    int right = check_out_of_bounds((int) (x + width * 1.05) / tile_width, map->width - 1);
    int left = check_out_of_bounds((x - width / 15) / tile_width, map->width - 1);
    int center = check_out_of_bounds((x + width / 2) / tile_width, map->width - 1);
//    printf("true_right: %d, true_left: %d, right: %d, left: %d, center: %d\n", true_right, true_left, right, left, center);

    SDL_bool on_ground_right = SDL_TRUE;
    SDL_bool on_ground_left = SDL_TRUE;
    SDL_bool on_ground_center = SDL_TRUE;

    // printf("x_tile: %d, y_tile: %d\n", x_tile, y_tile);

    // Si le personnage à les pieds sur le sol côté gauche et que sa vitesse verticale est positive
    if (map->tiles[feet][true_left].collision.up) {
        if (map->tiles[feet][true_left].collision.traversableUp) {
            if (character->down) {
                printf("character go through down left dy : %d\n", character->dy);
                on_ground_left = SDL_FALSE;
            }else{
                if (character->dy >0){
                    character->dy = 0;
                }
                character->on_ground = SDL_TRUE;
            }
        } else {
            if (character->dy >0){
                character->dy = 0;
            }
            character->on_ground = SDL_TRUE;
        }
    } else {
        on_ground_left = SDL_FALSE;
    }
    // Si le personnage à les pieds sur le sol au centre et que sa vitesse verticale est positive
    if (map->tiles[feet][center].collision.up) {
        if (map->tiles[feet][center].collision.traversableUp) {
            if (character->down) {
                printf("character go through down center dy : %d\n", character->dy);
                on_ground_center = SDL_FALSE;
            }else{
                if (character->dy >0){
                    character->dy = 0;
                }
                character->on_ground = SDL_TRUE;
            }
        } else {
            if (character->dy >0){
                character->dy = 0;
            }
            character->on_ground = SDL_TRUE;
        }
    } else {
        on_ground_center = SDL_FALSE;
    }
    // Si le personnage à les pieds sur le sol côté droite et que sa vitesse verticale est positive
    if (map->tiles[feet][true_right].collision.up) {
        if (map->tiles[feet][true_right].collision.traversableUp) {
            if (character->down) {
                printf("character go through down right dy : %d\n", character->dy);
                on_ground_right = SDL_FALSE;
            }else{
                if (character->dy >0){
                    character->dy = 0;
                }
                character->on_ground = SDL_TRUE;
            }
        } else {
            if (character->dy >0){
                character->dy = 0;
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
    // Si le centre du personnage rentre dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[body][right].collision.left) {
        if (map->tiles[body][right].collision.traversableLeft) {
            if (character->right) { // if user is pressing right he can go through
                //may be usefull for wallslide
            }else{
                if (character->dx >0){
                    character->dx = 0;
                }
                //todo wallslide
            }
        } else {
            if (character->dx >0){
                character->dx = 0;
            }
            // wallslide
        }
    } else {
        //wallslide
    }
    // Si les genoux du personnage rentrent dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[knee][right].collision.left) {
        if (map->tiles[knee][right].collision.traversableLeft) {
            if (character->right) { // if user is pressing right he can go through
                //may be usefull for wallslide
            }else{
                if (character->dx >0){
                    character->dx = 0;
                }
                //todo wallslide
            }
        } else {
            if (character->dx >0){
                character->dx = 0;
            }
            // wallslide
        }
    } else {
        //wallslide
    }
    // Si le coup du personnage rentre dans le mur de droite alors on annule sa vitesse horizontale
    if (map->tiles[neck][right].collision.left) {
        if (map->tiles[neck][right].collision.traversableLeft) {
            if (character->right) { // if user is pressing right he can go through
                //may be usefull for wallslide
            }else{
                if (character->dx >0){
                    character->dx = 0;
                }
                //todo wallslide
            }
        } else {
            if (character->dx >0){
                character->dx = 0;
            }
            // wallslide
        }
    } else {
        //wallslide
    }
    // Si le centre du personnage rentre dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[body][left].collision.right) {
        if (map->tiles[body][left].collision.traversableRight) {
            if (character->left) { // if user is pressing right he can go through
                //may be usefull for wallslide
            }else{
                if (character->dx < 0){
                    character->dx = 0;
                }
                //todo wallslide
            }
        } else {
            if (character->dx < 0){
                character->dx = 0;
            }
            // wallslide
        }
    } else {
        //wallslide
    }
    // Si les genoux du personnage rentrent dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[knee][left].collision.right) {
        if (map->tiles[knee][left].collision.traversableRight) {
            if (character->left) { // if user is pressing right he can go through
                //may be usefull for wallslide
            }else{
                if (character->dx < 0){
                    character->dx = 0;
                }
                //todo wallslide
            }
        } else {
            if (character->dx < 0){
                character->dx = 0;
            }
            // wallslide
        }
    } else {
        //wallslide
    }
    // Si le coup du personnage rentre dans le mur de gauche alors on annule sa vitesse horizontale
    if (map->tiles[neck][left].collision.right) {
        if (map->tiles[neck][left].collision.traversableRight) {
            if (character->left) { // if user is pressing right he can go through
                //may be usefull for wallslide
            }else{
                if (character->dx < 0){
                    character->dx = 0;
                }
                //todo wallslide
            }
        } else {
            if (character->dx < 0){
                character->dx = 0;
            }
            // wallslide
        }
    } else {
        //wallslide
    }
    // Si la tête côté droit du personnage alors on annule sa vitesse verticale
    if (map->tiles[head][true_right].collision.down) {
        if (map->tiles[head][true_right].collision.traversableDown) {
            if (character->up) { // if user is pressing right he can go through
                //may be usefull for wallslide
            }else{
                if (character->dy < 0){
                    character->dy = 0;
                }
                //todo wallslide
            }
        } else {
            if (character->dy < 0){
                character->dy = 0;
            }
            // wallslide
        }
    } else {
        //wallslide
    }
    // Si la tête côté gauche du personnage rentre dans le mur de gauche alors on annule sa vitesse verticale
    if (map->tiles[head][true_left].collision.down) {
        if (map->tiles[head][true_left].collision.traversableDown) {
            if (character->up) { // if user is pressing right he can go through
                //may be usefull for wallslide
            }else{
                if (character->dy < 0){
                    character->dy = 0;
                }
                //todo wallslide
            }
        } else {
            if (character->dy < 0){
                character->dy = 0;
            }
            // wallslide
        }
    } else {
        //wallslide
    }
    // Si le personnage est en dehors de la map en sortant par le bas (c'est à dire tombé dans un trou) alors on annule sa vitesse vertical
    if (character->y > map->height * tile_height) {
        if (character->dy > 0) {
            character->dy = 0;
        }
    }
    // Si le personnage est en dehors de la map par la droite alors on annule sa vitesse horizontal
    if (character->x > map->width * tile_width) {
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
}

void add_right_pattern_to_map(Map *pattern, Map *map) {
    // Si la map est pleine alors on ajoute une autre map à droite qui est la dernière
    if (MAX_TILES < map->width + pattern->width) {
        map->full = SDL_TRUE;
        printf("map is full\n");
        free(pattern);
        Map *last_pattern = create_map("Patterns/last_pattern.txt",map->tile_width,map->tile_height);
        // Si la map est complétement pleine alors on ne fait rien (cas si le dernier pattern est exactement la taille de la map)
        if (MAX_TILES < map->width + last_pattern->width) {
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
