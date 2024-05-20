#include "main.h"
#include <SDL2/SDL_mixer.h>

#define Music 1

// Fonction pour lire un fichier audio
void read_audio_file(char* filepath);

void play_music();

int main();

// il faut installer la bibliothèque SDL2_mixer pour compiler ce fichier : 
// libsdl2-mixer-dev 

// la ligne de commande pour compiler le fichier .c associé est :
// clang music.c -o music -L/opt/homebrew/lib -lSDL2 -lSDL2_mixer 

// la ligne de commande pour exécuter le fichier .c associé est :
// ./music
