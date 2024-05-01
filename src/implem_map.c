#include "../include/implem_map.h"

// Fonction qui donne un nombre aléatoire entre min et max
int random_number(int min, int max){

    srand(time(NULL));
    return rand() % (max - min + 1) + min;
}

// Fonction pour normaliser une ligne en remplaçant '\n' par '\0
void normalize_line(char ligne[20]){

    for(int i=0;i<20;i++){
        if(ligne[i]=='\n'){
            ligne[i]='\0';
            break;
        }
    }
}

// Fonction pour créer un graphe
struct Graphe* create_graph(){
    // les sommets sont les patterns 
    // graphe orienté et pondéré de manière à créer des maps différentes
    // chaque sommet est relié à deux autres sommets

    struct Arete aretes[] = {
        {0, 4, 1}, {0, 7, 4}, //sommet 0
        {1, 2, 1}, {1, 9, 1}, //sommet 1
        {2, 4, 2}, {2, 0, 1}, //sommet 2
        {3, 5, 1}, {3, 8, 2}, //sommet 3
        {4, 3, 2}, {4, 8, 1}, //sommet 4
        {5, 6, 2}, {5, 1, 1}, //sommet 5
        {6, 2, 3}, {6, 9, 3}, //sommet 6
        {7, 6, 2}, {7, 3, 1}, //sommet 7
        {8, 1, 2}, {8, 0, 1}, //sommet 8
        {9, 7, 1}, {9, 5, 1}, //sommet 9
    };

    int n = sizeof(aretes)/sizeof(aretes[0]);
    struct Graphe* graphe = (struct Graphe*)malloc(sizeof(struct Graphe));

    if (graphe == NULL) {
        printf("Erreur d'allocation de mémoire pour le graphe\n");
        exit(1);
    }

    //graphe->noeud = malloc(N * sizeof(struct Noeuds*)); //utiliser cette ligne et ça devrait le faire d'après copilot

    for (int i = 0; i < N; i++){
        graphe->noeud[i] = NULL;
    }

    for (int i = 0; i < n-2; i++){
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
        nouveau_noeud->suivant = graphe->noeud[source]; //erreur ici : voir le commentaire en fin de code
        graphe->noeud[source] = nouveau_noeud;
    }
    
    return graphe;
}

