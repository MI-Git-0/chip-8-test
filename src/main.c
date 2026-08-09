#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "chip8defs.h"

#define CHIP_STATE_SIZE 4096

int main(int argc, char* argv[])
{
	uint8_t chip_state[CHIP_STATE_SIZE];
	uint8_t v_reg[16];
	uint16_t i_reg;
	uint16_t pc = CHIP_START_OFFSET;
	uint16_t stack[16];
	uint8_t stack_p;
	uint8_t delay_timer;
	uint8_t sound_timer;
	memset(chip_state, 0, CHIP_STATE_SIZE);

	FILE* ch8_rom;

	if (argc == 2)
	{
		ch8_rom = fopen(argv[1], "r");
	}
	else
	{
		ch8_rom = fopen("roms/test.ch8", "r");
	}
	if (!ch8_rom)
	{
		perror("chip8 rom unavailable");
		return 1;
	}
	if (!fseek(ch8_rom, 0, SEEK_END))
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

	if (CHIP_STATE_SIZE < CHIP_START_OFFSET + rom_size)
	{
		printf("Rom file is too large to load");
		goto SETUP_ERROR;
	}

	size_t bytes_read = fread(chip_state + CHIP_START_OFFSET, 1, rom_size, ch8_rom);
	if (bytes_read != rom_size)
	{
		perror("Rom file read incorrectly");
		goto SETUP_ERROR;
	}
	fclose(ch8_rom);

	uint16_t opcode = chip_state[pc] << 8 | chip_state[pc + 1];

	/* while true */

	return 0;

	SETUP_ERROR:
	fclose(ch8_rom);
	return 1;
}