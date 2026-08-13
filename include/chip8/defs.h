#pragma once

#include <stdint.h>

#define CH8_CHIP_STATE_SIZE 4096
#define CH8_CHIP_START_OFFSET 0x200
#define CH8_SIMULATION_HZ 60
#define CH8_INSTRUCTIONS_PER_SIMULATION 10
#define CH8_KEYBOARD_SIZE 16

#define CH8_GET_VX_INDEX(op) (((op) >> 8) & 0x000F)
#define CH8_GET_VY_INDEX(op) (((op) >> 4) & 0x000F)
#define CH8_GET_FN(op) ((op) >> 12)
#define CH8_GET_LN(op) ((op) & 0x000F)
#define CH8_GET_LB(op) ((op) & 0x00FF)
#define CH8_GET_NNN(op) ((op) & 0x0FFF)
#define CH8_GET_LSB(op) ((op) & 0x1)
#define CH8_GET_MSB(op) (((op) >> 7) & 0x1)

#define CH8_FN_MISC_0 0x0
#define CH8_LB00_CLEAR_DISPLAY 0xE0 // Clear the display
#define CH8_LB01_RETURN 0xEE // Return from a subroutine
#define CH8_OP_JUMP 0x1 // Jump to location nnn
#define CH8_OP_CALL 0x2 // Call subroutine at nnn
#define CH8_OP_SKIP_IF_VX_EQ 0x3 // Skip next instruction if Vx = kk
#define CH8_OP_SKIP_IF_VX_NOT 0x4 // Skip next instruction if Vx != kk
#define CH8_OP_SKIP_IF_VXVY_EQ 0x5 // Skip next instruction if Vx = Vy
#define CH8_OP_SET_VX 0x6 // Set Vx = kk
#define CH8_OP_ADD_VXLB 0x7 // Set Vx = Vx + kk
#define CH8_FN_MISC_8 0x8
#define CH8_LN80_SET_VXVY 0x0 // Set Vx = Vy
#define CH8_LN81_SET_VX_VXORVY 0x1 // Set Vx = Vx OR Vy
#define CH8_LN82_SET_VX_VXANDVY 0x2 // Set Vx = Vx AND Vy
#define CH8_LN83_SET_VX_VXXORVY 0x3 // Set Vx = Vx XOR Vy
#define CH8_LN84_ADD_VXVY 0x4 // Set Vx = Vx + Vy, set VF = carry
#define CH8_LN85_SUB_VXVY 0x5 // Set Vx = Vx - Vy, set VF = NOT borrow
#define CH8_LN86_SHR_VX 0x6 // Set Vx = Vx SHR 1
#define CH8_LN87_SUB_VYVX_TO_VX 0x7 // Set Vx = Vy - Vx, set VF = NOT borrow
#define CH8_LN88_SHL_VX 0xE // Set Vx = Vx SHL 1
#define CH8_OP_SKIP_IF_VXVY_NOTEQ 0x9 // Skip next instruction if Vx != Vy
#define CH8_OP_SET_I 0xA // Set I = nnn
#define CH8_OP_JUMP_PLUS_V0 0xB // Jump to location nnn + V0
#define CH8_OP_SET_VX_RANDOM_AND_LB 0xC // Set Vx = random byte AND kk
#define CH8_OP_DRAW 0xD // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
#define CH8_FN_MISC_E 0xE
#define CH8_LNE0_SKIP_IF_VX_KEY_DOWN 0xE // Skip next instruction if key with the value of Vx is pressed
#define CH8_LNE1_SKIP_IF_VX_KEY_UP 0x1 // Skip next instruction if key with the value of Vx is not pressed
#define CH8_FN_MISC_F 0xF
#define CH8_LBF0_SET_VXDT 0x07 // Set Vx = delay timer value
#define CH8_LBF1_WAIT_FOR_KEYPRESS 0x0A // Wait for a key press, store the value of the key in Vx
#define CH8_LBF2_SET_DTVX 0x15 // Set delay timer = Vx
#define CH8_LBF3_SET_STVX 0x18 // Set sound timer = Vx
#define CH8_LBF4_ADD_IVX 0x1E // Set I = I + Vx
#define CH8_LBF5_SET_I_VX_SPRITE_LOCATION 0x29 // Set I = location of sprite for digit Vx
#define CH8_LBF6_BCD_VX_TO_I 0x33 // Store BCD representation of Vx in memory locations I, I+1, and I+2
#define CH8_LBF7_REGS_TO_MEM_AT_I 0x55 // Store registers V0 through Vx in memory starting at location I
#define CH8_LBF8_MEM_TO_REGS_AT_I 0x65 // Read registers V0 through Vx from memory starting at location I

#define CH8_FONT_BYTES_PER_DIGIT 5
void CH8_get_font_bytes(uint8_t* dest);