// Fonction pour libérer la mémoire allouée pour un graphe
void free_graph(struct Graphe* graphe){

    if (graphe == NULL){
        return;
    };

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

// Fonction qui transforme une matrice en un fichier .txt
void matrice_vers_file(Matrice matrice, char* file){

    FILE* f = fopen(file, "w");

    if (f == NULL){
        printf("Error opening file matrice_vers_file\n");
        exit(1);
    }

    for (int i = 0; i < matrice.rows; i++){
        for (int j = 0; j < matrice.cols; j++){
            fprintf(f, "%c", matrice.data[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);

}

// Fonction pour obtenir un parcours aléatoire dans le graphe
char* parcours_graphe(struct Graphe* graphe) {

    int debut = random_number(1,N-1);
    // printf("Début: %d\n", debut);
    int noeud_actuel = debut;
    char* parcours = (char*)malloc(T*sizeof(char));

    if (parcours == NULL) {
        printf("Erreur d'allocation de mémoire pour le parcours\n");
        exit(1);
    }

    parcours[0] = '\0';

    char temp[10]; // ouuuuuu char temp[T*sizeof(char)]

    for (int i = 0; i < 5; i++) {
        //printf("i: %d\n", i);
        //printf("noeud_actuel: %d\n", noeud_actuel);
        struct Noeuds* noeud = graphe->noeud[noeud_actuel];
        if (noeud != NULL) {
            sprintf(parcours, "%d", noeud_actuel);
            parcours+='\0';
            // strcat(parcours, temp);
            noeud_actuel = noeud->destination;
        } else {
            break;
        }
    }

    return parcours;

}

// Fonction pour créer une nouvelle matrice avec des dimensions spécifiées
Matrice creerMatrice(int rows, int cols) {

    Matrice mat;
    mat.rows = rows;
    mat.cols = cols;

    // Allouer de la mémoire pour le tableau 2D de données
    mat.data = (int **)malloc(rows * sizeof(int *));
    if (mat.data == NULL) {
        fprintf(stderr, "Erreur : Allocation mémoire pour les lignes de la matrice échouée\n");
        exit(EXIT_FAILURE);
    }

    // Allouer de la mémoire pour chaque ligne de la matrice
    for (int i = 0; i < rows; i++) {
        mat.data[i] = (int *)malloc(cols * sizeof(int));
        if (mat.data[i] == NULL) {
            fprintf(stderr, "Erreur : Allocation mémoire pour les colonnes de la matrice échouée\n");
            for (int j = 0; j < i; j++) {
                free(mat.data[j]);
            }
            free(mat.data);
            exit(EXIT_FAILURE);
        }
    }

    return mat;
}

// Fonction pour libérer la mémoire allouée pour une matrice
void libererMatrice(Matrice mat) {

    for (int i = 0; i < mat.rows; i++) {
        free(mat.data[i]);
    }

    free(mat.data);
}

// Fonction qui transforme un pattern en matrice
Matrice recup_matrice(char* file){

    FILE* f = fopen(file, "r");
    if (f == NULL){
        printf("Error opening file recup_matrice\n");
        exit(1);
    }

    Matrice matrice = creerMatrice(L,C);
    char line[C + 2]; // +2 pour le caractère de fin de ligne et le caractère nul de fin de chaîne

    for (int i = 0; i < L; i++){
        fgets(line, sizeof(line), f);
        for (int j = 0; j < C; j++){
            matrice.data[i][j] = line[j];
        }
    }

    fclose(f);

    return matrice;
}

// Fonction pour concaténer deux matrices par bloc (horizontalement)
Matrice concatenerMatrices(Matrice M1, Matrice M2) {

    if (M1.rows != M2.rows) {
        fprintf(stderr, "Erreur : Les matrices doivent avoir le même nombre de lignes pour être concaténées par bloc\n");
        exit(EXIT_FAILURE);
    }

    int rows = M1.rows;
    int cols1 = M1.cols;
    int cols2 = M2.cols;

    // Créer une nouvelle matrice pour stocker la concaténation
    Matrice concat = creerMatrice(rows, cols1 + cols2);

    // Copier les données de M1 dans la nouvelle matrice
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols1; j++) {
            concat.data[i][j] = M1.data[i][j];
        }
    }

    // Copier les données de M2 dans la nouvelle matrice
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols2; j++) {
            concat.data[i][cols1 + j] = M2.data[i][j];
        }
    }

    return concat;
}

