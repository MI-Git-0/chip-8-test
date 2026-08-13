#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include "chip8/defs.h"
#include "chip8/backend.h"

#define VX_REG(op) v_reg[CH8_GET_VX_INDEX(op)]
#define VY_REG(op) v_reg[CH8_GET_VY_INDEX(op)]
#define NO_KEYS_PRESSED 99

typedef struct timespec timespec;

uint8_t draw_sprite(uint8_t* restrict display, const uint8_t* restrict sprite_bytes, uint8_t sprite_height, uint8_t x, uint8_t y)
{
	uint8_t collision = 0;

	for (int byte_index = 0; byte_index < sprite_height; byte_index++)
	{
		for (int bit_offset = 7; bit_offset >= 0; bit_offset--)
		{
			uint8_t x_adjusted = (x + (7 - bit_offset)) % BACKEND_DISPLAY_WIDTH;
			int display_index = x_adjusted + BACKEND_DISPLAY_WIDTH * y;
			uint8_t pixel_value = (sprite_bytes[byte_index] >> bit_offset) & 0x1;

			if (display[display_index] & pixel_value) collision = 1;
			display[display_index] ^= pixel_value;
		}
		y = (y + 1) % BACKEND_DISPLAY_HEIGHT;
	}

	return collision;
}

uint8_t any_key_pressed(uint8_t* keyboard, uint8_t* previous_keyboard)
{
	for (int i = 0; i < CH8_KEYBOARD_SIZE; i++)
	{
		if (keyboard[i] && !previous_keyboard[i]) return i;
	}
	return NO_KEYS_PRESSED;
}

