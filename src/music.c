#include "../include/music.h"
// #include </System/Volumes/Data/opt/homebrew/include/SDL2/SDL_mixer.h>

// Fonction pour lire un fichier audio
void read_audio_file(char* filepath) {

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        exit(-1);
    }

    // Ouverture du périphérique audio avec : 
    // une fréquence d'échantillonnage de 44100 Hz, 
    // un format audio par défaut de 16 bits, 
    // 2 canaux pour le stéréo et 
    // un buffer (tampon audio) de 2048 octets

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Failed to open audio: %s\n", Mix_GetError());
        exit(-1);
    }

    // Chargement du fichier audio
    Mix_Music *music = Mix_LoadMUS(filepath);


    if (music == NULL) {
        printf("Failed to load music: %s\n", Mix_GetError());
        exit(-1);
    }

    // Lecture du fichier audio
    // Deuxième argument : nombre de répétitions ; -1 pour une lecture infinie
    Mix_PlayMusic(music, 1);

    // Attente de la fin de la lecture
    while (Mix_PlayingMusic()) {
        SDL_Delay(100);
    }

    // Libération de la mémoire
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
}

// Fonction pour jouer la musique
void play_music(){
    char* music_path = "../Music/Transforyou.mp3";
    if (Music == 1) {
        read_audio_file(music_path);
    }
}

// int main() {
//     // Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
//     play_music();
//     return 0;
// }
