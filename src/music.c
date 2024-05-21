#include "../include/music.h"
// #include </System/Volumes/Data/opt/homebrew/include/SDL2/SDL_mixer.h>

// Fonction pour lire un fichier audio
Mix_Music* read_audio_file(char* music_path) {

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
    Mix_Music *music = Mix_LoadMUS(music_path);

    // Vérification du chargement du fichier audio
    if (music == NULL) {
        printf("Failed to load music: %s\n", Mix_GetError());
        exit(-1);
    }

    return music;
}

// Fonction pour jouer la musique
void play_music(bool play, Mix_Music* music){
    if (play == true) {
        Mix_PlayMusic(music, -1);
    }
    if (play == false) {
        Mix_HaltMusic();
        }
}

// Fonction pour libérer la mémoire allouée à la musique
void free_music(Mix_Music* music){

    Mix_HaltMusic();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
}

// int main() {
//     // Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
//     play_music();
//     return 0;
// }
