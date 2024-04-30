#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 10 //nbre de sommets du graphe
#define T 5 //nbre de patterns dans la map 
#define MAX_LENGTH 150 //taille max de la ligne d'un pattern
#define L 14 //nbre de lignes dans un pattern
#define C 20 //nbre de colonnes dans un pattern

struct Graphe {
    struct Noeuds* noeud[N];
}; 

struct Noeuds{
    int destination; 
    int poids; 
    struct Noeuds* suivant;
}; 

struct Arete{
    int source; 
    int destination; 
    int poids;
}; 

// struct _Matrice{
//     char matrice[L][C];
// };

// typedef struct _Matrice Matrice;

// struct _MapTrice{
//     char matrice[L][MAX_LENGTH];
// };

typedef struct _MapTrice MapTrice;

// Structure pour représenter une matrice
typedef struct {
    int **data;  // Tableau 2D de données
    int rows;    // Nombre de lignes
    int cols;    // Nombre de colonnes
} Matrice;

struct Graphe* create_graph();

void free_graph(struct Graphe* graphe);

//void concat_Files(char* file1, char* file2);

char* parcours_graphe(struct Graphe* graphe);

void create_map_txt(char* parcours);

int random_number(int min, int max);

Matrice recup_matrice(char* pattern);

void print_matrice(Matrice matrice);

MapTrice matrice_bloc(Matrice M1, Matrice M2); 
