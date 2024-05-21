#include "../include/test.h"



int main() {
    Mix_Music* music = read_audio_file("../Music/Transforyou.mp3");
    play_music(true, music);
    free_music(music);
    return 0;
}
