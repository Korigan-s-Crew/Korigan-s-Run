#include "main.h"
#include <SDL2/SDL_mixer.h>

// Variable globale pour activer ou désactiver la musique
// Music = true pour activer la musique
// Music = false pour désactiver la musique
extern bool Music;

void read_audio_file(char* filepath);

void play_music();

void free_music(music_path);

//int main();

// il faut installer la bibliothèque SDL2_mixer pour compiler ce fichier : 
// libsdl2-mixer-dev 

// la ligne de commande pour compiler le fichier .c associé est :
// clang music.c -o music -L/opt/homebrew/lib -lSDL2 -lSDL2_mixer 

// la ligne de commande pour exécuter le fichier .c associé est :
// ./music
