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
    // chaque sommet est relié à deux autres sommets

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
    // libère la mémoire allouée pour le graphe

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
//     // celle-ci fonctionne pas : renvoie rien au final
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

// void concat_Files(char* file1, char* file2){
//     // concaténation, ligne par ligne, de deux fichiers dans le premier 
//     // celle-ci fonctionne presque
//
//     FILE* f1 = fopen(file1, "a");
//     FILE* f2 = fopen(file2, "r");
//
//     if (f1 == NULL || f2 == NULL){
//         printf("Error opening file\n");
//         exit(1);
//     }
//
//     char ligne2[21];
//
//     while(fgets(ligne2,20,f2) != NULL){
//         //normalize_line(ligne2);
//         fprintf(f1,"%s",ligne2);
//     }
//  
//
//     fclose(f1);
//     fclose(f2);
// }

// void concat_Files(char* file1, char* file2){
//     // concaténation, ligne par ligne, de deux fichiers dans le premier
//
//     // on ouvre les fichiers
//     FILE* f1 = fopen(file1, "a");
//     FILE* f2 = fopen(file2, "r");
//
//     // on crée des variables pour chaque ligne
//     char ligne1_finale[150];
//     char ligne2_finale[150];
//     char ligne3_finale[150];
//     char ligne4_finale[150];
//     char ligne5_finale[150];
//     char ligne6_finale[150];
//     char ligne7_finale[150];
//     char ligne8_finale[150];
//     char ligne9_finale[150];
//     char ligne10_finale[150];
//     char ligne11_finale[150];
//     char ligne12_finale[150];
//     char ligne13_finale[150];
//     char ligne14_finale[150];
//
//     char ligne1_file1[150];
//     char ligne2_file1[150];
//     char ligne3_file1[150];
//     char ligne4_file1[150];
//     char ligne5_file1[150];
//     char ligne6_file1[150];
//     char ligne7_file1[150];
//     char ligne8_file1[150];
//     char ligne9_file1[150];
//     char ligne10_file1[150];
//     char ligne11_file1[150];
//     char ligne12_file1[150];
//     char ligne13_file1[150];
//     char ligne14_file1[150];
//
//     char ligne1_file2[21];
//     char ligne2_file2[21];
//     char ligne3_file2[21];
//     char ligne4_file2[21];
//     char ligne5_file2[21];
//     char ligne6_file2[21];
//     char ligne7_file2[21];
//     char ligne8_file2[21];
//     char ligne9_file2[21];
//     char ligne10_file2[21];
//     char ligne11_file2[21];
//     char ligne12_file2[21];
//     char ligne13_file2[21];
//     char ligne14_file2[21];
//
//     // on récupère chaque ligne 
//     fgets(ligne1_file1, 150, f1);
//     fgets(ligne2_file1, 150, f1);
//     fgets(ligne3_file1, 150, f1);
//     fgets(ligne4_file1, 150, f1);
//     fgets(ligne5_file1, 150, f1);
//     fgets(ligne6_file1, 150, f1);
//     fgets(ligne7_file1, 150, f1);
//     fgets(ligne8_file1, 150, f1);
//     fgets(ligne9_file1, 150, f1);
//     fgets(ligne10_file1, 150, f1);
//     fgets(ligne11_file1, 150, f1);
//     fgets(ligne12_file1, 150, f1);
//     fgets(ligne13_file1, 150, f1);
//     fgets(ligne14_file1, 150, f1);
//     printf("%s\n", ligne14_file1);
//
//     fgets(ligne1_file2, 21, f2);
//     fgets(ligne2_file2, 21, f2);
//     fgets(ligne3_file2, 21, f2);
//     fgets(ligne4_file2, 21, f2);
//     fgets(ligne5_file2, 21, f2);
//     fgets(ligne6_file2, 21, f2);
//     fgets(ligne7_file2, 21, f2);
//     fgets(ligne8_file2, 21, f2);
//     fgets(ligne9_file2, 21, f2);
//     fgets(ligne10_file2, 21, f2);
//     fgets(ligne11_file2, 21, f2);
//     fgets(ligne12_file2, 21, f2);
//     fgets(ligne13_file2, 21, f2);
//     fgets(ligne14_file2, 21, f2);
//     printf("%s\n", ligne14_file2);
//
//     //on normalise les lignes "réceptrices"
//     normalize_line(ligne1_file1);
//     normalize_line(ligne2_file1);
//     normalize_line(ligne3_file1);
//     normalize_line(ligne4_file1);
//     normalize_line(ligne5_file1);
//     normalize_line(ligne6_file1);
//     normalize_line(ligne7_file1);
//     normalize_line(ligne8_file1);
//     normalize_line(ligne9_file1);
//     normalize_line(ligne10_file1);
//     normalize_line(ligne11_file1);
//     normalize_line(ligne12_file1);
//     normalize_line(ligne13_file1);
//     normalize_line(ligne14_file1);
//
//     // on les concatène 
//     strcpy(ligne1_finale, ligne1_file1);
//     strcat(ligne1_finale, ligne1_file2);
//
//     strcpy(ligne2_finale, ligne2_file1);
//     strcat(ligne2_finale, ligne2_file2);
//
//     strcpy(ligne3_finale, ligne3_file1);
//     strcat(ligne3_finale, ligne3_file2);
//
//     strcpy(ligne4_finale, ligne4_file1);
//     strcat(ligne4_finale, ligne4_file2);
//
//     strcpy(ligne5_finale, ligne5_file1);
//     strcat(ligne5_finale, ligne5_file2);
//
//     strcpy(ligne6_finale, ligne6_file1);
//     strcat(ligne6_finale, ligne6_file2);
//
//     strcpy(ligne7_finale, ligne7_file1);
//     strcat(ligne7_finale, ligne7_file2);
//
//     strcpy(ligne8_finale, ligne8_file1);
//     strcat(ligne8_finale, ligne8_file2);
//
//     strcpy(ligne9_finale, ligne9_file1);
//     strcat(ligne9_finale, ligne9_file2);
//
//     strcpy(ligne10_finale, ligne10_file1);
//     strcat(ligne10_finale, ligne10_file2);
//
//     strcpy(ligne11_finale, ligne11_file1);
//     strcat(ligne11_finale, ligne11_file2);
//
//     strcpy(ligne12_finale, ligne12_file1);
//     strcat(ligne12_finale, ligne12_file2);
//
//     strcpy(ligne13_finale, ligne13_file1);
//     strcat(ligne13_finale, ligne13_file2);
//
//     strcpy(ligne14_finale, ligne14_file1);
//     strcat(ligne14_finale, ligne14_file2);
//     printf("%s\n", ligne14_finale);
//
//     // on les écrit dans le fichier file1
//     fprintf(f1, "%s", ligne1_finale);
//     fprintf(f1, "%s", ligne2_finale);
//     fprintf(f1, "%s", ligne3_finale);
//     fprintf(f1, "%s", ligne4_finale);
//     fprintf(f1, "%s", ligne5_finale);
//     fprintf(f1, "%s", ligne6_finale);
//     fprintf(f1, "%s", ligne7_finale);
//     fprintf(f1, "%s", ligne8_finale);
//     fprintf(f1, "%s", ligne9_finale);
//     fprintf(f1, "%s", ligne10_finale);
//     fprintf(f1, "%s", ligne11_finale);
//     fprintf(f1, "%s", ligne12_finale);
//     fprintf(f1, "%s", ligne13_finale);
//     fprintf(f1, "%s", ligne14_finale);
//
//     // on ferme les fichiers
//     fclose(f1);
//     fclose(f2);
//
// }

