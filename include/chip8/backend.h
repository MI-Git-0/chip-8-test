#pragma once
#include <stdint.h>

#define BACKEND_QUIT 1
#define BACKEND_NONE 0

#define BACKEND_DISPLAY_WIDTH 64  // Change these and you'll regret it //
#define BACKEND_DISPLAY_HEIGHT 32 // ---------------------------------- //

#define BACKEND_DISPLAY_SIZE (BACKEND_DISPLAY_WIDTH * BACKEND_DISPLAY_HEIGHT)
#define BACKEND_DISPLAY_SCALE 15  // The actual amount of pixels that's used for the width and length of individual interpreted pixels, safe to change

typedef struct BACKEND_Controller BACKEND_Controller; // The main struct used to connect the backend and the main.c interpreter

BACKEND_Controller* BACKEND_create_controller(const uint8_t* display, uint8_t* keyboard); // One BACKEND_Controller is made in main.c before starting its loop
void BACKEND_destroy_controller(BACKEND_Controller* controller); // Called before the program closes, deallocate the controller with your backend
void BACKEND_update_display(BACKEND_Controller* controller); // Based on the current state of the display, render the game
void BACKEND_handle_sound(BACKEND_Controller* controller, uint8_t enabled); // Play the beep sound, Chip8 only has one tone
void BACKEND_wait_ns(long ns); // Use the backend's method of yielding the thread for specified amount of nanoseconds
uint8_t BACKEND_update(BACKEND_Controller* controller); // This is ran once per iteration, use this to handle the state of the controller.

// Keep every definition the same here, changing it won't compile unless main.c is changed too