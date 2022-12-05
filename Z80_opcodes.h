// Z80 Opcodes
//
// Structure to define Z80 opcodes, instruction length, and mnemonic

#ifndef Z80_OPCODES_H
#define Z80_OPCODES_H

#define MAX_OPCODE 0x10

struct Z80_opcodes {      // The opcode value is implicit in the array index defined below
    unsigned int length;
    const char* mn;       // Instruction mnemonic, not including data
};

Z80_opcodes opcodes[] = {
    {1, "NOP"},           // 0x00
    {3, "LD   BC, "},
    {1, "LD   (BC), A"},
    {1, "INC  BC"},
    {1, "INC  B"},
    {1, "DEC  B"},
    {2, "LD   B, "},
    {1, "RLCA"},
    {1, "EX   AF, AF'"},
    {1, "ADD  HL, BC"},
    {1, "LD   A, (BC)"},
    {1, "DEC  BC"},
    {1, "INC  C"},
    {1, "DEC  C"},
    {2, "LD   C, "},
    {1, "RRCA"},
    {2, "DJNZ "},         // 0x10
};

#endif