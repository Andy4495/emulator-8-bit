/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   v0.1.0    02/11/23  Andy4495 Read for first "release"
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
*/

#include "Z80.h"
#include "Z80_opcodes.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
using std::cout;

enum index_reg {IX, IY};
index_reg ix_or_iy;
const Z80_opcodes* index_opcode;

void Z80::fetch_and_decode() {
    // Only increment lower 7 bits of R; upper bit is controlled
    // with LD R, A instruction
    R = (R & 0x80) | ((R + 1) & 0x7F);

    PC_of_Fetch = PC;  // Save the current PC for printing later
    IR[0] = memory[PC++];
    // Figure out the opcode type (main or extended)
    // and decode from the appropriate opcode array
    switch (IR[0]) {
        case 0xcb:     // Bit Instructions
            // Extended opcodes update R a second time
            // Only increment lower 7 bits of R; upper bit is controlled
            // with LD R, A instruction
            R = (R & 0x80) | ((R + 1) & 0x7F);
            decode_bit_instruction();
            break;
        case 0xdd:     // IX Register Instructions
            // Extended opcodes update R a second time
            // Only increment lower 7 bits of R; upper bit is controlled
            // with LD R, A instruction
            R = (R & 0x80) | ((R + 1) & 0x7F);
            ix_or_iy = IX;
            index_opcode = opcodes_IX_instructions;
            decode_index_instruction();
            break;
        case 0xed:     // Miscellaneous Instructions
            // Extended opcodes update R a second time
            // Only increment lower 7 bits of R; upper bit is controlled
            // with LD R, A instruction
            R = (R & 0x80) | ((R + 1) & 0x7F);
            decode_misc_instruction();
            break;
        case 0xfd:    // IY Register Instructions
            // Extended opcodes update R a second time
            // Only increment lower 7 bits of R; upper bit is controlled
            // with LD R, A instruction
            R = (R & 0x80) | ((R + 1) & 0x7F);
            ix_or_iy = IY;
            index_opcode = opcodes_IY_instructions;
            decode_index_instruction();
            break;
        default:       // Main Instructions
            decode_main_instruction();
            break;
    }
}

void Z80::decode_main_instruction() {
    unsigned int i;
    instr_length = opcodes[IR[0]].length;
    for (i = 1; i < instr_length; i++) IR[i] = get_next_byte();
    // Create the memory string based on size of instruction:
    if (instr_length == 1)
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ",
                 IR[0]);
    if (instr_length == 2)
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ",
                IR[0], IR[1]);
    if (instr_length == 3)
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ",
                 IR[0], IR[1], IR[2]);
    if (instr_length == 4)
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x",
                 IR[0], IR[1], IR[2], IR[3]);
    // Depending on the instruction, there may be data that needs to be
    // displayed as part of the instruction string.
    // - The ".s" field in the opcodes array defines the instruction layout
    //   of opcodes and data.
    // - Relative jumps ("JN") require slightly different handling in order
    //   to generate the 16-bit address from the 8-bit relative jump value
    switch (opcodes[IR[0]].s) {
        case O:
        case OO:
            strncpy(mnemonic, opcodes[IR[0]].mn, MAX_MNEMONIC_LENGTH);
            break;
        case ON:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn,
                     IR[1]);
            break;
        case ONN:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn,
                     IR[2], IR[1]);
            break;
        case OONN:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn,
                     IR[3], IR[2]);
            break;
        case JN:
            // The following uses a clumsy way to deal with the special
            // case where the displacement value is actually signed
            // where we normally treat the data as unsigned
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn,
                     (IR[1] & 0x80) ?  ((PC - (uint8_t) ~IR[1] - 1) & 0xffff) :  ((PC + IR[1])) & 0xffff);
            break;
        case OON:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2]);
            break;

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}

