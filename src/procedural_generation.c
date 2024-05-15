#include "../include/procedural_generation.h"

#include <stdlib.h>

#include "../include/map.h"

Map *generated_pattern(Camera *camera, Character *character, Map *map) {
	Map *Pattern1 = create_map("Patterns/pattern.txt", map->tile_width, map->tile_height);
	return Pattern1;
}
