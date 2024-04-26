#include "../include/implem_map.h"

void normalize_line(char ligne[20]){
    // normalise une ligne en remplaçant le '\n' par '\0'
    for(int i=0;i<20;i++){
        if(ligne[i]=='\n'){
            ligne[i]='\0';
            break;
        }
    }
}

struct Graphe* create_graph(){
    // construit un graphe dont les sommets sont les patterns 
    // ce graphe est orienté et pondéré de manière à créer des maps différentes
    struct Arete aretes[] = {
        {1, 2, 1}, {1, 9, 1}, //sommet 1
        {2, 4, 2}, {2, 10, 1}, //sommet 2
        {3, 5, 1}, {3, 8, 2}, //sommet 3 
        {4, 3, 2}, {4, 8, 1}, //sommet 4
        {5, 6, 2}, {5, 1, 1}, //sommet 5
        {6, 2, 3}, {6, 9, 3}, //sommet 6
        {7, 6, 2}, {7, 3, 1}, //sommet 7 
        {8, 1, 2}, {8, 10, 1}, //sommet 8
        {9, 7, 1}, {9, 5, 1}, //sommet 9 
        {10, 4, 1}, {10, 7, 4} //sommet 10
    };
    int n = sizeof(aretes)/sizeof(aretes[0]);
    struct Graphe* graphe = (struct Graphe*)malloc(sizeof(struct Graphe));
    if (graphe == NULL) {
        printf("Erreur d'allocation de mémoire pour le graphe\n");
        exit(1);
    }
    for (int i = 0; i < N; i++){
        graphe->noeud[i] = NULL;
    }
    for (int i = 0; i < n; i++){
        int source = aretes[i].source; 
        int destination = aretes[i].destination;
        int poids = aretes[i].poids;
        struct Noeuds* nouveau_noeud = (struct Noeuds*)malloc(sizeof(struct Noeuds));
        if (nouveau_noeud == NULL) {
        printf("Erreur d'allocation de mémoire pour le nouveau noeud\n");
        exit(1);
    }
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

// void concat_Files(char* file1, char* file2){
//     // concaténation, ligne par ligne, de deux fichiers dans le premier 
//     FILE* f1 = fopen(file1, "a");
//     FILE* f2 = fopen(file2, "r");
//
//     if (f1 == NULL || f2 == NULL){
//         printf("Error opening file\n");
//         exit(1);
//     }
//     char ligne1[21];
//     char ligne2[21];
//     while(fgets(ligne1,20,f1) != NULL){
//         fgets(ligne2,20,f2);
//         normalize_line(ligne1);
//         fprintf(f1,"%s",ligne2);
//     }
//     fclose(f1);
//     fclose(f2);
// }

void concat_Files(char* file1, char* file2){
    // concaténation, ligne par ligne, de deux fichiers dans le premier 
    FILE* f1 = fopen(file1, "a");
    FILE* f2 = fopen(file2, "r");

    if (f1 == NULL || f2 == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    char ligne2[21];
    while(fgets(ligne2,20,f2) != NULL){
        //normalize_line(ligne2);
        fprintf(f1,"%s",ligne2);
    }
    fclose(f1);
    fclose(f2);
}

char* parcours_graphe(struct Graphe* graphe) {
    int debut = random_number(1,N);
    printf("Début: %d\n", debut);
    int noeud_actuel = debut;
    char* parcours = (char*)malloc(T*sizeof(char));
    if (parcours == NULL) {
        printf("Erreur d'allocation de mémoire pour le parcours\n");
        exit(1);
    }
    char temp[10];

    for (int i = 0; i < 5; i++) {
        struct Noeuds* noeud = graphe->noeud[noeud_actuel];
        if (noeud != NULL) {
            sprintf(temp, "%d", noeud_actuel);
            strcat(parcours, temp);
            noeud_actuel = noeud->destination;
        } else {
            break;
        }
    }
    return parcours;
}

void create_map_txt(char* parcours){
    // création du fichier map.txt qui contient les patterns dans l'ordre
    //
    //ouverture des fichiers en mode lecture ou écriture
    // FILE* f1 = fopen("../Patterns/pattern1.txt","r");
    // FILE* f2 = fopen("../Patterns/pattern2.txt","r");
    // FILE* f3 = fopen("../Patterns/pattern3.txt","r");
    // FILE* f4 = fopen("../Patterns/pattern4.txt","r");
    // FILE* f5 = fopen("../Patterns/pattern5.txt","r");
    // FILE* f6 = fopen("../Patterns/pattern6.txt","r");
    // FILE* f7 = fopen("../Patterns/pattern7.txt","r");
    // FILE* f8 = fopen("../Patterns/pattern8.txt","r");
    // FILE* f9 = fopen("../Patterns/pattern9.txt","r");
    // FILE* f10 = fopen("../Patterns/pattern10.txt","r");
    // FILE* f = fopen("../test.txt", "w");
    char* f1 = "../Patterns/pattern1.txt";
    char* f2 = "../Patterns/pattern2.txt";
    char* f3 = "../Patterns/pattern3.txt";
    char* f4 = "../Patterns/pattern4.txt";
    char* f5 = "../Patterns/pattern5.txt";
    char* f6 = "../Patterns/pattern6.txt";
    char* f7 = "../Patterns/pattern7.txt";
    char* f8 = "../Patterns/pattern8.txt";
    char* f9 = "../Patterns/pattern9.txt";
    char* f10 = "../Patterns/pattern10.txt";
    char* f = "../test.txt";

    // gestion de l'erreur d'ouverture des fichiers
    // if (f1 == NULL || f2 == NULL || f3 == NULL || f4 == NULL || f5 == NULL || f6 == NULL || f7 == NULL || f8 == NULL || f9 == NULL || f10 == NULL || f == NULL){
    //     printf("Error opening file\n");
    //     exit(1);
    // }

    //concaténation des fichiers patterns dans le fichier map.txt
    for (int i = 0; parcours[i] != '\0'; i++){
        //printf("%c\n", parcours[i]);
        if (parcours[i] == '1' && (parcours[i+1] != '0' || parcours[i+1] == '\0')){
            concat_Files(f, f1);
            printf("f1 concatené\n");
        }
        if (parcours[i] == '2'){
            concat_Files(f, f2);
            printf("f2 concatené\n");
        }
        if (parcours[i] == '3'){
            concat_Files(f, f3);
            printf("f3 concatené\n");
        }
        if (parcours[i] == '4'){
            concat_Files(f, f4);
            printf("f4 concatené\n");
        }
        if (parcours[i] == '5'){
            concat_Files(f, f5);
            printf("f5 concatené\n");
        }
        if (parcours[i] == '6'){
            concat_Files(f, f6);
            printf("f6 concatené\n");
        }
        if (parcours[i] == '7'){
            concat_Files(f, f7);
            printf("f7 concatené\n");
        }
        if (parcours[i] == '8'){
            concat_Files(f, f8);
            printf("f8 concatené\n");
        }
        if (parcours[i] == '9'){
            concat_Files(f, f9);
            printf("f9 concatené\n");
        }
        if (parcours[i] == '1' && parcours[i+1] == '0'){
            concat_Files(f, f10);
            printf("f10 concatené\n");
        }
    }
    //return f;
}

int random_number(int min, int max){
    // génère un nombre aléatoire entre min et max
    srand(time(NULL));
    return rand() % (max - min + 1) + min;
}

int main(){
    struct Graphe* graphe = create_graph();
    printf("Graphe créé\n");
    char* parcours = parcours_graphe(graphe);
    free_graph(graphe);
    printf("%s\n", parcours);
    create_map_txt(parcours);
    free(parcours);
    //fclose(f);
    return 0;
}