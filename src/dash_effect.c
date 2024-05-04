#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/dash_effect.h"

void apply_box_blur(unsigned char* data, int width, int height, int channels, int radius) {
    unsigned char* temp = (unsigned char*)malloc(width * height * channels);
    memcpy(temp, data, width * height * channels);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sum[4] = {0};
            int count = 0;

            for (int ky = -radius; ky <= radius; ky++) {
                for (int kx = -radius; kx <= radius; kx++) {
                    int nx = x + kx;
                    int ny = y + ky;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        int index = (ny * width + nx) * channels;
                        sum[0] += temp[index];
                        sum[1] += temp[index + 1];
                        sum[2] += temp[index + 2];
                        sum[3] += temp[index + 3];
                        count++;
                    }
                }
            }

            int index = (y * width + x) * channels;
            data[index] = sum[0] / count;
            data[index + 1] = sum[1] / count;
            data[index + 2] = sum[2] / count;
            data[index + 3] = sum[3] / count;
        }
    }

    free(temp);
}

void apply_ghostly_effect(const char* input_file, const char* output_file_prefix, int num_frames, int blur_radius) {
    int width, height, channels;
    unsigned char* image_data = stbi_load(input_file, &width, &height, &channels, 0);

    if (image_data == NULL) {
        printf("Failed to load image: %s\n", input_file);
        return;
    }

    size_t image_size = width * height * channels;
    unsigned char* prev_frame = (unsigned char*)malloc(image_size);
    memcpy(prev_frame, image_data, image_size);

    char output_file[256];
    snprintf(output_file, sizeof(output_file), "%s_%03d.png", output_file_prefix, 0);
    stbi_write_png(output_file, width, height, channels, prev_frame, width * channels);

    for (int frame = 1; frame < num_frames; frame++) {
        unsigned char* ghostly_data = (unsigned char*)malloc(image_size);

        // 1. Create a grayscale version
        for (int i = 0; i < image_size; i += channels) {
            unsigned char gray = (unsigned char)(0.299f * prev_frame[i] + 0.587f * prev_frame[i + 1] + 0.114f * prev_frame[i + 2]);
            ghostly_data[i] = gray;
            ghostly_data[i + 1] = gray;
            ghostly_data[i + 2] = gray;
            ghostly_data[i + 3] = prev_frame[i + 3]; // Copy alpha channel
        }

        // 2. Apply transparency and blur
        float transparency = 1.0f - (float)(frame * frame) / ((num_frames - 1) * (num_frames - 1));
        for (int i = 0; i < image_size; i += channels) {
            ghostly_data[i + 3] = (unsigned char)(ghostly_data[i + 3] * transparency);
        }

        apply_box_blur(ghostly_data, width, height, channels, blur_radius);

        // 4. Save the result
        snprintf(output_file, sizeof(output_file), "%s_%03d.png", output_file_prefix, frame);
        stbi_write_png(output_file, width, height, channels, ghostly_data, width * channels);

        free(prev_frame);
        prev_frame = ghostly_data;
    }

    free(prev_frame);
    stbi_image_free(image_data);
}
//
//void apply_dash_effect(SDL_Renderer* renderer, Character* character, Texture* textures, int num_frames, float dash_duration, float fade_duration) {
//    int frame_index = 0;
//    float elapsed_time = 0.0f;
//    float total_duration = dash_duration + fade_duration;
//    float frame_duration = dash_duration / (float)num_frames;
//
//    SDL_Rect src_rect = {0};
//    SDL_Rect dst_rect = {
//            character->x - character->width / 2,
//            character->y - character->height / 2,
//            character->width,
//            character->height
//    };
//
//    Uint32 start_time = SDL_GetTicks();
//
//    while (elapsed_time < total_duration) {
//        Uint32 current_time = SDL_GetTicks();
//        elapsed_time = (current_time - start_time) / 1000.0f;
//
//        float alpha = 1.0f;
//
//        if (elapsed_time < dash_duration) {
//            // During the dash, update the frame index
//            frame_index = (int)((elapsed_time / frame_duration) + 0.5f);
//            if (frame_index >= num_frames) {
//                frame_index = num_frames - 1;
//            }
//        } else {
//            // After the dash, fade out the trail
//            float fade_progress = (elapsed_time - dash_duration) / fade_duration;
//            alpha = 1.0f - fade_progress;
//            frame_index = num_frames - 1;
//        }
//
//        SDL_SetTextureAlphaMod(textures->trail_frames[frame_index], (Uint8)(alpha * 255));
//        SDL_RenderCopy(renderer, textures->trail_frames[frame_index], &src_rect, &dst_rect);
//
//        // Update the character's position or draw the character here
//        // ...
//
//        SDL_RenderPresent(renderer);
//    }
//}