// void concat_Files(char* file1, char* file2){
//     // on concatène deux fichiers en un seul, ligne par ligne 
//
//     // on ouvre les fichiers
//     FILE* f1 = fopen(file1, "r+");
//     FILE* f2 = fopen(file2, "r");
//
//     // gestion de l'erreur d'ouverture des fichiers
//     if (f1 == NULL || f2 == NULL){
//         printf("Error opening file\n");
//         exit(1);
//     }
//
//     char ligne1[150];
//     char ligne2[21];
//     while(fgets(ligne1, 150, f1) != NULL && fgets(ligne2, 21, f2) != NULL){
//         printf("%s\n", ligne1);
//         printf("%s\n", ligne2);
//         normalize_line(ligne1);
//         strcat(ligne1, ligne2);
//         printf("%s\n", ligne1);
//         fprintf(f1, "%s", ligne1);
//     }
//
//     fclose(f1);
//     fclose(f2);
//    
// }

// void concat_Files(char *fichier1, char *fichier2) {
//     FILE *f1, *f2, *output;
//     char ligneF1[MAX_LENGTH];
//     char ligneF2[MAX_LENGTH];
// //
//     // Ouvrir les fichiers en mode lecture
//     f1 = fopen(fichier1, "r");
//     if (f1 == NULL) {
//         fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", fichier1);
//         return;
//     }
// //
//     f2 = fopen(fichier2, "r");
//     if (f2 == NULL) {
//         fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", fichier2);
//         fclose(f1);
//         return;
//     }
// //
//     // Ouvrir le fichier de sortie en mode écriture
//     output = fopen(fichier1, "w");
//     if (output == NULL) {
//         fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s en écriture\n", fichier1);
//         fclose(f1);
//         fclose(f2);
//         return;
//     }
// //
//     // Lire et concaténer les lignes jusqu'à ce qu'il n'y ait plus de lignes à lire dans l'un des fichiers
//     while (fgets(ligneF1, MAX_LENGTH, f1) != NULL && fgets(ligneF2, MAX_LENGTH, f2) != NULL) {
//         // Supprimer le caractère de saut de ligne à la fin de chaque ligne
//         //ligneF1[strcspn(ligneF1, "\n")] = '\0';
//         normalize_line(ligneF1);
//         //ligneF2[strcspn(ligneF2, "\n")] = '\0';
// //
//         // Concaténer les lignes lues
//         strcat(ligneF1, ligneF2);
// //
//         // Écrire la ligne concaténée dans le premier fichier
//         fprintf(output, "%s", ligneF1);
//     }
// //
//     // Fermer les fichiers
//     fclose(f1);
//     fclose(f2);
//     fclose(output);
// //
//     printf("Opération de concaténation terminée avec succès.\n");
// }

