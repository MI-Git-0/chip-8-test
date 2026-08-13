#pragma once
#include <stdint.h>

#define BACKEND_QUIT 1
#define BACKEND_NONE 0

#define BACKEND_DISPLAY_SIZE (64 * 32)
#define BACKEND_DISPLAY_WIDTH 64
#define BACKEND_DISPLAY_HEIGHT 32
#define BACKEND_DISPLAY_SCALE 15

typedef struct BACKEND_Controller BACKEND_Controller;

BACKEND_Controller* BACKEND_create_controller(const uint8_t* display, uint8_t* keyboard);
void BACKEND_destroy_controller(BACKEND_Controller* controller);
void BACKEND_update_display(BACKEND_Controller* controller);
void BACKEND_handle_sound(BACKEND_Controller* controller, uint8_t enabled);
void BACKEND_wait_ns(long ns);
uint8_t BACKEND_update(BACKEND_Controller* controller);