// Z80 Opcodes
//
// Structure to define Z80 opcodes, instruction length, and mnemonic

#ifndef Z80_OPCODES_H
#define Z80_OPCODES_H

#define MAX_OPCODE 0x40

// Specify the instruction byte structure
// O -> Opcode
// J -> Relative jump opcode
// N -> Data byte
enum OPCODE_STRUCTURE {O, OO, ON, ONN, JN, OON, OONO, OONN};

struct Z80_opcodes {      // The opcode value is implicit in the array index defined below
    unsigned int length;
    OPCODE_STRUCTURE s;
    const char* mn;       // Instruction mnemonic, not including data
};

Z80_opcodes opcodes[] = {
    {1, O,    "NOP"},                   // 0x00
    {3, ONN,  "LD   BC, $%02x%02x"},    // LD BC, nn
    {1, O,    "LD   (BC), A"},
    {1, O,    "INC  BC"},
    {1, O,    "INC  B"},
    {1, O,    "DEC  B"},
    {2, ON,   "LD   B, $%02x"},         // LD B, n
    {1, O,    "RLCA"},
    {1, O,    "EX   AF, AF'"},
    {1, O,    "ADD  HL, BC"},
    {1, O,    "LD   A, (BC)"},
    {1, O,    "DEC  BC"},
    {1, O,    "INC  C"},
    {1, O,    "DEC  C"},
    {2, O,    "LD   C, "},
    {1, O,    "RRCA"},
    {2, JN,   "DJNZ $%04x"},            // DJNZ n       // 0x10
    {3, ONN,  "LD   DE, $%02x%02x"},    // LD DE, nn
    {1, O,    "LD   (DE), A"},
    {1, O,    "INC  DE"},
    {1, O,    "INC  D"},
    {1, O,    "DEC  D"},
    {2, O,    "LD   D, "},
    {1, O,    "RLA"},
    {2, JN,   "JR   $%04x"},            // JR d
    {1, O,    "ADD  HL, DE"},
    {1, O,    "LD   A, (DE)"},
    {1, O,    "DEC  DE"},
    {1, O,    "INC  E"}, 
    {1, O,    "DEC  E"},
    {2, ON,   "LD   E, $%02x"},         // LD E, n
    {1, O,    "RRA"}, 
    {2, JN,   "JR   NZ, $%04x"},        // JR NZ, d     // 0x20
    {3, ONN,  "LD   HL, $%02x%02x"},    // LD HL, nn
    {3, ONN,  "LD   ($%02x%02x), HL"},  // LD (nn), HL
    {1, O,    "INC  HL"}, 
    {1, O,    "INC  H"}, 
    {1, O,    "DEC  H"},
    {2, ON,   "LD   H, $%02x"},         // LD H, n
    {1, O,    "DAA"}, 
    {2, JN,   "JR   Z, $%04x"},         // JR Z, d
    {1, O,    "ADD  HL, HL"}, 
    {3, ONN,  "LD   HL, ($%02x%02x)"},  // LD HL, (nn)
    {1, O,    "DEC  HL"}, 
    {1, O,    "INC  L"}, 
    {1, O,    "DEC  L"},
    {2, ON,   "LD   D, $%02x"},         // LD D, n
    {1, O,    "CPL"},
    {2, JN,   "JR   NC, $%04x"},        // JR NC, d     // 0x30
    {3, ONN,  "LD   SP, $%02x%02x"},    // LD SP, nn
    {3, ONN,  "LD   ($%02x%02x), A"},   // LD (nn), A
    {1, O,    "INC  SP"}, 
    {1, O,    "INC  (HL)"},
    {1, O,    "DEC  (HL)"},
    {2, ON,   "LD   (HL), $%02x"},      // LD (HL), n
    {1, O,    "SCF"}, 
    {2, JN,   "JR    C, $%04x"},        // JR C, d
    {1, O,    "ADD   HL, SP"}, 
    {3, ONN,  "LD    A, ($%02x%02x)"},  // LD A, (nn)
    {1, O,    "DEC   SP"}, 
    {1, O,    "INC   A"}, 
    {1, O,    "DEC   A"},
    {2, ON,   "LD    A, $%02x"},        // LD A, n
    {1, O,    "CCF"}, 
    {1, O,    "LD    B, B"},            // 0x40
    {1, O,    "LD    B, C"},

};

#endif