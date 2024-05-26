#include "../include/test.h"



int main() {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	init(&window, &renderer, 100, 100);
	Mix_Music *music = read_audio_file("Music/Transforyou.mp3");
    play_music(music);
    char c;
	scanf(" %c", &c);
	free_music(music);
	if (NULL != renderer)
		SDL_DestroyRenderer(renderer);
	if (NULL != window)
		SDL_DestroyWindow(window);
	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
	return 0;
}
