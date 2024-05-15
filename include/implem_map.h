#ifndef IMPLEM_MAP_H
#define IMPLEM_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 10			// nbre de sommets du graphe
#define T 5				// nbre de patterns dans la map
#define MAX_LENGTH 150	// taille max de la ligne d'un pattern
#define L 14			// nbre de lignes dans un pattern
#define C 20			// nbre de colonnes dans un pattern

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
	Matrice Start;
	Matrice M1;
	Matrice M2;
	Matrice M3;
	Matrice M4;
	Matrice M5;
	Matrice M6;
	Matrice M7;
	Matrice M8;
	Matrice M9;
	Matrice M0;
	Matrice End;
};
typedef struct Pattern Pattern;
// Fonctions
struct Graphe*
create_graph();

void free_graph(struct Graphe* graphe);

char* parcours_graphe(struct Graphe* graphe, int length);

void create_map_txt(Pattern pat, int length, char* file);

int random_number(int min, int max);

Matrice recup_matrice(char* file);

void normalize_line(char ligne[20]);

void matrice_vers_file(Matrice matrice, char* file);

Matrice creerMatrice(int rows, int cols);

void libererMatrice(Matrice matrice);

Matrice concatenerMatrices(Matrice M1, Matrice M2);

void afficherMatrice(Matrice mat);

Pattern pattern_initialisation();

void free_pattern(Pattern pat);

#endif
