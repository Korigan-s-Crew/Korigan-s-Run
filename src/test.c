#include "../include/main.h"
#include "../include/music.h"



int main_test() {
    music = read_audio_file("../Music/Transforyou.mp3");
    play_music(true);
    free_music(music);
    return 0;
}
