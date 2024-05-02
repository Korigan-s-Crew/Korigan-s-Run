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
