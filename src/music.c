#include "../include/music.h"

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
void play_music(Mix_Music* music){
	if (Mix_PlayingMusic() == 0) {
		// Play the music
		Mix_PlayMusic(music, -1);
	}
	// If music is being played
	else {
		// If the music is paused
		if (Mix_PausedMusic() == 1) {
			// Resume the music
			Mix_ResumeMusic();
		}
		// If the music is playing
		else {
			// Pause the music
			Mix_PauseMusic();
		}
	}
}

bool is_music_playing(){
    return Mix_PlayingMusic();
}

// Fonction pour libérer la mémoire allouée à la musique
void free_music(Mix_Music* music){

    Mix_HaltMusic();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}

// int main() {
//     // Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
//     play_music();
//     return 0;
// }
