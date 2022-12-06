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
    {3, "LD   BC, $"},    // LD BC, nn
    {1, "LD   (BC), A"},
    {1, "INC  BC"},
    {1, "INC  B"},
    {1, "DEC  B"},
    {2, "LD   B, $"},     // LD B, n
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
    {3, "LD   DE, $"},    // LD DE, nn
    {1, "LD   (DE), A"},
    {1, "INC  DE"},
    {1, "INC  D"},
    {1, "DEC  D"},
    {2, "LD   D, "},
    {1, "RLA"},
    {2, "JR, $"},         // JR, d
    {1, "ADD  HL, DE"},
    {1, "LD   A, (DE)"},
    {1, "DEC  DE"},
    {1, "INC  E"}, 
    {1, "DEC  E"},
    {2, "LD   E, $"},     // LD E, n
    {1, "RRA"}, 
    {2, "JR   NZ, $"},    // JR NZ, d     // 0x20
    {3, "LD   HL, $"},    // LD, HL, nn
    {3, "LD   ($"},       // LD (nn), HL
    {1, "INC  HL"}, 
    {1, "INC  H"}, 
    {1, "DEC  H"},
    {2, "LD   H, n"},
    {1, "DAA"}, 
    {2, "JR   Z, $"},     // JR Z, d
    {1, "ADD  HL, HL"}, 
    {3, "LD   HL, ($"},   // LD HL, (nn)
    {1, "DEC  HL"}, 
    {1, "INC  L"}, 
    {1, "DEC  L"},
    {2, "LD   D, $"},     // LD D, n
    {1, "CPL"},
    {2, "JR   NC, $"},    // JR NC, d     // 0x30
    {3, "LD   SP, $"},    // LD SP, nn
    {3, "LD   ($"},       // LD (nn), A
    {1, "INC  SP"}, 
    {1, "INC  (HL)"},
    {1, "DEC  (HL)"},
    {2, "LD   (HL), $"},  // LD (HL), n
    {1, "SCF"}, 
    {2, "JR    C, $"},    // JR C, d
    {1, "ADD   HL, SP"}, 
    {3, "LD    A, ($"},   // LD A, (nn)
    {1, "DEC   SP"}, 
    {1, "INC   A"}, 
    {1, "DEC   A"},
    {2, "LD    A, $"},    // LD A, n
    {1, "CCF"}, 
    {1, "LD    B, B"},    // 0x40
    {1, "LD    B, C"},
    
};

#endif