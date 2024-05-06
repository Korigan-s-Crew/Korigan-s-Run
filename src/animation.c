#include "../include/animation.h"

// Buffer to hold character animations
CharacterAnimation character_animations[MAX_CHARACTERS];

// Initialize character animations buffer
void init_character_animations_buffer() {
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        character_animations[i].character = NULL;
        for (int j = 0; j < 100; j++)
            character_animations[i].texture[j] = NULL;
        character_animations[i].dst.x = 0;
        character_animations[i].dst.y = 0;
        character_animations[i].dst.w = 0;
        character_animations[i].dst.h = 0;
        character_animations[i].camera_x = 0;
        character_animations[i].camera_y = 0;
        character_animations[i].nb_frame = 0;
        character_animations[i].start_time = 0;
        character_animations[i].duration = 0;
        character_animations[i].angle = 0;
        character_animations[i].center = NULL;
        character_animations[i].flip = SDL_FLIP_NONE;
        character_animations[i].finished = true;
    }
}

// Add a character animation to the buffer
int add_character_animation(Character *character, SDL_Texture *texture[], Camera *camera, SDL_Rect dst, int nb_frame, int duration, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    // Find the first finished animation in the buffer
    int empty_slot = -1;
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (character_animations[i].finished) {
            empty_slot = i;
            break;
        }
    }

    // If no empty slot is found, return an error
    if (empty_slot == -1) {
        printf("Error: Character animation buffer full\n");
        return -1;
    }

    // Replace the empty slot with the new animation
    character_animations[empty_slot].character = character;
    for (int i = 0; i < nb_frame; i++)
        character_animations[empty_slot].texture[i] = texture[i];
    character_animations[empty_slot].dst = dst; // Assign directly
    character_animations[empty_slot].camera_x = camera->x;
    character_animations[empty_slot].camera_y = camera->y;
    character_animations[empty_slot].nb_frame = nb_frame;
    character_animations[empty_slot].start_time = SDL_GetTicks();
    character_animations[empty_slot].duration = duration;
    character_animations[empty_slot].angle = angle;
    character_animations[empty_slot].center = center;
    character_animations[empty_slot].flip = flip;
    character_animations[empty_slot].finished = false;
//    printf("animation index: %d, dst: %d %d %d %d\n", empty_slot, character_animations[empty_slot].dst.x, character_animations[empty_slot].dst.y, character_animations[empty_slot].dst.w, character_animations[empty_slot].dst.h);
//    print_nb_animation(-1);
    return 0; // Return success
}



// Update character animations in the buffer
void update_character_animations() {
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (character_animations[i].character != NULL && !character_animations[i].finished) {
            Uint32 elapsed_time = SDL_GetTicks() - character_animations[i].start_time;
            Uint32 frame_duration = character_animations[i].duration / character_animations[i].nb_frame;
            int current_frame = elapsed_time / frame_duration;
            if (current_frame >= character_animations[i].nb_frame) {
                character_animations[i].finished = true;
            }
        }
    }
}

// Render character animations in the buffer
void render_character_animations(SDL_Renderer *renderer, Camera *camera){
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (character_animations[i].character != NULL && !character_animations[i].finished) {
            int current_frame = (SDL_GetTicks() - character_animations[i].start_time) /
                                (character_animations[i].duration / character_animations[i].nb_frame);
//            printf("Current frame: %d, animation index: %d, dst: %d %d %d %d\n", current_frame, i, character_animations[i].dst.x, character_animations[i].dst.y, character_animations[i].dst.w, character_animations[i].dst.h);
            SDL_Rect newDST = {character_animations[i].dst.x - (camera->x - character_animations[i].camera_x),
                               character_animations[i].dst.y - (camera->y - character_animations[i].camera_y),
                               character_animations[i].dst.w, character_animations[i].dst.h};
            SDL_RenderCopyEx(renderer, character_animations[i].texture[current_frame], NULL, &newDST,
                             character_animations[i].angle, character_animations[i].center, character_animations[i].flip);
        }
    }
}

int print_nb_animation(int prev_nb) {
    int nb_animation = 0;
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (character_animations[i].character != NULL && !character_animations[i].finished) {
            nb_animation++;
        }
    }
    if (nb_animation != prev_nb) {
        printf("Number of animations: %d\n", nb_animation);
    }
    return nb_animation;
}
