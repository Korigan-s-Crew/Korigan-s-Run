#include "../include/procedural_generation.h"

#include <stdlib.h>

#include "../include/map.h"

Map *generated_pattern(Camera *camera, Character *character, Map *map) {
    int last_pattern_index = 0;
    // last_pattern_index est l'index du dernier pattern généré
    // On peut utiliser cette variable pour savoir quel pattern générer à la suite
    while (camera->pattern_generated_history[last_pattern_index] != -1) {
        last_pattern_index++;
    }
    // ICI, on génère un nombre aléatoire entre 0 et 100
    printf("random number: %d\n", seeded_random_number(100));
    if (last_pattern_index < 100) {
        // On ajoute 1 qui représente le pattern généré à la suite de l'index du dernier pattern généré
        // ICI, 1 est un example, il faudra remplacer par l'index du pattern généré
        camera->pattern_generated_history[last_pattern_index + 1] = 1;
    }
    // ICI, on retourne le pattern généré
    Map *Pattern1 = create_map("Patterns/pattern.txt", map->tile_width, map->tile_height);
    return Pattern1;
}

int seeded_random_number(int max) {
    // Cette fonction donne un nombre aléatoire entre 0 et max
    // en partant d'une seed choisie à la ligne 57 de main.c avec srand(time(NULL))
    int nb = rand() % max;
    srand(rand());
    return nb;
}
