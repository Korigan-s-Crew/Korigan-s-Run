#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 10 //nbre de sommets du graphe
#define T 5 //nbre de patterns dans la map 
#define MAX_LENGTH 150 //taille max de la ligne d'un pattern
#define L 14 //nbre de lignes dans un pattern
#define C 20 //nbre de colonnes dans un pattern

// Structures
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

typedef struct {
    int **data;  // Tableau 2D de données
    int rows;    // Nombre de lignes
    int cols;    // Nombre de colonnes
} Matrice;

// Fonctions
struct Graphe* create_graph();

void free_graph(struct Graphe* graphe);

char* parcours_graphe(struct Graphe* graphe);

void create_map_txt(char* parcours);

int random_number(int min, int max);

Matrice recup_matrice(char* file);

void normalize_line(char* line[20]);

void matrice_vers_file(Matrice matrice, char* file);

Matrice creerMatrice(int rows, int cols);

void libererMatrice(Matrice matrice);

Matrice concatenerMatrice(Matrice M1, Matrice M2);

void afficherMatrice(Matrice mat);
