#include <stdio.h>
#include <stdlib.h>

#define N 10 //nbre de sommets du graphe

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

struct Graphe* create_graph(struct Arete aretes[], int n){
    struct Graphe* graphe = (struct Graphe*)malloc(sizeof(struct Graphe));
    for (int i = 0; i < N; i++){
        graphe->noeud[i] = NULL;
    }
    for (int i = 0; i < n; i++){
        int source = aretes[i].source; 
        int destination = aretes[i].destination;
        int poids = aretes[i].poids;
        struct Noeuds* nouveau_noeud = (struct Noeuds*)malloc(sizeof(struct Noeuds));
        nouveau_noeud->destination = destination;
        nouveau_noeud->poids = poids;
        nouveau_noeud->suivant = graphe->noeud[source];
        graphe->noeud[source] = nouveau_noeud;
    }
    return graphe;
}

void free_graph(struct Graphe* graphe) {
    if (graphe == NULL) return;
    for (int i = 0; i < N; i++) {
        struct Noeuds* noeud = graphe->noeud[i];
        while (noeud != NULL) {
            struct Noeuds* temp = noeud;
            noeud = noeud->suivant;
            free(temp);
        }
    }
    free(graphe);
}

int main(void){
    struct Arete aretes[] = {
        {0, 1, 6}, {0, 2, 3}, {1, 2, 2}, {1, 3, 5}, {2, 3, 3}
    }; 
    int n = sizeof(aretes)/sizeof(aretes[0]);
    struct Graphe* graphe = create_graph(aretes, n);
    free_graph(graphe);
    return 0;
}
