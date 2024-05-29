#include <assert.h>

#include "../include/test.h"
#include "../include/implem_map.h"

// Test pour random_number
void test_random_number() {
    int min = 1, max = 10;
    for (int i = 0; i < 1000; i++) {
        int num = random_number(min, max);
        assert(num >= min && num <= max);
    }
    printf("test_random_number passed\n");
}

// Test pour normalize_line
void test_normalize_line() {
    char line[20] = "Hello\nWorld";
    normalize_line(line);
    assert(strcmp(line, "Hello") == 0);

    char line2[20] = "NoNewLineHere";
    normalize_line(line2);
    assert(strcmp(line2, "NoNewLineHere") == 0);

    printf("test_normalize_line passed\n");
}

// Test pour create_graph et free_graph
void test_create_free_graph() {
    struct Graphe *graph = create_graph();
    assert(graph != NULL);

    // Vérification de quelques noeuds aléatoires pour s'assurer qu'ils existent
    assert(graph->noeud[10] != NULL);
    assert(graph->noeud[11] != NULL);
    assert(graph->noeud[12] != NULL);

    free_graph(graph);
    printf("test_create_free_graph passed\n");
}

// Test pour matrice_vers_file et recup_matrice
void test_matrice_vers_file_recup_matrice() {
    Matrice mat = creerMatrice(3, 3);
    mat.data[0][0] = '1'; mat.data[0][1] = '2'; mat.data[0][2] = '3';
    mat.data[1][0] = '4'; mat.data[1][1] = '5'; mat.data[1][2] = '6';
    mat.data[2][0] = '7'; mat.data[2][1] = '8'; mat.data[2][2] = '9';
    
    matrice_vers_file(mat, "../test_matrix.txt");
    
    Matrice mat_recup = recup_matrice("test_matrix.txt");
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            assert(mat.data[i][j] == mat_recup.data[i][j]);
        }
    }

    libererMatrice(mat);
    libererMatrice(mat_recup);

    printf("test_matrice_vers_file_recup_matrice passed\n");
}

// Test pour parcours_graphe
void test_parcours_graphe() {
    struct Graphe *graph = create_graph();
    int start_node = 10;
    int next_node = parcours_graphe(graph, start_node);
    assert(next_node != start_node); // on s'attend à un noeud différent
    free_graph(graph);

    printf("test_parcours_graphe passed\n");
}

// Test pour creerMatrice et libererMatrice
void test_creer_liberer_matrice() {
    int rows = 3, cols = 3;
    Matrice mat = creerMatrice(rows, cols);
    assert(mat.data != NULL);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat.data[i][j] = i * cols + j;
        }
    }

    libererMatrice(mat);

    printf("test_creer_liberer_matrice passed\n");
}

// Test pour concatenerMatrices
void test_concatenerMatrices() {
    Matrice M1 = creerMatrice(2, 2);
    Matrice M2 = creerMatrice(2, 2);

    M1.data[0][0] = 'a'; M1.data[0][1] = 'b';
    M1.data[1][0] = 'c'; M1.data[1][1] = 'd';
    
    M2.data[0][0] = 'e'; M2.data[0][1] = 'f';
    M2.data[1][0] = 'g'; M2.data[1][1] = 'h';

    int pattern2big = 0;
    Matrice concat = concatenerMatrices(M1, M2, &pattern2big, 0, M2.cols);

    assert(concat.rows == 2);
    assert(concat.cols == 4);

    assert(concat.data[0][0] == 'a');
    assert(concat.data[0][1] == 'b');
    assert(concat.data[0][2] == 'e');
    assert(concat.data[0][3] == 'f');

    assert(concat.data[1][0] == 'c');
    assert(concat.data[1][1] == 'd');
    assert(concat.data[1][2] == 'g');
    assert(concat.data[1][3] == 'h');

	libererMatrice(M2);
    libererMatrice(concat);

	printf("test_concatenerMatrices passed\n");
}

// Test pour concatStr
void test_concatStr() {
    char buffer[100];
    char *result = concatStr("Hello", "World", buffer, sizeof(buffer));
    assert(strcmp(result, "HelloWorld") == 0);

    printf("test_concatStr passed\n");
}

// Test pour pattern_initialisation et free_pattern
void test_pattern_initialisation_free_pattern() {
    Pattern pat = pattern_initialisation();
    assert(pat.graphe != NULL);

    free_pattern(pat);

    printf("test_pattern_initialisation_free_pattern passed\n");
}

// Test pour create_map_txt
void test_create_map_txt() {
    Pattern pat = pattern_initialisation();
    create_map_txt(pat, "map.txt");

    FILE *file = fopen("map.txt", "r");
    assert(file != NULL);
    fclose(file);

    free_pattern(pat);

    printf("test_create_map_txt passed\n");
}


int main() {
	
	// SDL_Window *window = NULL;
	// SDL_Renderer *renderer = NULL;
	// init(&window, &renderer, 100, 100);
	// Mix_Music *music = read_audio_file("Music/Transforyou.mp3");
    // play_music(music);
    // char c;
	// scanf(" %c", &c);
	// free_music(music);
	// if (NULL != renderer)
	// 	SDL_DestroyRenderer(renderer);
	// if (NULL != window)
	// 	SDL_DestroyWindow(window);
	// IMG_Quit();
	// Mix_Quit();
	// SDL_Quit();

	// Tests sur les fonctions de implem_map.c 
	test_random_number();
    test_normalize_line();
    test_create_free_graph();
    test_matrice_vers_file_recup_matrice();
    test_parcours_graphe();
    test_creer_liberer_matrice();
    test_concatenerMatrices();
    test_concatStr();
    test_pattern_initialisation_free_pattern();
    test_create_map_txt();

	return 0;
}
