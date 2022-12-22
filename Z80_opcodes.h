/* Z80 Emulator 
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
*/

// Z80 Opcodes
//
// Structure to define Z80 opcodes, instruction length, and mnemonic

#ifndef Z80_OPCODES_H
#define Z80_OPCODES_H

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
    {1, O,    "NOP"},                  // 0x00
    {3, ONN,  "LD   BC, $%02x%02x"},   // LD BC, nn
    {1, O,    "LD   (BC), A"},
    {1, O,    "INC  BC"},
    {1, O,    "INC  B"},
    {1, O,    "DEC  B"},
    {2, ON,   "LD   B, $%02x"},        // LD B, n
    {1, O,    "RLCA"},
    {1, O,    "EX   AF, AF'"},
    {1, O,    "ADD  HL, BC"},
    {1, O,    "LD   A, (BC)"},
    {1, O,    "DEC  BC"},
    {1, O,    "INC  C"},
    {1, O,    "DEC  C"},
    {2, O,    "LD   C, "},
    {1, O,    "RRCA"},
    {2, JN,   "DJNZ $%04x"},           // DJNZ n       // 0x10
    {3, ONN,  "LD   DE, $%02x%02x"},   // LD DE, nn
    {1, O,    "LD   (DE), A"},
    {1, O,    "INC  DE"},
    {1, O,    "INC  D"},
    {1, O,    "DEC  D"},
    {2, O,    "LD   D, "},
    {1, O,    "RLA"},
    {2, JN,   "JR   $%04x"},           // JR d
    {1, O,    "ADD  HL, DE"},
    {1, O,    "LD   A, (DE)"},
    {1, O,    "DEC  DE"},
    {1, O,    "INC  E"}, 
    {1, O,    "DEC  E"},
    {2, ON,   "LD   E, $%02x"},        // LD E, n
    {1, O,    "RRA"}, 
    {2, JN,   "JR   NZ, $%04x"},       // JR NZ, d     // 0x20
    {3, ONN,  "LD   HL, $%02x%02x"},   // LD HL, nn
    {3, ONN,  "LD   ($%02x%02x), HL"}, // LD (nn), HL
    {1, O,    "INC  HL"}, 
    {1, O,    "INC  H"}, 
    {1, O,    "DEC  H"},
    {2, ON,   "LD   H, $%02x"},        // LD H, n
    {1, O,    "DAA"}, 
    {2, JN,   "JR   Z, $%04x"},        // JR Z, d
    {1, O,    "ADD  HL, HL"}, 
    {3, ONN,  "LD   HL, ($%02x%02x)"}, // LD HL, (nn)
    {1, O,    "DEC  HL"}, 
    {1, O,    "INC  L"}, 
    {1, O,    "DEC  L"},
    {2, ON,   "LD   D, $%02x"},        // LD D, n
    {1, O,    "CPL"},
    {2, JN,   "JR   NC, $%04x"},       // JR NC, d     // 0x30
    {3, ONN,  "LD   SP, $%02x%02x"},   // LD SP, nn
    {3, ONN,  "LD   ($%02x%02x), A"},  // LD (nn), A
    {1, O,    "INC  SP"}, 
    {1, O,    "INC  (HL)"},
    {1, O,    "DEC  (HL)"},
    {2, ON,   "LD   (HL), $%02x"},     // LD (HL), n
    {1, O,    "SCF"}, 
    {2, JN,   "JR   C, $%04x"},        // JR C, d
    {1, O,    "ADD  HL, SP"}, 
    {3, ONN,  "LD   A, ($%02x%02x)"},  // LD A, (nn)
    {1, O,    "DEC  SP"}, 
    {1, O,    "INC  A"}, 
    {1, O,    "DEC  A"},
    {2, ON,   "LD   A, $%02x"},        // LD A, n
    {1, O,    "CCF"}, 
    {1, O,    "LD   B, B"},            // 0x40
    {1, O,    "LD   B, C"},
    {1, O,    "LD   B, D"},
    {1, O,    "LD   B, E"},
    {1, O,    "LD   B, H"},
    {1, O,    "LD   B, L"}, 
    {1, O,    "LD   B, (HL)"},
    {1, O,    "LD   B, A"},
    {1, O,    "LD   C, B"},
    {1, O,    "LD   C, C"}, 
    {1, O,    "LD   C, D"}, 
    {1, O,    "LD   C, E"},
    {1, O,    "LD   C, H"}, 
    {1, O,    "LD   C, L"},
    {1, O,    "LD   C, (HL)"},
    {1, O,    "LD   C, A"},
    {1, O,    "LD   D, B"},            // 0x50
    {1, O,    "LD   D, C"},
    {1, O,    "LD   D, D"},
    {1, O,    "LD   D, E"}, 
    {1, O,    "LD   D, H"},
    {1, O,    "LD   D, L"},
    {1, O,    "LD   D, (HL)"},
    {1, O,    "LD   D, A"},
    {1, O,    "LD   E, B"},
    {1, O,    "LD   E, C"},
    {1, O,    "LD   E, D"},
    {1, O,    "LD   E, E"},
    {1, O,    "LD   E, H"}, 
    {1, O,    "LD   E, L"},
    {1, O,    "LD   E, (HL)"},
    {1, O,    "LD   E, A"},
    {1, O,    "LD   H, B"},            // 0x60
    {1, O,    "LD   H, C"}, 
    {1, O,    "LD   H, D"},
    {1, O,    "LD   H, E"}, 
    {1, O,    "LD   H, H"},
    {1, O,    "LD   H, L"}, 
    {1, O,    "LD   H, (HL)"},
    {1, O,    "LD   H, A"}, 
    {1, O,    "LD   L, B"}, 
    {1, O,    "LD   L, C"}, 
    {1, O,    "LD   L, D"},
    {1, O,    "LD   L, E"}, 
    {1, O,    "LD   L, H"},
    {1, O,    "LD   L, L"},
    {1, O,    "LD   L, (HL)"},
    {1, O,    "LD   L, A"},
    {1, O,    "LD   (HL), B"},         // 0x70
    {1, O,    "LD   (HL), C"},
    {1, O,    "LD   (HL), D"}, 
    {1, O,    "LD   (HL), E"}, 
    {1, O,    "LD   (HL), H"}, 
    {1, O,    "LD   (HL), L"},
    {1, O,    "HALT"}, 
    {1, O,    "LD   (HL), A"},
    {1, O,    "LD   A, B"}, 
    {1, O,    "LD   A, C"}, 
    {1, O,    "LD   A, D"},
    {1, O,    "LD   A, E"}, 
    {1, O,    "LD   A, H"}, 
    {1, O,    "LD   A, L"},
    {1, O,    "LD   A, (HL)"}, 
    {1, O,    "LD   A, A"},
    {1, O,    "ADD  A, B"},            // 0x80
    {1, O,    "ADD  A, C"}, 
    {1, O,    "ADD  A, D"}, 
    {1, O,    "ADD  A, E"}, 
    {1, O,    "ADD  A, H"}, 
    {1, O,    "ADD  A, L"}, 
    {1, O,    "ADD  A, (HL)"},
    {1, O,    "ADD  A, A"}, 
    {1, O,    "ADC  A, B"}, 
    {1, O,    "ADC  A, C"},
    {1, O,    "ADC  A, D"}, 
    {1, O,    "ADC  A, E"}, 
    {1, O,    "ADC  A, H"}, 
    {1, O,    "ADC  A, L"}, 
    {1, O,    "ADC  A, (HL)"}, 
    {1, O,    "ADC  A, A"}, 
    {1, O,    "SUB  B"},               // 0x90
    {1, O,    "SUB  C"}, 
    {1, O,    "SUB  D"}, 
    {1, O,    "SUB  E"}, 
    {1, O,    "SUB  H"},
    {1, O,    "SUB  L"}, 
    {1, O,    "SUB  (HL)"}, 
    {1, O,    "SUB  A"}, 
    {1, O,    "SBC  A, B"}, 
    {1, O,    "SBC  A, C"}, 
    {1, O,    "SBC  A, D"},
    {1, O,    "SBC  A, E"}, 
    {1, O,    "SBC  A, H"}, 
    {1, O,    "SBC  A, L"}, 
    {1, O,    "SBC  A, (HL)"},
    {1, O,    "SBC  A, A"}, 
    {1, O,    "AND  B"},               // 0xA0
    {1, O,    "AND  C"}, 
    {1, O,    "AND  D"}, 
    {1, O,    "AND  E"}, 
    {1, O,    "AND  H"},
    {1, O,    "AND  L"}, 
    {1, O,    "AND  (HL)"}, 
    {1, O,    "AND  A"}, 
    {1, O,    "XOR  B"}, 
    {1, O,    "XOR  C"}, 
    {1, O,    "XOR  D"},
    {1, O,    "XOR  E"}, 
    {1, O,    "XOR  H"}, 
    {1, O,    "XOR  L"}, 
    {1, O,    "XOR  (HL)"},
    {1, O,    "XOR  A"}, 
    {1, O,    "OR   B"},               // 0xB0
    {1, O,    "OR   C"}, 
    {1, O,    "OR   D"}, 
    {1, O,    "OR   E"}, 
    {1, O,    "OR   H"},
    {1, O,    "OR   L"}, 
    {1, O,    "OR   (HL)"}, 
    {1, O,    "OR   A"}, 
    {1, O,    "CP   B"}, 
    {1, O,    "CP   C"}, 
    {1, O,    "CP   D"},
    {1, O,    "CP   E"}, 
    {1, O,    "CP   H"}, 
    {1, O,    "CP   L"}, 
    {1, O,    "CP   (HL)"},
    {1, O,    "CP   A"}, 
    {1, O,    "RET  NZ"},              // 0xC0
    {1, O,    "POP  BC"}, 
    {3, ONN,  "JP   NZ, $%02x%02x"}, 
    {3, ONN,  "JP   $%02x%02x"}, 
    {3, ONN,  "CALL NZ, $%02x%02x"}, 
    {1, O,    "PUSH BC"}, 
    {2, ON,   "ADD  A, $%02x"}, 
    {1, O,    "RST  $00"}, 
    {1, O,    "RET  Z"},
    {1, O,    "RET"}, 
    {3, ONN,  "JP   Z, $%02x%02x"},
    {2, OO,   "BIT INSTRUCTION"},      // 0xcb: Bit Intructions; additional decoding required
    {3, ONN,  "CALL Z, $%02x%02x"}, 
    {3, ONN,  "CALL $%02x%02x"}, 
    {2, ON,   "ADC  A, $%02x"},
    {1, O,    "RST  $08"},
    {1, O,    "RET  NC"},              // 0xD0
    {1, O,    "POP  DE"}, 
    {3, ONN,  "JP   NC, $%02x%02x"}, 
    {2, ON,   "OUT  ($%02x), A"}, 
    {3, ONN,  "CALL NC, $%02x%02x"}, 
    {1, O,    "PUSH DE"}, 
    {2, ON,   "SUB  %02x"}, 
    {1, O,    "RST  $10"}, 
    {1, O,    "RET  C"},
    {1, O,    "EXX"}, 
    {3, ONN,  "JP   C, $%02x%02x"},
    {2, ON,   "IN   A, ($%02x"},
    {3, ONN,  "CALL C, $%02x%02x"}, 
    {2, OO,   "IX INSTRUCTIONS"},      // 0xDD: IX Instructions; additional decoding required 
    {2, ON,   "SBC  A, $%02x"},
    {1, O,    "RST  $18"},
    {1, O,    "RET  PO"},              // 0xE0
    {1, O,    "POP  HL"}, 
    {3, ONN,  "JP   PO, $%02x%02x"}, 
    {1, O,    "EX   (SP), HL"}, 
    {3, ONN,  "CALL PO, $%02x%02x"}, 
    {1, O,    "PUSH HL"}, 
    {2, ON,   "AND  %02x"}, 
    {1, O,    "RST  $20"}, 
    {1, O,    "RET  PE"},
    {1, O,    "JP   (HL)"}, 
    {3, ONN,  "JP   PE, $%02x%02x"},
    {1, O,    "EX   DE, HL"},
    {3, ONN,  "CALL PE, $%02x%02x"}, 
    {2, OO,   "MISC INSTRUCTIONS"},    // 0xED: Misc. Instructions; additional decoding required 
    {2, ON,   "XOR  $%02x"},
    {1, O,    "RST  $28"},
    {1, O,    "RET  P"},               // 0xF0
    {1, O,    "POP  AF"}, 
    {3, ONN,  "JP   P, $%02x%02x"}, 
    {1, O,    "DI"}, 
    {3, ONN,  "CALL P, $%02x%02x"}, 
    {1, O,    "PUSH AF"}, 
    {2, ON,   "OR   %02x"}, 
    {1, O,    "RST  $30"}, 
    {1, O,    "RET  M"},
    {1, O,    "LD   SP, HL"}, 
    {3, ONN,  "JP   M, $%02x%02x"},
    {1, O,    "EI"},
    {3, ONN,  "CALL M, $%02x%02x"}, 
    {2, OO,   "IY INSTRUCTIONS"},      // 0xFD: IY Instructions; additional decoding required 
    {2, ON,   "CP   $%02x"},
    {1, O,    "RST  $38"}
};

