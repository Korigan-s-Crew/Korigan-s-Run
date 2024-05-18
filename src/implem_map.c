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

	struct Arete aretes[100] = {
		{10, 14, 1},
		{10, 17, 1},  // sommet 0
		{11, 12, 1},
		{11, 19, 1},  // sommet 1
		{12, 14, 1},
		{12, 10, 1},  // sommet 2
		{13, 15, 1},
		{13, 18, 1},  // sommet 3
		{14, 13, 1},
		{14, 18, 1},  // sommet 4
		{15, 16, 1},
		{15, 11, 1},  // sommet 5
		{16, 12, 1},
		{16, 19, 1},  // sommet 6
		{17, 16, 1},
		{17, 13, 1},  // sommet 7
		{18, 11, 1},
		{18, 10, 1},  // sommet 8
		{19, 17, 1},
		{19, 15, 1},  // sommet 9
		{20, 24, 1},
		{20, 27, 1},  // sommet 0
		{21, 22, 1},
		{21, 29, 1},  // sommet 1
		{22, 24, 1},
		{22, 20, 1},  // sommet 2
		{23, 25, 1},
		{23, 28, 1},  // sommet 3
		{24, 23, 1},
		{24, 28, 1},  // sommet 4
		{25, 26, 1},
		{25, 21, 1},  // sommet 5
		{26, 22, 1},
		{26, 29, 1},  // sommet 6
		{27, 37, 1},
		{27, 37, 1},  // sommet 7
		{28, 21, 1},
		{28, 20, 1},  // sommet 8
		{29, 27, 1},
		{29, 25, 1},  // sommet 9
		{30, 34, 1},
		{30, 37, 1},  // sommet 0
		{31, 32, 1},
		{31, 39, 1},  // sommet 1
		{32, 34, 1},
		{32, 30, 1},  // sommet 2
		{33, 35, 1},
		{33, 38, 1},  // sommet 3
		{34, 33, 1},
		{34, 38, 1},  // sommet 4
		{35, 36, 1},
		{35, 31, 1},  // sommet 5
		{36, 32, 1},
		{36, 39, 1},  // sommet 6
		{37, 36, 1},
		{37, 33, 1},  // sommet 7
		{38, 31, 1},
		{38, 30, 1},  // sommet 8
		{39, 37, 1},
		{39, 35, 1},  // sommet 9
	};
	int n = 60;
	int added = 0;
	for (int i = 1; i <= 3; i++) {
		for (int j = 0; j < 10; j++) {
			aretes[n + ((i - 1) * 10) + j + 1] = (struct Arete){i, i * 10 + j, 1};
			added += 1;
		}
	}
	n += added; // added = 30
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
	// printf("noeud_actuel %d \n", noeud_actuel);
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

char* concatStr(char* str1, char* str2, char* buffer, int bufferSize) {
	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if (len1 + len2 + 1 > bufferSize) {
		printf("Error: Buffer is too small.\n");
		return "";
	}

	strcpy(buffer, str1);
	strcat(buffer, str2);

	return buffer;
}

Pattern pattern_initialisation() {
	Pattern pat;
	for (int i = 0; i < 40; i++) {
		pat.Mat[i] = (Matrice){NULL, 0, 0};
	}
	pat.graphe = create_graph();

	char* pattern[] = {
		"pattern0.txt",
		"pattern1.txt",
		"pattern2.txt",
		"pattern3.txt",
		"pattern4.txt",
		"pattern5.txt",
		"pattern6.txt",
		"pattern7.txt",
		"pattern8.txt",
		"pattern9.txt",
		"END"};
	char buffer[100];
	for (int i = 0; strcmp(pattern[i], "END"); i++) {
		pat.Mat[10 + i] = recup_matrice(concatStr("./Patterns/low/", pattern[i], buffer, sizeof(buffer)));
		pat.Mat[20 + i] = recup_matrice(concatStr("./Patterns/mid/", pattern[i], buffer, sizeof(buffer)));
		pat.Mat[30 + i] = recup_matrice(concatStr("./Patterns/high/", pattern[i], buffer, sizeof(buffer)));
	}

	char* end_low = "./Patterns/last_pattern_low.txt";
	char* end_mid = "./Patterns/last_pattern_mid.txt";
	char* end_high = "./Patterns/last_pattern_high.txt";

	// récupération des matrices liées aux patterns
	pat.end_low = recup_matrice(end_low);
	pat.end_mid = recup_matrice(end_mid);
	pat.end_high = recup_matrice(end_high);

	return pat;
}

void free_pattern(Pattern pat) {
	for (int i = 0; i < 40; i++) {
		if (pat.Mat[i].cols != 0 && pat.Mat[i].rows != 0) {
			libererMatrice(pat.Mat[i]);
		}
	}
	libererMatrice(pat.end_low);
	libererMatrice(pat.end_mid);
	libererMatrice(pat.end_high);
	free_graph(pat.graphe);
}

// Fonction pour créer un fichier .txt à partir des fichiers patterns concaténés selon le parcours du graphe
void create_map_txt(Pattern pat, char* file) {
	char* start_low = "./Patterns/first_pattern_low.txt";
	char* start_mid = "./Patterns/first_pattern_mid.txt";
	char* start_high = "./Patterns/first_pattern_high.txt";

	int noeud_actuel = random_number(1, 3);
	Matrice M;
	switch (noeud_actuel) {
	case 1:
		M = recup_matrice(start_low);
		break;
	case 2:
		M = recup_matrice(start_mid);
		break;
	case 3:
		M = recup_matrice(start_high);
		break;
	}

	int pattern2big = 0;
	while (M.cols <= MAX_TILES - pat.end_low.cols && pattern2big == 0) {
		noeud_actuel = parcours_graphe(pat.graphe, noeud_actuel);
		M = concatenerMatrices(M, pat.Mat[noeud_actuel], &pattern2big);
	}
	if (noeud_actuel >= 10 && noeud_actuel < 20) {
		M = concatenerMatrices(M, pat.end_low, &pattern2big);
	} else if (noeud_actuel >= 20 && noeud_actuel < 30) {
		M = concatenerMatrices(M, pat.end_mid, &pattern2big);
	} else if (noeud_actuel >= 30 && noeud_actuel < 40) {
		M = concatenerMatrices(M, pat.end_high, &pattern2big);
	}
	matrice_vers_file(M, "test.txt");
	libererMatrice(M);
}

// int main(){
//     Pattern pat = pattern_initialisation();
//     create_map_txt(pat, "test.txt");
//     return 0;
// }
