#ifndef IMPLEM_MAP_H
#define IMPLEM_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"

#define N 40			// nbre de sommets du graphe
#define T 5				// nbre de patterns dans la map

// Structures
struct Graphe {
	struct Noeuds* noeud[N];
};

typedef struct Graphe Graphe;

struct Noeuds {
	int destination;
	int poids;
	struct Noeuds* suivant;
};

struct Arete {
	int source;
	int destination;
	int poids;
};

struct Matrice {
	int** data;	 // Tableau 2D de données
	int rows;	 // Nombre de lignes
	int cols;	 // Nombre de colonnes
};
typedef struct Matrice Matrice;

struct Pattern {
	// récupération des matrices liées aux patterns
	Graphe* graphe;
	Matrice Mat[40];
	Matrice end_low;
	Matrice end_mid;
	Matrice end_high;
};
typedef struct Pattern Pattern;
// Fonctions
struct Graphe*
create_graph();

void free_graph(struct Graphe* graphe);

int parcours_graphe(struct Graphe* graphe, int noeud_actuel);

void create_map_txt(Pattern pat, char* file);

int random_number(int min, int max);

Matrice recup_matrice(char* file);

void normalize_line(char ligne[20]);

void matrice_vers_file(Matrice matrice, char* file);

Matrice creerMatrice(int rows, int cols);

void libererMatrice(Matrice matrice);

Matrice concatenerMatrices(Matrice M1, Matrice M2, int* pattern2big, int last_pattern, int last_pattern_cols);

void afficherMatrice(Matrice mat);

Pattern pattern_initialisation();

void free_pattern(Pattern pat);

void get_file_dimensions(const char* filename, int* rows, int* cols);

char* concatStr(char* str1, char* str2, char* buffer, int bufferSize);

#endif