// Bit Instructions -- opcodes prefixed with 0xCB
Z80_opcodes opcodes_bit_instructions[] = {
    {2, OO,   "RLC  B"},             // 0xCB00
    {2, OO,   "RLC  C"},
    {2, OO,   "RLC  D"},
    {2, OO,   "RLC  E"},
    {2, OO,   "RLC  H"},
    {2, OO,   "RLC  L"},
    {2, OO,   "RLC  (HL)"},
    {2, OO,   "RLC  A"},
    {2, OO,   "RRC  B"},             // 0xCB08
    {2, OO,   "RRC  C"},
    {2, OO,   "RRC  D"},
    {2, OO,   "RRC  E"},
    {2, OO,   "RRC  H"},
    {2, OO,   "RRC  L"},
    {2, OO,   "RRC  (HL)"},
    {2, OO,   "RRC  A"},
    {2, OO,   "RL   B"},             // 0xCB10
    {2, OO,   "RL   C"},
    {2, OO,   "RL   D"},
    {2, OO,   "RL   E"},
    {2, OO,   "RL   H"},
    {2, OO,   "RL   L"},
    {2, OO,   "RL   (HL)"},
    {2, OO,   "RL   A"},
    {2, OO,   "RR   B"},             // 0xCB18
    {2, OO,   "RR   C"},
    {2, OO,   "RR   D"},
    {2, OO,   "RR   E"},
    {2, OO,   "RR   H"},
    {2, OO,   "RR   L"},
    {2, OO,   "RR   (HL)"},
    {2, OO,   "RR   A"},
    {2, OO,   "SLA  B"},             // 0xCB20
    {2, OO,   "SLA  C"},
    {2, OO,   "SLA  D"},
    {2, OO,   "SLA  E"},
    {2, OO,   "SLA  H"},
    {2, OO,   "SLA  L"},
    {2, OO,   "SLA  (HL)"},
    {2, OO,   "SLA  A"},
    {2, OO,   "SRA  B"},             // 0xCB28
    {2, OO,   "SRA  C"},
    {2, OO,   "SRA  D"},
    {2, OO,   "SRA  E"},
    {2, OO,   "SRA  H"},
    {2, OO,   "SRA  L"},
    {2, OO,   "SRA  (HL)"},
    {2, OO,   "SRA  A"},
    {2, OO,   "SLL  B"},             // 0xCB30
    {2, OO,   "SLL  C"},
    {2, OO,   "SLL  D"},
    {2, OO,   "SLL  E"},
    {2, OO,   "SLL  H"},
    {2, OO,   "SLL  L"},
    {2, OO,   "SLL  (HL)"},
    {2, OO,   "SLL  A"},
    {2, OO,   "SRL  B"},             // 0xCB38
    {2, OO,   "SRL  C"},
    {2, OO,   "SRL  D"},
    {2, OO,   "SRL  E"},
    {2, OO,   "SRL  H"},
    {2, OO,   "SRL  L"},
    {2, OO,   "SRL  (HL)"},
    {2, OO,   "SRL  A"},
    {2, OO,   "BIT  0, B"},          // 0xCB40
    {2, OO,   "BIT  0, C"},
    {2, OO,   "BIT  0, D"},
    {2, OO,   "BIT  0, E"},
    {2, OO,   "BIT  0, H"},
    {2, OO,   "BIT  0, L"},
    {2, OO,   "BIT  0, (HL)"},
    {2, OO,   "BIT  0, A"},
    {2, OO,   "BIT  1, B"},          // 0xCB48
    {2, OO,   "BIT  1, C"},
    {2, OO,   "BIT  1, D"},
    {2, OO,   "BIT  1, E"},
    {2, OO,   "BIT  1, H"},
    {2, OO,   "BIT  1, L"},
    {2, OO,   "BIT  1, (HL)"},
    {2, OO,   "BIT  1, A"},
    {2, OO,   "BIT  2, B"},          // 0xCB50
    {2, OO,   "BIT  2, C"},
    {2, OO,   "BIT  2, D"},
    {2, OO,   "BIT  2, E"},
    {2, OO,   "BIT  2, H"},
    {2, OO,   "BIT  2, L"},
    {2, OO,   "BIT  2, (HL)"},
    {2, OO,   "BIT  2, A"},
    {2, OO,   "BIT  3, B"},          // 0xCB58
    {2, OO,   "BIT  3, C"},
    {2, OO,   "BIT  3, D"},
    {2, OO,   "BIT  3, E"},
    {2, OO,   "BIT  3, H"},
    {2, OO,   "BIT  3, L"},
    {2, OO,   "BIT  3, (HL)"},
    {2, OO,   "BIT  3, A"},
    {2, OO,   "BIT  4, B"},          // 0xCB60
    {2, OO,   "BIT  4, C"},
    {2, OO,   "BIT  4, D"},
    {2, OO,   "BIT  4, E"},
    {2, OO,   "BIT  4, H"},
    {2, OO,   "BIT  4, L"},
    {2, OO,   "BIT  4, (HL)"},
    {2, OO,   "BIT  4, A"},
    {2, OO,   "BIT  5, B"},          // 0xCB68
    {2, OO,   "BIT  5, C"},
    {2, OO,   "BIT  5, D"},
    {2, OO,   "BIT  5, E"},
    {2, OO,   "BIT  5, H"},
    {2, OO,   "BIT  5, L"},
    {2, OO,   "BIT  5, (HL)"},
    {2, OO,   "BIT  5, A"},
    {2, OO,   "BIT  6, B"},          // 0xCB70
    {2, OO,   "BIT  6, C"},
    {2, OO,   "BIT  6, D"},
    {2, OO,   "BIT  6, E"},
    {2, OO,   "BIT  6, H"},
    {2, OO,   "BIT  6, L"},
    {2, OO,   "BIT  6, (HL)"},
    {2, OO,   "BIT  6, A"},
    {2, OO,   "BIT  7, B"},          // 0xCB78
    {2, OO,   "BIT  7, C"},
    {2, OO,   "BIT  7, D"},
    {2, OO,   "BIT  7, E"},
    {2, OO,   "BIT  7, H"},
    {2, OO,   "BIT  7, L"},
    {2, OO,   "BIT  7, (HL)"},
    {2, OO,   "BIT  7, A"},
    {2, OO,   "RES  0, B"},          // 0xCB80
    {2, OO,   "RES  0, C"},
    {2, OO,   "RES  0, D"},
    {2, OO,   "RES  0, E"},
    {2, OO,   "RES  0, H"},
    {2, OO,   "RES  0, L"},
    {2, OO,   "RES  0, (HL)"},
    {2, OO,   "RES  0, A"},
    {2, OO,   "RES  1, B"},          // 0xCB88
    {2, OO,   "RES  1, C"},
    {2, OO,   "RES  1, D"},
    {2, OO,   "RES  1, E"},
    {2, OO,   "RES  1, H"},
    {2, OO,   "RES  1, L"},
    {2, OO,   "RES  1, (HL)"},
    {2, OO,   "RES  1, A"},
    {2, OO,   "RES  2, B"},          // 0xCB90
    {2, OO,   "RES  2, C"},
    {2, OO,   "RES  2, D"},
    {2, OO,   "RES  2, E"},
    {2, OO,   "RES  2, H"},
    {2, OO,   "RES  2, L"},
    {2, OO,   "RES  2, (HL)"},
    {2, OO,   "RES  2, A"},
    {2, OO,   "RES  3, B"},          // 0xCB98
    {2, OO,   "RES  3, C"},
    {2, OO,   "RES  3, D"},
    {2, OO,   "RES  3, E"},
    {2, OO,   "RES  3, H"},
    {2, OO,   "RES  3, L"},
    {2, OO,   "RES  3, (HL)"},
    {2, OO,   "RES  3, A"},
    {2, OO,   "RES  4, B"},          // 0xCBA0
    {2, OO,   "RES  4, C"},
    {2, OO,   "RES  4, D"},
    {2, OO,   "RES  4, E"},
    {2, OO,   "RES  4, H"},
    {2, OO,   "RES  4, L"},
    {2, OO,   "RES  4, (HL)"},
    {2, OO,   "RES  4, A"},
    {2, OO,   "RES  5, B"},          // 0xCBA8
    {2, OO,   "RES  5, C"},
    {2, OO,   "RES  5, D"},
    {2, OO,   "RES  5, E"},
    {2, OO,   "RES  5, H"},
    {2, OO,   "RES  5, L"},
    {2, OO,   "RES  5, (HL)"},
    {2, OO,   "RES  5, A"},
    {2, OO,   "RES  6, B"},          // 0xCBB0
    {2, OO,   "RES  6, C"},
    {2, OO,   "RES  6, D"},
    {2, OO,   "RES  6, E"},
    {2, OO,   "RES  6, H"},
    {2, OO,   "RES  6, L"},
    {2, OO,   "RES  6, (HL)"},
    {2, OO,   "RES  6, A"},
    {2, OO,   "RES  7, B"},          // 0xCBB8
    {2, OO,   "RES  7, C"},
    {2, OO,   "RES  7, D"},
    {2, OO,   "RES  7, E"},
    {2, OO,   "RES  7, H"},
    {2, OO,   "RES  7, L"},
    {2, OO,   "RES  7, (HL)"},
    {2, OO,   "RES  7, A"},
    {2, OO,   "SET  0, B"},          // 0xCBC0
    {2, OO,   "SET  0, C"},
    {2, OO,   "SET  0, D"},
    {2, OO,   "SET  0, E"},
    {2, OO,   "SET  0, H"},
    {2, OO,   "SET  0, L"},
    {2, OO,   "SET  0, (HL)"},
    {2, OO,   "SET  0, A"},
    {2, OO,   "SET  1, B"},          // 0xCBC8
    {2, OO,   "SET  1, C"},
    {2, OO,   "SET  1, D"},
    {2, OO,   "SET  1, E"},
    {2, OO,   "SET  1, H"},
    {2, OO,   "SET  1, L"},
    {2, OO,   "SET  1, (HL)"},
    {2, OO,   "SET  1, A"},
    {2, OO,   "SET  2, B"},          // 0xCBD0
    {2, OO,   "SET  2, C"},
    {2, OO,   "SET  2, D"},
    {2, OO,   "SET  2, E"},
    {2, OO,   "SET  2, H"},
    {2, OO,   "SET  2, L"},
    {2, OO,   "SET  2, (HL)"},
    {2, OO,   "SET  2, A"},
    {2, OO,   "SET  3, B"},          // 0xCBD8
    {2, OO,   "SET  3, C"},
    {2, OO,   "SET  3, D"},
    {2, OO,   "SET  3, E"},
    {2, OO,   "SET  3, H"},
    {2, OO,   "SET  3, L"},
    {2, OO,   "SET  3, (HL)"},
    {2, OO,   "SET  3, A"},
    {2, OO,   "SET  4, B"},          // 0xCBE0
    {2, OO,   "SET  4, C"},
    {2, OO,   "SET  4, D"},
    {2, OO,   "SET  4, E"},
    {2, OO,   "SET  4, H"},
    {2, OO,   "SET  4, L"},
    {2, OO,   "SET  4, (HL)"},
    {2, OO,   "SET  4, A"},
    {2, OO,   "SET  5, B"},          // 0xCBE8
    {2, OO,   "SET  5, C"},
    {2, OO,   "SET  5, D"},
    {2, OO,   "SET  5, E"},
    {2, OO,   "SET  5, H"},
    {2, OO,   "SET  5, L"},
    {2, OO,   "SET  5, (HL)"},
    {2, OO,   "SET  5, A"},
    {2, OO,   "SET  6, B"},          // 0xCBF0
    {2, OO,   "SET  6, C"},
    {2, OO,   "SET  6, D"},
    {2, OO,   "SET  6, E"},
    {2, OO,   "SET  6, H"},
    {2, OO,   "SET  6, L"},
    {2, OO,   "SET  6, (HL)"},
    {2, OO,   "SET  6, A"},
    {2, OO,   "SET  7, B"},          // 0xCBF8
    {2, OO,   "SET  7, C"},
    {2, OO,   "SET  7, D"},
    {2, OO,   "SET  7, E"},
    {2, OO,   "SET  7, H"},
    {2, OO,   "SET  7, L"},
    {2, OO,   "SET  7, (HL)"},
    {2, OO,   "SET  7, A"},
};

