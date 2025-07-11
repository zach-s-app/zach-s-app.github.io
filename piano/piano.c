#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 300
#define SAMPLE_RATE 48000
#define AMPLITUDE 28000

// Frequencies for 13 piano keys (C to C including sharps)
double notes_freq[13] = {
    261.63, // C
    277.18, // C#
    293.66, // D
    311.13, // D#
    329.63, // E
    349.23, // F
    369.99, // F#
    392.00, // G
    415.30, // G#
    440.00, // A
    466.16, // A#
    493.88, // B
    523.25  // High C
};

SDL_AudioDeviceID audio_dev;
int16_t *current_sound = NULL;
int sound_len = 0;

int16_t* generate_tone(double freq, int duration_ms, int *out_len) {
    int samples = (SAMPLE_RATE * duration_ms) / 1000;
    int16_t *buffer = malloc(samples * sizeof(int16_t));
    if (!buffer) return NULL;
    for (int i = 0; i < samples; i++) {
        double t = (double)i / SAMPLE_RATE;
        buffer[i] = (int16_t)(AMPLITUDE * sin(2.0 * M_PI * freq * t));
    }
    *out_len = samples;
    return buffer;
}

void play_note(double freq) {
    if (current_sound) free(current_sound);
    SDL_ClearQueuedAudio(audio_dev);
    current_sound = generate_tone(freq, 500, &sound_len);
    if (current_sound) SDL_QueueAudio(audio_dev, current_sound, sound_len * sizeof(int16_t));
}

int is_black_key(int index) {
    return (index == 1 || index == 3 || index == 6 || index == 8 || index == 10);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window *window = SDL_CreateWindow("SDL2 Piano with Black Keys",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_AudioSpec want = {0}, have;
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 1024;

    audio_dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    SDL_PauseAudioDevice(audio_dev, 0);

    int running = 1;
    SDL_Event event;

    while (running) {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        int white_key_w = SCREEN_WIDTH / 8;

        // Draw white keys
        for (int i = 0; i < 8; i++) {
            SDL_Rect key = { i * white_key_w, 0, white_key_w, SCREEN_HEIGHT };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &key);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &key);
        }

        // Draw black keys
        for (int i = 0; i < 13; i++) {
            if (is_black_key(i)) {
                int x_offset = (i * white_key_w / 2) + (white_key_w / 4);
                SDL_Rect black_key = { x_offset, 0, white_key_w / 2, SCREEN_HEIGHT / 2 };
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &black_key);
            }
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Check black keys first (they sit on top)
                for (int i = 0; i < 13; i++) {
                    if (is_black_key(i)) {
                        int x_offset = (i * white_key_w / 2) + (white_key_w / 4);
                        if (x >= x_offset && x < x_offset + white_key_w / 2 && y < SCREEN_HEIGHT / 2) {
                            play_note(notes_freq[i]);
                        }
                    }
                }

                // Check white keys
                int key_index = x / white_key_w;
                if (key_index >= 0 && key_index < 8) {
                    play_note(notes_freq[key_index * 2]);
                }
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_a: play_note(notes_freq[0]); break;  // C
                    case SDLK_w: play_note(notes_freq[1]); break;  // C#
                    case SDLK_s: play_note(notes_freq[2]); break;  // D
                    case SDLK_e: play_note(notes_freq[3]); break;  // D#
                    case SDLK_d: play_note(notes_freq[4]); break;  // E
                    case SDLK_f: play_note(notes_freq[5]); break;  // F
                    case SDLK_t: play_note(notes_freq[6]); break;  // F#
                    case SDLK_g: play_note(notes_freq[7]); break;  // G
                    case SDLK_y: play_note(notes_freq[8]); break;  // G#
                    case SDLK_h: play_note(notes_freq[9]); break;  // A
                    case SDLK_u: play_note(notes_freq[10]); break; // A#
                    case SDLK_j: play_note(notes_freq[11]); break; // B
                    case SDLK_k: play_note(notes_freq[12]); break; // High C
                }
            }
        }
    }

    if (current_sound) free(current_sound);
    SDL_CloseAudioDevice(audio_dev);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

