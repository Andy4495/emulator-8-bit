// Z80 Opcodes
//
// Structure to define Z80 opcodes, instruction length, and mnemonic

#ifndef Z80_OPCODES_H
#define Z80_OPCODES_H

#define MAX_OPCODE 0xFF

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
    {1, O,    "LD    B, D"},
    {1, O,    "LD    B, E"},
    {1, O,    "LD    B, H"},
    {1, O,    "LD    B, L"}, 
    {1, O,    "LD    B, (HL)"},
    {1, O,    "LD    B, A"},
    {1, O,    "LD    C, B"},
    {1, O,    "LD    C, C"}, 
    {1, O,    "LD    C, D"}, 
    {1, O,    "LD    C, E"},
    {1, O,    "LD    C, H"}, 
    {1, O,    "LD    C, L"},
    {1, O,    "LD    C, (HL)"},
    {1, O,    "LD    C, A"},
    {1, O,    "LD    D, B"},            // 0x50
    {1, O,    "LD    D, C"},
    {1, O,    "LD    D, D"},
    {1, O,    "LD    D, E"}, 
    {1, O,    "LD    D, H"},
    {1, O,    "LD    D, L"},
    {1, O,    "LD    D, (HL)"},
    {1, O,    "LD    D, A"},
    {1, O,    "LD    E, B"},
    {1, O,    "LD    E, C"},
    {1, O,    "LD    E, D"},
    {1, O,    "LD    E, E"},
    {1, O,    "LD    E, H"}, 
    {1, O,    "LD    E, L"},
    {1, O,    "LD    E, (HL)"},
    {1, O,    "LD    E, A"},
    {1, O,    "LD    H, B"},            // 0x60
    {1, O,    "LD    H, C"}, 
    {1, O,    "LD    H, D"},
    {1, O,    "LD    H, E"}, 
    {1, O,    "LD    H, H"},
    {1, O,    "LD    H, L"}, 
    {1, O,    "LD    H, (HL)"},
    {1, O,    "LD    H, A"}, 
    {1, O,    "LD    L, B"}, 
    {1, O,    "LD    L, C"}, 
    {1, O,    "LD    L, D"},
    {1, O,    "LD    L, E"}, 
    {1, O,    "LD    L, H"},
    {1, O,    "LD    L, L"},
    {1, O,    "LD    L, (HL)"},
    {1, O,    "LD    L, A"},
    {1, O,    "LD    (HL), B"},         // 0x70
    {1, O,    "LD    (HL), C"},
    {1, O,    "LD    (HL), D"}, 
    {1, O,    "LD    (HL), E"}, 
    {1, O,    "LD    (HL), H"}, 
    {1, O,    "LD    (HL), L"},
    {1, O,    "HALT"}, 
    {1, O,    "LD    (HL), A"},
    {1, O,    "LD    A, B"}, 
    {1, O,    "LD    A, C"}, 
    {1, O,    "LD    A, D"},
    {1, O,    "LD    A, E"}, 
    {1, O,    "LD    A, H"}, 
    {1, O,    "LD    A, L"},
    {1, O,    "LD    A, (HL)"}, 
    {1, O,    "LD    A, A"},
    {1, O,    "ADD   A, B"},            // 0x80
    {1, O,    "ADD   A, C"}, 
    {1, O,    "ADD   A, D"}, 
    {1, O,    "ADD   A, E"}, 
    {1, O,    "ADD   A, H"}, 
    {1, O,    "ADD   A, L"}, 
    {1, O,    "ADD   A, (HL)"},
    {1, O,    "ADD   A, A"}, 
    {1, O,    "ADC   A, B"}, 
    {1, O,    "ADC   A, C"},
    {1, O,    "ADC   A, D"}, 
    {1, O,    "ADC   A, E"}, 
    {1, O,    "ADC   A, H"}, 
    {1, O,    "ADC   A, L"}, 
    {1, O,    "ADC   A, (HL)"}, 
    {1, O,    "ADC   A, A"}, 
    {1, O,    "SUB   B"},               // 0x90
    {1, O,    "SUB   C"}, 
    {1, O,    "SUB   D"}, 
    {1, O,    "SUB   E"}, 
    {1, O,    "SUB   H"},
    {1, O,    "SUB   L"}, 
    {1, O,    "SUB   (HL)"}, 
    {1, O,    "SUB   A"}, 
    {1, O,    "SBC   A, B"}, 
    {1, O,    "SBC   A, C"}, 
    {1, O,    "SBC   A, D"},
    {1, O,    "SBC   A, E"}, 
    {1, O,    "SBC   A, H"}, 
    {1, O,    "SBC   A, L"}, 
    {1, O,    "SBC   A, (HL)"},
    {1, O,    "SBC   A, A"}, 
    {1, O,    "AND   B"},               // 0xA0
    {1, O,    "AND   C"}, 
    {1, O,    "AND   D"}, 
    {1, O,    "AND   E"}, 
    {1, O,    "AND   H"},
    {1, O,    "AND   L"}, 
    {1, O,    "AND   (HL)"}, 
    {1, O,    "AND   A"}, 
    {1, O,    "XOR   B"}, 
    {1, O,    "XOR   C"}, 
    {1, O,    "XOR   D"},
    {1, O,    "XOR   E"}, 
    {1, O,    "XOR   H"}, 
    {1, O,    "XOR   L"}, 
    {1, O,    "XOR   (HL)"},
    {1, O,    "XOR   A"}, 
    {1, O,    "OR    B"},               // 0xB0
    {1, O,    "OR    C"}, 
    {1, O,    "OR    D"}, 
    {1, O,    "OR    E"}, 
    {1, O,    "OR    H"},
    {1, O,    "OR    L"}, 
    {1, O,    "OR    (HL)"}, 
    {1, O,    "OR    A"}, 
    {1, O,    "CP    B"}, 
    {1, O,    "CP    C"}, 
    {1, O,    "CP    D"},
    {1, O,    "CP    E"}, 
    {1, O,    "CP    H"}, 
    {1, O,    "CP    L"}, 
    {1, O,    "CP    (HL)"},
    {1, O,    "CP    A"}, 
    {1, O,    "RET   NZ"},              // 0xC0
    {1, O,    "POP   BC"}, 
    {3, ONN,  "JP    NZ, $%02x%02x"}, 
    {3, ONN,  "JP    $%02x%02x"}, 
    {3, ONN,  "CALL  NZ, $%02x%02x"}, 
    {1, O,    "PUSH  BC"}, 
    {2, ON,   "ADD   A, $%02x"}, 
    {1, O,    "RST   $00"}, 
    {1, O,    "RET   Z"},
    {1, O,    "RET"}, 
    {3, ONN,  "JP    Z, $%02x%02x"},
    {2, OO,   "BIT INSTR"},             // 0xcb: Bit Intructions; additional decoding required
    {3, ONN,  "CALL  Z, $%02x%02x"}, 
    {3, ONN,  "CALL  $%02x%02x"}, 
    {2, ON,   "ADC   A, $%02x"},
    {1, O,    "RST   $08"},
    {1, O,    "RET   NC"},              // 0xD0
    {1, O,    "POP   DE"}, 
    {3, ONN,  "JP    NC, $%02x%02x"}, 
    {2, ON,   "OUT   ($%02x), A"}, 
    {3, ONN,  "CALL  NC, $%02x%02x"}, 
    {1, O,    "PUSH  DE"}, 
    {2, ON,   "SUB   %02x"}, 
    {1, O,    "RST   $10"}, 
    {1, O,    "RET   C"},
    {1, O,    "EXX"}, 
    {3, ONN,  "JP    C, $%02x%02x"},
    {2, ON,   "IN    A, ($%02x"},
    {3, ONN,  "CALL  C, $%02x%02x"}, 
    {2, OO,   "IX INSTRUCTIONS"},       // 0xDD: IX Instructions; additional decoding required 
    {2, ON,   "SBC   A, $%02x"},
    {1, O,    "RST   $18"},
    {1, O,    "RET   PO"},              // 0xE0
    {1, O,    "POP   HL"}, 
    {3, ONN,  "JP    PO, $%02x%02x"}, 
    {1, O,    "EX    (SP), HL"}, 
    {3, ONN,  "CALL  PO, $%02x%02x"}, 
    {1, O,    "PUSH  HL"}, 
    {2, ON,   "AND   %02x"}, 
    {1, O,    "RST   $20"}, 
    {1, O,    "RET   PE"},
    {1, O,    "JP    (HL)"}, 
    {3, ONN,  "JP    PE, $%02x%02x"},
    {1, O,    "EX    DE, HL"},
    {3, ONN,  "CALL  PE, $%02x%02x"}, 
    {2, OO,   "MISC INSTRUCTIONS"},     // 0xED: Misc. Instructions; additional decoding required 
    {2, ON,   "XOR   $%02x"},
    {1, O,    "RST   $28"},
    {1, O,    "RET   P"},               // 0xF0
    {1, O,    "POP   AF"}, 
    {3, ONN,  "JP    P, $%02x%02x"}, 
    {1, O,    "DI"}, 
    {3, ONN,  "CALL  P, $%02x%02x"}, 
    {1, O,    "PUSH  AF"}, 
    {2, ON,   "OR    %02x"}, 
    {1, O,    "RST   $30"}, 
    {1, O,    "RET   M"},
    {1, O,    "LD    SP, HL"}, 
    {3, ONN,  "JP    M, $%02x%02x"},
    {1, O,    "EI"},
    {3, ONN,  "CALL  M, $%02x%02x"}, 
    {2, OO,   "IY INSTRUCTIONS"},     // 0xFD: IY Instructions; additional decoding required 
    {2, ON,   "CP    $%02x"},
    {1, O,    "RST   $38"}
};

#endif