// Fonction pour afficher une matrice
void afficherMatrice(Matrice mat) {

    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            printf("%d ", mat.data[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour créer un fichier .txt à partir des fichiers patterns concaténés selon le parcours du graphe
void create_map_txt(char* parcours){

    char* start = "../Patterns/first_pattern.txt";
    char* f1 = "../Patterns/pattern1.txt";
    char* f2 = "../Patterns/pattern2.txt";
    char* f3 = "../Patterns/pattern3.txt";
    char* f4 = "../Patterns/pattern4.txt";
    char* f5 = "../Patterns/pattern5.txt";
    char* f6 = "../Patterns/pattern6.txt";
    char* f7 = "../Patterns/pattern7.txt";
    char* f8 = "../Patterns/pattern8.txt";
    char* f9 = "../Patterns/pattern9.txt";
    char* f0 = "../Patterns/pattern0.txt";
    char* end = "../Patterns/last_pattern.txt";
    char* f = "../test.txt";

    // récupération des matrices liées aux patterns
    Matrice Start = recup_matrice(start);
    Matrice M1 = recup_matrice(f1);
    Matrice M2 = recup_matrice(f2);
    Matrice M3 = recup_matrice(f3);
    Matrice M4 = recup_matrice(f4);
    Matrice M5 = recup_matrice(f5);
    Matrice M6 = recup_matrice(f6);
    Matrice M7 = recup_matrice(f7);
    Matrice M8 = recup_matrice(f8);
    Matrice M9 = recup_matrice(f9);
    Matrice M0 = recup_matrice(f0);
    Matrice End = recup_matrice(end);
    Matrice M = recup_matrice(f);

    M = concatenerMatrices(M, Start);

    for(int i = 0; parcours[i] != '\0'; i++){
        if (parcours[i] == '1'){
            M = concatenerMatrices(M, M1);
            printf("f1 concatené\n");
        }
        if (parcours[i] == '2'){
            M = concatenerMatrices(M, M2);
            printf("f2 concatené\n");
        }
        if (parcours[i] == '3'){
            M = concatenerMatrices(M, M3);
            printf("f3 concatené\n");
        }
        if (parcours[i] == '4'){
            M = concatenerMatrices(M, M4);
            printf("f4 concatené\n");
        }
        if (parcours[i] == '5'){
            M = concatenerMatrices(M, M5);
            printf("f5 concatené\n");
        }
        if (parcours[i] == '6'){
            M = concatenerMatrices(M, M6);
            printf("f6 concatené\n");
        }
        if (parcours[i] == '7'){
            M = concatenerMatrices(M, M7);
            printf("f7 concatené\n");
        }
        if (parcours[i] == '8'){
            M = concatenerMatrices(M, M8);
            printf("f8 concatené\n");
        }
        if (parcours[i] == '9'){
            M = concatenerMatrices(M, M9);
            printf("f9 concatené\n");
        }
        if (parcours[i] == '0'){
            M = concatenerMatrices(M, M0);
            printf("f10 concatené\n");
        }
    }

    //afficherMatrice(M);

    M = concatenerMatrices(M, End);

    matrice_vers_file(M, f);

    libererMatrice(Start);
    libererMatrice(M1);
    libererMatrice(M2);
    libererMatrice(M3);
    libererMatrice(M4);
    libererMatrice(M5);
    libererMatrice(M6);
    libererMatrice(M7);
    libererMatrice(M8);
    libererMatrice(M9);
    libererMatrice(M0);
    libererMatrice(End);
    libererMatrice(M);
}

// int main(){

//     struct Graphe* graphe = create_graph();
//     char* parcours = parcours_graphe(graphe);
//     printf("%s\n", parcours);
//     create_map_txt(parcours);
//     free(graphe);
//     free(parcours); 

//     return 0;
// }


///////////////////////////
// Error:

// ==72036==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x00010546feb0 at pc 0x000100358b38 bp 0x00016faba7d0 sp 0x00016faba7c8
// READ of size 8 at 0x00010546feb0 thread T0
//     #0 0x100358b34 in create_graph implem_map.c:65
//     #1 0x1003522e4 in generated_pattern procedural_generation.c:8
//     #2 0x10034d07c in move_camera main.c:727
//     #3 0x10034a210 in draw main.c:689
//     #4 0x1003483c4 in main main.c:267
//     #5 0x192f920dc  (<unknown module>)

// 0x00010546feb0 is located 0 bytes after 80-byte region [0x00010546fe60,0x00010546feb0)
// allocated by thread T0 here:
//     #0 0x100f0b124 in wrap_malloc+0x94 (libclang_rt.asan_osx_dynamic.dylib:arm64e+0x53124)
//     #1 0x1003582b0 in create_graph implem_map.c:41
//     #2 0x1003522e4 in generated_pattern procedural_generation.c:8
//     #3 0x10034d07c in move_camera main.c:727
//     #4 0x10034a210 in draw main.c:689
//     #5 0x1003483c4 in main main.c:267
//     #6 0x192f920dc  (<unknown module>)

// SUMMARY: AddressSanitizer: heap-buffer-overflow implem_map.c:65 in create_graph
// Shadow bytes around the buggy address:
//   0x00010546fc00: fd fd fa fa fa fa 00 00 00 00 00 00 00 00 00 00
//   0x00010546fc80: fa fa fa fa 00 00 00 00 00 00 00 00 00 00 fa fa
//   0x00010546fd00: fa fa 00 00 00 00 00 00 00 00 00 fa fa fa fa fa
//   0x00010546fd80: 00 00 00 00 00 00 00 00 00 fa fa fa fa fa fd fd
//   0x00010546fe00: fd fd fd fd fd fd fd fd fa fa fa fa 00 00 00 00
// =>0x00010546fe80: 00 00 00 00 00 00[fa]fa fa fa fa fa fa fa fa fa
//   0x00010546ff00: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
//   0x00010546ff80: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
//   0x000105470000: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
//   0x000105470080: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
//   0x000105470100: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
// Shadow byte legend (one shadow byte represents 8 application bytes):
//   Addressable:           00
//   Partially addressable: 01 02 03 04 05 06 07 
//   Heap left redzone:       fa
//   Freed heap region:       fd
//   Stack left redzone:      f1
//   Stack mid redzone:       f2
//   Stack right redzone:     f3
//   Stack after return:      f5
//   Stack use after scope:   f8
//   Global redzone:          f9
//   Global init order:       f6
//   Poisoned by user:        f7
//   Container overflow:      fc
//   Array cookie:            ac
//   Intra object redzone:    bb
//   ASan internal:           fe
//   Left alloca redzone:     ca
//   Right alloca redzone:    cb
// ==72036==ABORTING
