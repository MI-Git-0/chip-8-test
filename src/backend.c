#include "chip8/backend.h"
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <math.h>

struct BACKEND_Controller
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_AudioStream* audio_stream;
    uint8_t* const keyboard;
    const uint8_t* const display;
};

BACKEND_Controller* BACKEND_create_controller(const uint8_t* display, uint8_t* keyboard)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) goto SDL_INIT_ERROR;
    BACKEND_Controller base_controller = {.display = display, .keyboard = keyboard, .window = NULL, .renderer = NULL, .audio_stream = NULL};
    BACKEND_Controller* controller = malloc(sizeof(BACKEND_Controller));
    if (!controller) goto CONTROLLER_ALLOC_ERROR;
    memcpy(controller, &base_controller, sizeof(BACKEND_Controller));

    controller->window = SDL_CreateWindow("Chip8 Via SDL", BACKEND_DISPLAY_WIDTH * BACKEND_DISPLAY_SCALE, 
    BACKEND_DISPLAY_HEIGHT * BACKEND_DISPLAY_SCALE, 0);
    if (!controller->window) goto WINDOW_ERROR;

    controller->renderer = SDL_CreateRenderer(controller->window, NULL);
    if (!controller->renderer) goto RENDERER_ERROR;

    SDL_AudioSpec audio_spec = {.channels = 1, .format = SDL_AUDIO_F32, .freq = 48000};
    controller->audio_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_spec, NULL, NULL);
    if (!controller->audio_stream) goto AUDIO_ERROR;
    SDL_ResumeAudioStreamDevice(controller->audio_stream);

    return controller;

AUDIO_ERROR:
    SDL_DestroyRenderer(controller->renderer);
RENDERER_ERROR:
    SDL_DestroyWindow(controller->window);
WINDOW_ERROR:
    free(controller);
CONTROLLER_ALLOC_ERROR:
    SDL_Quit();
SDL_INIT_ERROR:
    return NULL;
}

void BACKEND_destroy_controller(BACKEND_Controller* controller)
{
    SDL_DestroyAudioStream(controller->audio_stream);
    SDL_DestroyRenderer(controller->renderer);
    SDL_DestroyWindow(controller->window);
    free(controller);
    SDL_Quit();
}

void BACKEND_update_display(BACKEND_Controller* controller)
{
    for (int y = 0; y < BACKEND_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < BACKEND_DISPLAY_WIDTH; x++)
        {
            uint8_t color = controller->display[x + BACKEND_DISPLAY_WIDTH * y] * 255;
            SDL_FRect pixel = {x * BACKEND_DISPLAY_SCALE, y * BACKEND_DISPLAY_SCALE, BACKEND_DISPLAY_SCALE, BACKEND_DISPLAY_SCALE};
            SDL_SetRenderDrawColor(controller->renderer, color, color, color, 255);
            SDL_RenderFillRect(controller->renderer, &pixel);
        }
    }
    SDL_RenderPresent(controller->renderer);
}

void BACKEND_handle_sound(BACKEND_Controller* controller, uint8_t enabled)
{
    static float phase = 0.0f;

    float samples[800];

    for (int i = 0; i < 800; i++) {
        if (enabled) {
            samples[i] = sinf(
                phase * 2.0f * 3.14159265358979323846f
            );

            phase += 440.0f / 48000.0f;

            if (phase >= 1.0f)
                phase -= 1.0f;
        } else {
            samples[i] = 0.0f;
        }
    }

    SDL_PutAudioStreamData(
        controller->audio_stream,
        samples,
        sizeof(samples)
    );
}


void BACKEND_wait_ns(long ns)
{
    SDL_DelayPrecise(ns);
}

SDL_Scancode hex_to_key_map[16] =
{
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,

    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_R,

    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_F,

    SDL_SCANCODE_Z,
    SDL_SCANCODE_X,
    SDL_SCANCODE_C,
    SDL_SCANCODE_V
};

uint8_t BACKEND_update(BACKEND_Controller* controller)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return BACKEND_QUIT;
        }
    }

    const bool* keys = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < 16; i++)
    {
        controller->keyboard[i] = keys[hex_to_key_map[i]];
    }

    return BACKEND_NONE;
}