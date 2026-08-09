#pragma once

#define CHIP_START_OFFSET 0x200

#define NIBBLE_MISC_0 0x0
#define LB00_CLEAR_DISPLAY 0xE0 // Clear the display
#define LB01_RETURN 0xEE // Return from a subroutine
#define OP_JUMP 0x1 // Jump to location nnn
#define OP_CALL 0x2 // Call subroutine at nnn
#define OP_SKIP_IF_VX_EQ 0x3 // Skip next instruction if Vx = kk
#define OP_SKIP_IF_VX_NOT 0x4 // Skip next instruction if Vx != kk
#define OP_SKIP_IF_VXVY_EQ 0x5 // Skip next instruction if Vx = Vy
#define OP_SET_VX 0x6 // Set Vx = kk
#define OP_ADD_TO_VX 0x7 // Set Vx = Vx + kk
#define NIBBLE_MISC_8 0x8
#define LN80_SET_VX_VY 0x0 // Set Vx = Vy
#define LN81_SET_VX_VXORVY 0x1 // Set Vx = Vx OR Vy
#define LN82_SET_VX_VXANDVY 0x2 // Set Vx = Vx AND Vy
#define LN83_SET_VX_VXXORVY 0x3 // Set Vx = Vx XOR Vy
#define LN84_ADD_VY_TO_VX 0x4 // Set Vx = Vx + Vy, set VF = carry
#define LN85_SUB_VY_FROM_VX 0x5 // Set Vx = Vx - Vy, set VF = NOT borrow
#define LN86_SHR_VX 0x6 // Set Vx = Vx SHR 1
#define LN87_SUB_VX_FROM_VY_TO_VX 0x7 // Set Vx = Vy - Vx, set VF = NOT borrow
#define LN88_SHL_VX 0xE // Set Vx = Vx SHL 1
#define OP_SKIP_IF_VXVY_NOTEQ 0x9 // Skip next instruction if Vx != Vy
#define OP_SET_I 0xA // Set I = nnn
#define OP_JUMP_PLUS_V0 0xB // Jump to location nnn + V0
#define OP_SET_VX_RANDOM_AND_KK 0xC // Set Vx = random byte AND kk
#define OP_DRAW 0xD // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
#define NIBBLE_MISC_E 0xE
#define LNE0_SKIP_IF_VX_KEY_DOWN 0xE // Skip next instruction if key with the value of Vx is pressed
#define LNE1_SKIP_IF_VX_KEY_UP 0x1 // Skip next instruction if key with the value of Vx is not pressed
#define NIBBLE_MISC_F 0xF
#define LBF0_SET_VX_DT 0x07 // Set Vx = delay timer value
#define LBF1_WAIT_FOR_KEYPRESS 0x0A // Wait for a key press, store the value of the key in Vx
#define LBF2_SET_DT_VX 0x15 // Set delay timer = Vx
#define LBF3_SET_ST_VX 0x18 // Set sound timer = Vx
#define LBF4_ADD_VX_TO_I 0x1E // Set I = I + Vx
#define LBF5_SET_I_VX_SPRITE_LOCATION 0x29 // Set I = location of sprite for digit Vx
#define LBF6_BCD_VX_TO_I 0x33 // Store BCD representation of Vx in memory locations I, I+1, and I+2
#define LBF7_REGS_TO_MEM_AT_I 0x55 // Store registers V0 through Vx in memory starting at location I
#define LBF8_MEM_TO_REGS_AT_I 0x65 // Read registers V0 through Vx from memory starting at location I