int main(int argc, char* argv[])
{
	uint8_t chip_state[CH8_CHIP_STATE_SIZE] = {0};
	uint8_t v_reg[16];
	uint16_t i_reg;
	uint16_t pc = CH8_CHIP_START_OFFSET;
	uint16_t stack[16];
	uint8_t stack_p = 0;
	uint8_t delay_timer = 0;
	uint8_t sound_timer = 0;

	srand(time(NULL));
	FILE* ch8_rom;
	CH8_get_font_bytes(chip_state);

	if (argc == 2)
	{
		ch8_rom = fopen(argv[1], "rb");
	}
	else
	{
		ch8_rom = fopen("roms/ibm-logo.ch8", "r");
	}
	if (!ch8_rom)
	{
		perror("chip8 rom unavailable");
		return 1;
	}
	int fseek_result = fseek(ch8_rom, 0, SEEK_END) != 0;
	if (fseek_result)
	{
		perror("fseek error");
		goto SETUP_ERROR;
	}

	long ftell_result = ftell(ch8_rom);
	if (ftell_result == -1)
	{
		perror("ftell error");
		goto SETUP_ERROR;
	}

	rewind(ch8_rom);
	size_t rom_size = (size_t)ftell_result;

	if (CH8_CHIP_STATE_SIZE < CH8_CHIP_START_OFFSET + rom_size)
	{
		printf("Rom file is too large to load");
		goto SETUP_ERROR;
	}

	size_t bytes_read = fread(chip_state + CH8_CHIP_START_OFFSET, 1, rom_size, ch8_rom);
	if (bytes_read != rom_size)
	{
		perror("Rom file read incorrectly");
		goto SETUP_ERROR;
	}
	fclose(ch8_rom);

	int8_t running = 1;
	uint8_t display[BACKEND_DISPLAY_SIZE] = {0};
	uint8_t keyboard[CH8_KEYBOARD_SIZE] = {0};
	uint8_t previous_keyboard[CH8_KEYBOARD_SIZE] = {0};

	BACKEND_Controller* controller = BACKEND_create_controller(display, keyboard);
	if (!controller)
	{
		printf("Backend Controller failed to initialize\n");
		return 1;
	}

	while (running)
	{
		timespec start_time;
		if (timespec_get(&start_time, TIME_UTC) != TIME_UTC) break;

		uint8_t event = BACKEND_update(controller);
		if (event == BACKEND_QUIT) break;

		for (int i = 0; i < CH8_INSTRUCTIONS_PER_SIMULATION; i++)
		{
			uint16_t opcode = chip_state[pc] << 8 | chip_state[pc + 1];
			pc += 2;
			switch (CH8_GET_FN(opcode))
			{
				case CH8_FN_MISC_0:
				{
					switch(CH8_GET_LB(opcode))
					{
						case CH8_LB00_CLEAR_DISPLAY: memset(display, 0, sizeof(display)); BACKEND_update_display(controller); break;
						case CH8_LB01_RETURN: pc = stack[--stack_p]; break;
					}
					break;
				}

				case CH8_OP_JUMP: pc = CH8_GET_NNN(opcode); break;
				case CH8_OP_CALL: stack[stack_p++] = pc; pc = CH8_GET_NNN(opcode); break;
				case CH8_OP_SKIP_IF_VX_EQ: if (VX_REG(opcode) == CH8_GET_LB(opcode)) pc += 2; break;
				case CH8_OP_SKIP_IF_VX_NOT: if (VX_REG(opcode) != CH8_GET_LB(opcode)) pc += 2; break;
				case CH8_OP_SKIP_IF_VXVY_EQ: if (VX_REG(opcode) == VY_REG(opcode)) pc += 2; break;
				case CH8_OP_SET_VX: VX_REG(opcode) = CH8_GET_LB(opcode); break;
				case CH8_OP_ADD_VXLB: VX_REG(opcode) += CH8_GET_LB(opcode); break;

				case CH8_FN_MISC_8:
				{
					switch (CH8_GET_LN(opcode))
					{
						case CH8_LN80_SET_VXVY: VX_REG(opcode) = VY_REG(opcode); break;
						case CH8_LN81_SET_VX_VXORVY: VX_REG(opcode) = VX_REG(opcode) | VY_REG(opcode); break;
						case CH8_LN82_SET_VX_VXANDVY: VX_REG(opcode) = VX_REG(opcode) & VY_REG(opcode); break;
						case CH8_LN83_SET_VX_VXXORVY: VX_REG(opcode) = VX_REG(opcode) ^ VY_REG(opcode); break;
						case CH8_LN84_ADD_VXVY:
						{
							uint8_t x = VX_REG(opcode), y = VY_REG(opcode);
							VX_REG(opcode) = x + y;
							v_reg[0xF] = (x + y) > 255;
							break;
						}
						case CH8_LN85_SUB_VXVY:
						{
							uint8_t x = VX_REG(opcode), y = VY_REG(opcode);
							VX_REG(opcode) = x - y;
							v_reg[0xF] = x >= y;
							break;
						}
						case CH8_LN86_SHR_VX:
						{
							uint8_t x = VX_REG(opcode);
							VX_REG(opcode) = x >> 1;
							v_reg[0xF] = CH8_GET_LSB(x);
							break;
						}
						case CH8_LN87_SUB_VYVX_TO_VX:
						{
							uint8_t x = VX_REG(opcode), y = VY_REG(opcode);
							VX_REG(opcode) = y - x;
							v_reg[0xF] = y >= x;
							break;
						}
						case CH8_LN88_SHL_VX:
						{
							uint8_t x = VX_REG(opcode);
							VX_REG(opcode) = x << 1;
							v_reg[0xF] = CH8_GET_MSB(x);
							break;
						}
					}
					break;
				}

				case CH8_OP_SKIP_IF_VXVY_NOTEQ: if (VX_REG(opcode) != VY_REG(opcode)) pc += 2; break;
				case CH8_OP_SET_I: i_reg = CH8_GET_NNN(opcode); break;
				case CH8_OP_JUMP_PLUS_V0: i_reg = CH8_GET_NNN(opcode) + v_reg[0]; break;
				case CH8_OP_SET_VX_RANDOM_AND_LB: VX_REG(opcode) = (rand() & 0xFF) & CH8_GET_LB(opcode); break;

				case CH8_OP_DRAW:
				{
					uint8_t sprite_height = CH8_GET_LN(opcode);
					uint8_t x_position = VX_REG(opcode);
					uint8_t y_position = VY_REG(opcode);
					uint8_t sprite_bytes[8 * sprite_height];
					memcpy(sprite_bytes, &chip_state[i_reg], sprite_height);

					v_reg[0xF] = draw_sprite(display, sprite_bytes, sprite_height, x_position, y_position);
					BACKEND_update_display(controller);
					break;
				}

				case CH8_FN_MISC_E:
				{
					switch (CH8_GET_LN(opcode))
					{
						case CH8_LNE0_SKIP_IF_VX_KEY_DOWN: if (keyboard[VX_REG(opcode)]) pc += 2; break;
						case CH8_LNE1_SKIP_IF_VX_KEY_UP: if (!keyboard[VX_REG(opcode)]) pc += 2; break;
					}
					break;
				}

				case CH8_FN_MISC_F:
				{
					switch (CH8_GET_LB(opcode))
					{
						case CH8_LBF0_SET_VXDT: VX_REG(opcode) = delay_timer; break;
						case CH8_LBF1_WAIT_FOR_KEYPRESS:
						{
							uint8_t key_pressed = any_key_pressed(keyboard, previous_keyboard);
							if (key_pressed == NO_KEYS_PRESSED) { pc-= 2; break; }
							VX_REG(opcode) = key_pressed;
							break;
						}
						case CH8_LBF2_SET_DTVX: delay_timer = VX_REG(opcode); break;
						case CH8_LBF3_SET_STVX: sound_timer = VX_REG(opcode); break;
						case CH8_LBF4_ADD_IVX: i_reg += VX_REG(opcode); break;
						case CH8_LBF5_SET_I_VX_SPRITE_LOCATION: i_reg = VX_REG(opcode) * CH8_FONT_BYTES_PER_DIGIT; break;
						case CH8_LBF6_BCD_VX_TO_I:
						{
							uint8_t number = VX_REG(opcode);
							chip_state[i_reg] = number / 100;
							chip_state[i_reg+1] = (number / 10) % 10;
							chip_state[i_reg+2] = number % 10;
							break;
						}
						case CH8_LBF7_REGS_TO_MEM_AT_I: memcpy(&chip_state[i_reg], v_reg, CH8_GET_VX_INDEX(opcode) + 1); break;
						case CH8_LBF8_MEM_TO_REGS_AT_I: memcpy(v_reg, &chip_state[i_reg], CH8_GET_VX_INDEX(opcode) + 1); break;
					}
					break;
				}
			}
		}

		BACKEND_handle_sound(controller, sound_timer);
		if (delay_timer > 0) delay_timer--;
		if (sound_timer > 0) sound_timer--;

		memcpy(previous_keyboard, keyboard, CH8_KEYBOARD_SIZE);

		timespec end_time;
		if (timespec_get(&end_time,TIME_UTC) != TIME_UTC) break;

		long elapsed_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000L + (end_time.tv_nsec - start_time.tv_nsec);
		long frame_max_ns = 1000000000L / CH8_SIMULATION_HZ;
		long wait_ns = frame_max_ns - elapsed_ns;
		if (wait_ns > 0) BACKEND_wait_ns(wait_ns);
	}

	BACKEND_destroy_controller(controller);
	return 0;

	SETUP_ERROR:
	fclose(ch8_rom);
	return 1;
}