void Z80::decode_bit_instruction() {
    IR[1] = get_next_byte();
    // These are all 2 byte instructions of the form OO
    instr_length = opcodes_bit_instructions[IR[1]].length;
    // Create the memory string based on size of instruction:
    snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ", IR[0], IR[1]);
    // Depending on the instruction, there may be data that needs to be
    // displayed as part of the instruction string.
    // The ".s" field in the opcodes array defines the instruction
    // layout of opcodes and data
    switch (opcodes_bit_instructions[IR[1]].s) {
        case OO:
            strncpy(mnemonic, opcodes_bit_instructions[IR[1]].mn,
                    MAX_MNEMONIC_LENGTH);
            break;

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}

void Z80::decode_misc_instruction() {
    unsigned int i;
    IR[1] = get_next_byte();
    instr_length = opcodes_misc_instructions[IR[1]].length;
    for (i = 2; i < instr_length; i++) IR[i] = get_next_byte();
    // Create the memory string based on size of instruction:
    if (instr_length == 1)
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ",
                 IR[0]);
    if (instr_length == 2)
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ",
                 IR[0], IR[1]);
    if (instr_length == 3)
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ",
                 IR[0], IR[1], IR[2]);
    if (instr_length == 4)
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x",
                 IR[0], IR[1], IR[2], IR[3]);
    // Depending on the instruction, there may be data that needs to be
    // displayed as part of the instruction string.
    // - The ".s" field in the opcodes array defines the instruction
    //   layout of opcodes and data
    switch (opcodes_misc_instructions[IR[1]].s) {
        case O:
        case OO:
            strncpy(mnemonic, opcodes_misc_instructions[IR[1]].mn,
                    MAX_MNEMONIC_LENGTH);
            break;
        case ON:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH,
                     opcodes_misc_instructions[IR[1]].mn,
                     IR[1]);
            break;
        case ONN:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH,
                     opcodes_misc_instructions[IR[1]].mn,
                     IR[2], IR[1]);
            break;
        case OONN:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH,
                     opcodes_misc_instructions[IR[1]].mn,
                     IR[3], IR[2]);
            break;
        case OON:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH,
                     opcodes_misc_instructions[IR[1]].mn, IR[2]);
            break;
        case II:  // Invalid opcode
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_misc_instructions[IR[1]].mn, IR[0], IR[1]);
            break;

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}

void Z80::decode_index_instruction() {
    unsigned int i;
    char sign;
    IR[1] = get_next_byte();
    if (IR[1] == 0xcb) {
            decode_index_bit_instruction();
    } else {
        instr_length = index_opcode[IR[1]].length;
        for (i = 2; i < instr_length; i++) IR[i] = get_next_byte();
        // Create the memory string based on size of instruction:
        if (instr_length == 1)
            snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ",
                     IR[0]);
        if (instr_length == 2)
            snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ",
                     IR[0], IR[1]);
        if (instr_length == 3)
            snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ",
                     IR[0], IR[1], IR[2]);
        if (instr_length == 4)
            snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x",
                     IR[0], IR[1], IR[2], IR[3]);
        // Depending on the instruction, there may be data that needs to be
        // displayed as part of the instruction string.
        // - The ".s" field in the opcodes array defines the instruction
        //   layout of opcodes and data
        switch (index_opcode[IR[1]].s) {
            case O:
            case OO:
                strncpy(mnemonic, index_opcode[IR[1]].mn, MAX_MNEMONIC_LENGTH);
                break;
            case ON:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, index_opcode[IR[1]].mn,
                         IR[1]);
                break;
            case ONN:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, index_opcode[IR[1]].mn,
                         IR[2], IR[1]);
                break;
            case OONN:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, index_opcode[IR[1]].mn,
                         IR[3], IR[2]);
                break;
           case OON:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, index_opcode[IR[1]].mn,
                         IR[2]);
                break;
            case OOd:
                if (IR[2] & 0x80) {
                    sign = '-';
                    disp_pos = false;
                    // Convert two's complement to absolute negative amount
                    IR[2] = (IR[2] ^ 0xff) + 1;
                } else {
                    sign = '+';
                    disp_pos = true;
                }
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, index_opcode[IR[1]].mn,
                         sign, IR[2]);
                break;
            case OOdN:
                if (IR[2] & 0x80) {
                    sign = '-';
                    disp_pos = false;
                     // Convert two's complement to absolute negative amount
                     IR[2] = (IR[2] ^ 0xff) + 1;
                } else {
                    sign = '+';
                    disp_pos = true;
                }
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, index_opcode[IR[1]].mn,
                         sign, IR[2], IR[3]);
                break;
            case II:  // Invalid opcode
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, index_opcode[IR[1]].mn, IR[0], IR[1]);
                break;

            default:  // Invalid instruction layout
                strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
                break;
        }
    }
}

void Z80::decode_index_bit_instruction() {
    char sign;
    if (ix_or_iy == IX)
        index_opcode = opcodes_IX_bit_instructions;
    else
        index_opcode = opcodes_IY_bit_instructions;
    // All of these are 4 byte instructions
    IR[2] = get_next_byte();
    IR[3] = get_next_byte();
    instr_length = index_opcode[IR[3]].length;
    snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x",
             IR[0], IR[1], IR[2], IR[3]);
    // Depending on the instruction, there may be data that needs to be
    // displayed as part of the instruction string.
    // The ".s" field in the opcodes array defines the instruction layout of
    // opcodes and data
    // All of these are of the form OONO
    switch (index_opcode[IR[3]].s) {
        case OOdO:
            if (IR[2] & 0x80) {
                sign = '-';
                disp_pos = false;
                // Convert two's complement to absolute negative amount
                IR[2] = (IR[2] ^ 0xff) + 1;
            } else {
                sign = '+';
                disp_pos = true;
            }
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, index_opcode[IR[3]].mn,
                     sign, IR[2]);
            break;

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}
