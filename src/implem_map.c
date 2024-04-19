#include <stdio.h>
#include <stdlib.h>
#include "../include/implem_map.h"



struct Graphe* create_graph(){
    // construit un graphe dont les sommets sont les patterns 
    // ce graphe est orienté et pondéré de manière à créer des maps différentes
    struct Arete aretes[] = {
        // à modifier suivant les patterns qu'on aura créé
        {0, 1, 6}, {0, 2, 3}, {1, 2, 2}, {1, 3, 5}, {2, 3, 3}
    };
    int n = sizeof(aretes)/sizeof(aretes[0]);
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

void free_graph(struct Graphe* graphe){
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

FILE* concat_Files(char* file1, char* file2){
    // concaténation de deux fichiers dans un troisème 
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");
    FILE* f3 = fopen("map.txt", "w");
    char c;
    if (f1 == NULL || f2 == NULL || f3 == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    while ((c = fgetc(f1)) != EOF){
        fputc(c, f3);
    }
    while ((c = fgetc(f2)) != EOF){
        fputc(c, f3);
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
    return f3;
}

char* parcours_graphe(struct Graphe* graphe){
    // parcours du graphe pour obtenir les patterns dans l'ordre
    char* parcours = (char*)malloc(T*sizeof(char));
    for (int i = 0; i < T; i++){
        struct Noeuds* noeud = graphe->noeud[i];
        while (noeud != NULL){
            sprintf(parcours, "%d %d %d\n", i, noeud->destination, noeud->poids);
            noeud = noeud->suivant;
        }
    }
    return parcours;

}

FILE* create_map_txt(char* parcours){
    // création du fichier map.txt qui contient les patterns dans l'ordre
    FILE* f1 = fopen("pattern1.txt","r");
    FILE* f2 = fopen("pattern2.txt","r");
    FILE* f3 = fopen("pattern3.txt","r");
    FILE* f4 = fopen("pattern4.txt","r");
    FILE* f5 = fopen("pattern5.txt","r");
    FILE* f6 = fopen("pattern6.txt","r");
    FILE* f7 = fopen("pattern7.txt","r");
    FILE* f8 = fopen("pattern8.txt","r");
    FILE* f9 = fopen("pattern9.txt","r");
    FILE* f10 = fopen("pattern10.txt","r");
    FILE* f = fopen("map.txt", "w");

    if (f1 == NULL || f2 == NULL || f3 == NULL || f4 == NULL || f5 == NULL || f6 == NULL || f7 == NULL || f8 == NULL || f9 == NULL || f10 == NULL || f == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    for (int i = 0; parcours[i] != '\0'; i++){
        if (parcours[i] == '1' && parcours[i+1] != '0'){
            FILE* f = concat_Files("map.txt", "pattern1.txt");
        }
        if (parcours[i] == '2'){
            FILE* f = concat_Files("map.txt", "pattern2.txt");
        }
        if (parcours[i] == '3'){
            FILE* f = concat_Files("map.txt", "pattern3.txt");
        }
        if (parcours[i] == '4'){
            FILE* f = concat_Files("map.txt", "pattern4.txt");
        }
        if (parcours[i] == '5'){
            FILE* f = concat_Files("map.txt", "pattern5.txt");
        }
        if (parcours[i] == '6'){
            FILE* f = concat_Files("map.txt", "pattern6.txt");
        }
        if (parcours[i] == '7'){
            FILE* f = concat_Files("map.txt", "pattern7.txt");
        }
        if (parcours[i] == '8'){
            FILE* f = concat_Files("map.txt", "pattern8.txt");
        }
        if (parcours[i] == '9'){
            FILE* f = concat_Files("map.txt", "pattern9.txt");
        }
        if (parcours[i] == '1' && parcours[i+1] == '0'){
            FILE* f = concat_Files("map.txt", "pattern10.txt");
        }
    }
    return f;
}