// void concat_Files(char* file1, char* file2){
//     // on concatène deux fichiers en un seul, ligne par ligne 
//
//     // on ouvre les fichiers
//     FILE* f1 = fopen(file1, "r+");
//     FILE* f2 = fopen(file2, "r");
//
//     // gestion de l'erreur d'ouverture des fichiers
//     if (f1 == NULL || f2 == NULL){
//         printf("Error opening file\n");
//         exit(1);
//     }
//
//     //
//     char ligne1[151];
//     char ligne2[21];
//     while(fgets(ligne1, 150, f1) != NULL && fgets(ligne2, 20, f2) != NULL){
//         // on concatène les lignes
//         strcat(ligne1, ligne2);
//         // on écrit la ligne concaténée dans le premier fichier
//         fprintf(f1, "%s", ligne1);
//     }
//
//     fclose(f1);
//     fclose(f2);
// }

// void concat_Files(char* file1, char* file2){
//     // concaténation, ligne par ligne, de deux fichiers dans le premier 
//     // celle-ci fonctionne presque 
//     // le soucis c'est qu'elle concatène verticalement et non horizontalement
//
//     FILE* f1 = fopen(file1, "r+");
//     FILE* f2 = fopen(file2, "r");
//
//     if (f1 == NULL || f2 == NULL){
//         printf("Error opening file\n");
//         exit(1);
//     }
//
//     char ligne1[MAX_LENGTH];
//     char ligne2[21];
//
//     while(fgets(ligne2,20,f2) != NULL && fgets(ligne1,MAX_LENGTH,f1) != NULL){
//         normalize_line(ligne1);
//         strcat(ligne1,ligne2);
//
//         //normalize_line(ligne2);
//         fprintf(f1,"%s",ligne1);
//     }
// 
//
//     fclose(f1);
//     fclose(f2);
// }

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

