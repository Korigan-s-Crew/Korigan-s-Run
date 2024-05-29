#ifndef TEST_H
#define TEST_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "init.h"
#include "map.h"
#include "movement.h"
#include "music.h"
#include "draw.h"

int main();
void test_random_number();
void test_normalize_line();
void test_create_free_graph();
void test_matrice_vers_file_recup_matrice();
void test_parcours_graphe();
void test_creer_liberer_matrice();
void test_concatenerMatrices();
void test_concatStr();
void test_pattern_initialisation_free_pattern();
void test_create_map_txt();
void test_music_and_SDL();
void test_create_and_change_map();

#endif
