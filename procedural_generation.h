#ifndef PROCEDURAL_GENERATION_H
#define PROCEDURAL_GENERATION_H
#include "main.h"
Map *generated_pattern(Camera *camera, Character *character, Map *map);
int seeded_random_number(int max);
#endif