// void create_map_txt(char* parcours){
//     // création du fichier map.txt à partir des fichiers patterns
//
//     char* f1 = "../Patterns/pattern1.txt";
//     char* f2 = "../Patterns/pattern2.txt";
//     char* f3 = "../Patterns/pattern3.txt";
//     char* f4 = "../Patterns/pattern4.txt";
//     char* f5 = "../Patterns/pattern5.txt";
//     char* f6 = "../Patterns/pattern6.txt";
//     char* f7 = "../Patterns/pattern7.txt";
//     char* f8 = "../Patterns/pattern8.txt";
//     char* f9 = "../Patterns/pattern9.txt";
//     char* f10 = "../Patterns/pattern10.txt";
//     char* f = "../test.txt";
//
//     // gestion de l'erreur d'ouverture des fichiers
//     // if (f1 == NULL || f2 == NULL || f3 == NULL || f4 == NULL || f5 == NULL || f6 == NULL || f7 == NULL || f8 == NULL || f9 == NULL || f10 == NULL || f == NULL){
//     //     printf("Error opening file\n");
//     //     exit(1);
//     // }
//
//     //concaténation des fichiers patterns dans le fichier map.txt
//     for (int i = 0; parcours[i] != '\0'; i++){
//         if (parcours[i] == '1' && (parcours[i+1] != '0' || parcours[i+1] == '\0')){
//             concat_Files(f, f1);
//             printf("f1 concatené\n");
//         }
//         if (parcours[i] == '2'){
//             concat_Files(f, f2);
//             printf("f2 concatené\n");
//         }
//         if (parcours[i] == '3'){
//             concat_Files(f, f3);
//             printf("f3 concatené\n");
//         }
//         if (parcours[i] == '4'){
//             concat_Files(f, f4);
//             printf("f4 concatené\n");
//         }
//         if (parcours[i] == '5'){
//             concat_Files(f, f5);
//             printf("f5 concatené\n");
//         }
//         if (parcours[i] == '6'){
//             concat_Files(f, f6);
//             printf("f6 concatené\n");
//         }
//         if (parcours[i] == '7'){
//             concat_Files(f, f7);
//             printf("f7 concatené\n");
//         }
//         if (parcours[i] == '8'){
//             concat_Files(f, f8);
//             printf("f8 concatené\n");
//         }
//         if (parcours[i] == '9'){
//             concat_Files(f, f9);
//             printf("f9 concatené\n");
//         }
//         if (parcours[i] == '1' && parcours[i+1] == '0'){
//             concat_Files(f, f10);
//             printf("f10 concatené\n");
//         }
//     }
// }

int random_number(int min, int max){
    // génère un nombre aléatoire entre min et max
    srand(time(NULL));
    return rand() % (max - min + 1) + min;
}

// void print_matrice(Matrice matrice){
//     // affiche la matrice
//     for (int i = 0; i < L; i++){
//         for (int j = 0; j < C; j++){
//             printf("%d ", matrice.matrice[i][j]);
//         }
//         printf("\n");
//     }
// }

void matrice_vers_file(Matrice matrice, char* file){
    // écrit la matrice dans un fichier
    FILE* f = fopen(file, "w");
    if (f == NULL){
        printf("Error opening file\n");
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

// MapTrice matrice_bloc(Matrice M1, Matrice M2){
//     // concatène deux matrices en une seule la matrice créée est une matrice par bloc 
//     // à savoir M = (M1|M2)
//     int column = C + sizeof(M1.matrice[0])/sizeof(M1.matrice[0][0]);
//     MapTrice M;
//     for (int i = 0; i < L; i++){
//         for (int j = 0; j < C; j++){
//             M.matrice[i][j] = M1.matrice[i][j];
//             M.matrice[i][j+C] = M2.matrice[i][j];
//         }
//         for (int j = C; j < column; j++){
//             M.matrice[i][j] = 0; // Remplir les cases vides avec 0
//         }
//     }
//     return M;
// }



// Fonction pour allouer une nouvelle matrice avec des dimensions spécifiées
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
            // Libérer la mémoire allouée précédemment
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

Matrice recup_matrice(char* file){
    // récupère la matrice d'un pattern
    FILE* f = fopen(file, "r");
    if (f == NULL){
        printf("Error opening file\n");
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

int main(){
    Matrice M1 = recup_matrice("../Patterns/pattern1.txt");
    Matrice M2 = recup_matrice("../Patterns/pattern2.txt");
    //printf("Matrices récupérées\n");
    //afficherMatrice(M1);
    //print_matrice(M1);
    //printf("\n");
    //print_matrice(M2);
    //printf("\n");
    //Matrice M = matrice_bloc(M1, M2);
    Matrice M = concatenerMatrices(M1, M2);
    //afficherMatrice(M);
    matrice_vers_file(M, "../test.txt");
    //print_matrice(M);
    //struct Graphe* graphe = create_graph();
    //printf("Graphe créé\n");
    //char* parcours = parcours_graphe(graphe);
    //free_graph(graphe);
    //printf("%s\n", parcours);
    //create_map_txt(parcours);
    //free(parcours);
    //fclose(f);
    return 0;
}