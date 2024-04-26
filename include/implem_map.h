#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 10 //nbre de sommets du graphe
#define T 5 //nbre de patterns dans la map

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

struct Graphe* create_graph();

void free_graph(struct Graphe* graphe);

void concat_Files(char* file1, char* file2);

char* parcours_graphe(struct Graphe* graphe, int debut);

FILE* create_map_txt(char* parcours);