// IX Register Instructions -- opcodes prefixed with 0xDD
Z80_opcodes opcodes_IX_instructions[] = {
    {2, OO,   "INVALID: NOP" },       // 0xDD00 - Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INC  B"},
};

// Miscellaneous Instructions -- opcodes prefixed with 0xED
Z80_opcodes opcodes_misc_instructions[] = {
    // 0x40 insttructions not implemented (opcodes 0xED00 - 0xED3F) -- execuate NOP instead
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode  0xED00
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode  0xED10
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode  0xED20
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode  0xED30
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "IN   B, (C)"},        // 0x40
};

// IY Register Instructions -- opcodes prefixed with 0xFD
Z80_opcodes opcodes_IY_instructions[] = {
    {2, OO,   "INVALID: NOP" },       // 0xDD00 - Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INVALID: NOP" },       // Unimplemented opcode
    {2, OO,   "INC  B"},
};

// IX Register Bit Instructions -- opcodes prefixed with 0xDDCB
Z80_opcodes opcodes_IX_bit_instructions[] = {
    {4, OONO, "RLC  (IX + $%02x), B"},
};

// IY Register Bit Instructions -- opcodes prefixed with 0xFDCB
Z80_opcodes opcodes_IY_bit_instructions[] = {
    {4, OONO, "RLC  (IY + $%02x), B"},
};

#endif