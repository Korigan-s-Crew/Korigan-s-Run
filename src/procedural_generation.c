#include "../include/procedural_generation.h"

#include <stdlib.h>

#include "../include/map.h"

Map *generated_pattern(Camera *camera, Character *character, Map *map, Graphe *graphe) {
	char *parcours = parcours_graphe(graphe);
	printf("%s\n", parcours);
	create_map_txt(parcours);
	Map *Pattern1 = create_map("test.txt", map->tile_width, map->tile_height);
	free(parcours);
	return Pattern1;
}

int seeded_random_number(int max) {
	// Cette fonction donne un nombre aléatoire entre 0 et max
	// en partant d'une seed choisie à la ligne 57 de main.c avec srand(time(NULL))
	int nb = rand() % max;
	srand(rand());
	return nb;
}
