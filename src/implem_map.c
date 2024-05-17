#include "../include/implem_map.h"

// Fonction qui donne un nombre aléatoire entre min et max
int random_number(int min, int max) {
	srand(rand());
	printf("rand : %d \n", rand());
	return rand() % (max - min + 1) + min;
}

// Fonction pour normaliser une ligne en remplaçant '\n' par '\0
void normalize_line(char ligne[20]) {
	for (int i = 0; i < 20; i++) {
		if (ligne[i] == '\n') {
			ligne[i] = '\0';
			break;
		}
	}
}

// Fonction pour créer un graphe
struct Graphe* create_graph() {
	// les sommets sont les patterns
	// graphe orienté et pondéré de manière à créer des maps différentes
	// chaque sommet est relié à deux autres sommets

	struct Arete aretes[] = {
		{0, 4, 1}, {0, 7, 4},  // sommet 0
		{1, 2, 1},
		{1, 9, 1},	// sommet 1
		{2, 4, 2},
		{2, 0, 1},	// sommet 2
		{3, 5, 1},
		{3, 8, 2},	// sommet 3
		{4, 3, 2},
		{4, 8, 1},	// sommet 4
		{5, 6, 2},
		{5, 1, 1},	// sommet 5
		{6, 2, 3},
		{6, 9, 3},	// sommet 6
		{7, 6, 2},
		{7, 3, 1},	// sommet 7
		{8, 1, 2},
		{8, 0, 1},	// sommet 8
		{9, 7, 1},
		{9, 5, 1},	// sommet 9
	};

	int n = sizeof(aretes) / sizeof(aretes[0]);
	struct Graphe* graphe = (struct Graphe*)malloc(sizeof(struct Graphe));

	if (graphe == NULL) {
		printf("Erreur d'allocation de mémoire pour le graphe\n");
		exit(1);
	}

	for (int i = 0; i < N; i++) {
		graphe->noeud[i] = NULL;
	}

	for (int i = 0; i < n; i++) {
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

// Fonction pour libérer la mémoire allouée pour un graphe
void free_graph(struct Graphe* graphe) {
	if (graphe == NULL) {
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
void matrice_vers_file(Matrice matrice, char* file) {
	FILE* f = fopen(file, "w");

	if (f == NULL) {
		printf("Error opening file in : matrice_vers_file\n");
		exit(1);
	}

	for (int i = 0; i < matrice.rows; i++) {
		for (int j = 0; j < matrice.cols; j++) {
			fprintf(f, "%c", matrice.data[i][j]);
		}
		if (i < matrice.rows - 1) {
			fprintf(f, "\n");
		}
	}

	fclose(f);
}

// Fonction pour obtenir un parcours aléatoire dans le graphe
int parcours_graphe(struct Graphe* graphe, int noeud_actuel) {
	struct Noeuds* noeud = graphe->noeud[noeud_actuel];
	if (noeud != NULL) {
		noeud_actuel = noeud->destination;
	} else {
		printf("BUG: Noeud in parcours_graphe is null");
	}
	return noeud_actuel;
}

// Fonction pour créer une nouvelle matrice avec des dimensions spécifiées
Matrice creerMatrice(int rows, int cols) {
	Matrice mat;
	mat.rows = rows;
	mat.cols = cols;

	// Allouer de la mémoire pour le tableau 2D de données
	mat.data = (int**)malloc(rows * sizeof(int*));
	if (mat.data == NULL) {
		fprintf(stderr, "Erreur : Allocation mémoire pour les lignes de la matrice échouée\n");
		exit(EXIT_FAILURE);
	}

	// Allouer de la mémoire pour chaque ligne de la matrice
	for (int i = 0; i < rows; i++) {
		mat.data[i] = (int*)malloc(cols * sizeof(int));
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
Matrice recup_matrice(char* file) {
	FILE* f = fopen(file, "r");
	if (f == NULL) {
		printf("Error opening %s in : recup_matrice\n", file);
		exit(1);
	}
	int rows, cols;
	get_file_dimensions(file, &rows, &cols);
	Matrice matrice = creerMatrice(rows, cols);
	char line[cols + 2];  // +2 pour le caractère de fin de ligne et le caractère nul de fin de chaîne

	for (int i = 0; i < rows; i++) {
		fgets(line, sizeof(line), f);
		for (int j = 0; j < cols; j++) {
			matrice.data[i][j] = line[j];
		}
	}

	fclose(f);

	return matrice;
}

void get_file_dimensions(const char* filename, int* rows, int* cols) {
	FILE* fp;
	char line[1024];  // assume max line length is 1024
	int max_cols = 0;

	fp = fopen(filename, "r");	// open the file in read mode
	if (fp == NULL) {
		perror("Error opening file");
		*rows = 0;
		*cols = 0;
		return;
	}

	*rows = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		(*rows)++;	// increment row count
		int len = strlen(line);
		if (len > max_cols) {
			max_cols = len;	 // update max column count
		}
	}

	*cols = max_cols - 1;  // set cols to max_cols

	fclose(fp);
}

// Fonction pour concaténer deux matrices par bloc (horizontalement)
Matrice concatenerMatrices(Matrice M1, Matrice M2, int* pattern2big) {
	if (M1.rows != M2.rows) {
		fprintf(stderr, "Erreur : Les matrices doivent avoir le même nombre de lignes pour être concaténées par bloc\n");
		exit(EXIT_FAILURE);
	}

	int rows = M1.rows;
	int cols1 = M1.cols;
	int cols2 = M2.cols;
	if (cols1 + cols2 > MAX_TILES) {
		*pattern2big = 1;
		return M1;
	}
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
	libererMatrice(M1);
	return concat;
}

// Fonction pour afficher une matrice
void afficherMatrice(Matrice mat) {
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			printf("%c", mat.data[i][j]);
		}
		printf("\n");
	}
}

Pattern pattern_initialisation() {
	Pattern pat;

	pat.graphe = create_graph();

	char* f1 = "./Patterns/pattern1.txt";
	char* f2 = "./Patterns/pattern2.txt";
	char* f3 = "./Patterns/pattern3.txt";
	char* f4 = "./Patterns/pattern4.txt";
	char* f5 = "./Patterns/pattern5.txt";
	char* f6 = "./Patterns/pattern6.txt";
	char* f7 = "./Patterns/pattern7.txt";
	char* f8 = "./Patterns/pattern8.txt";
	char* f9 = "./Patterns/pattern9.txt";
	char* f0 = "./Patterns/pattern0.txt";
	char* end = "./Patterns/last_pattern.txt";
	char* start = "./Patterns/first_pattern.txt";

	// récupération des matrices liées aux patterns
	pat.Start = recup_matrice(start);
	pat.M1 = recup_matrice(f1);
	pat.M2 = recup_matrice(f2);
	pat.M3 = recup_matrice(f3);
	pat.M4 = recup_matrice(f4);
	pat.M5 = recup_matrice(f5);
	pat.M6 = recup_matrice(f6);
	pat.M7 = recup_matrice(f7);
	pat.M8 = recup_matrice(f8);
	pat.M9 = recup_matrice(f9);
	pat.M0 = recup_matrice(f0);
	pat.End = recup_matrice(end);

	return pat;
}

void free_pattern(Pattern pat) {
	libererMatrice(pat.Start);
	libererMatrice(pat.M1);
	libererMatrice(pat.M2);
	libererMatrice(pat.M3);
	libererMatrice(pat.M4);
	libererMatrice(pat.M5);
	libererMatrice(pat.M6);
	libererMatrice(pat.M7);
	libererMatrice(pat.M8);
	libererMatrice(pat.M9);
	libererMatrice(pat.M0);
	libererMatrice(pat.End);
	free_graph(pat.graphe);
}

// Fonction pour créer un fichier .txt à partir des fichiers patterns concaténés selon le parcours du graphe
void create_map_txt(Pattern pat, char* file) {
	char* start = "./Patterns/first_pattern.txt";
	Matrice M = recup_matrice(start);
	int noeud_actuel = random_number(1, N - 1);
	int pattern2big = 0;
	while (M.cols <= MAX_TILES - pat.End.cols && pattern2big == 0) {
		noeud_actuel = parcours_graphe(pat.graphe, noeud_actuel);
		switch (noeud_actuel) {
		case 0:
			M = concatenerMatrices(M, pat.M0, &pattern2big);
			break;
		case 1:
			M = concatenerMatrices(M, pat.M1, &pattern2big);
			break;
		case 2:
			M = concatenerMatrices(M, pat.M2, &pattern2big);
			break;
		case 3:
			M = concatenerMatrices(M, pat.M3, &pattern2big);
			break;
		case 4:
			M = concatenerMatrices(M, pat.M4, &pattern2big);
			break;
		case 5:
			M = concatenerMatrices(M, pat.M5, &pattern2big);
			break;
		case 6:
			M = concatenerMatrices(M, pat.M6, &pattern2big);
			break;
		case 7:
			M = concatenerMatrices(M, pat.M7, &pattern2big);
			break;
		case 8:
			M = concatenerMatrices(M, pat.M8, &pattern2big);
			break;
		case 9:
			M = concatenerMatrices(M, pat.M9, &pattern2big);
			break;
		}
	}
	M = concatenerMatrices(M, pat.End, &pattern2big);
	matrice_vers_file(M, "test.txt");
	libererMatrice(M);
}

// int main(){
//     Pattern pat = pattern_initialisation();
//     create_map_txt(pat, "test.txt");
//     return 0;
// }
