/* Homemade CPU Emulator 
   Copyright 2025 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   See https://github.com/Andy4495/Homemade-CPU for the CPU this emulates

   This code is based off version 2 of the CPU design.

   0.1  15-Oct-2025  Andy4495  Initial Creation


*/

// Homemade CPU Opcodes
//
// Structure to define Homemade CPU opcodes and mnemonics
// All instructions are of length 2

#ifndef SRC_HOMEMADE_CPU2_OPCODES_H_
#define SRC_HOMEMADE_CPU2_OPCODES_H_

// Specify the instruction byte structure
// O -> Opcode nibble
// N -> Data nibble
enum CPU_OPCODE_STRUCTURE {OO, ON, NN};

struct Homemade_CPU_opcodes {      // The opcode value is implicit in the array index
    CPU_OPCODE_STRUCTURE s;
    const char* mn;       // Instruction mnemonic, not including data
};

// The actual opcode value is implicitly represented by the array index
const Homemade_CPU_opcodes opcodes[] = {
    {OO, "NOOP"},                                                  // 0x00
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x01
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x02
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x03
    {OO, "BCFL 4               ; or BCFL V"},                      // 0x04
    {OO, "BCFL 5               ; or BCFL C"},                      // 0x05
    {OO, "BCFL 6               ; or BCFL Z"},                      // 0x06
    {OO, "BCFL 7               ; or BCFL S"},                      // 0x07
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x08
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x09
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x0A
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x0B
    {OO, "BSFL 4               ; or BSFL V"},                      // 0x0C
    {OO, "BSFL 5               ; or BSFL C"},                      // 0x0D
    {OO, "BSFL 6               ; or BSFL Z"},                      // 0x0E
    {OO, "BSFL 7               ; or BSFL S"},                      // 0x0F
    {OO, "BCAA 0"},                                                // 0x10
    {OO, "BCAA 1"},                                                // 0x11
    {OO, "BCAA 2"},                                                // 0x12
    {OO, "BCAA 3"},                                                // 0x13
    {OO, "BCAA 4"},                                                // 0x14
    {OO, "BCAA 5"},                                                // 0x15
    {OO, "BCAA 6"},                                                // 0x16
    {OO, "BCAA 7"},                                                // 0x17
    {OO, "BSAA 0"},                                                // 0x18
    {OO, "BSAA 1"},                                                // 0x19
    {OO, "BSAA 2"},                                                // 0x1A
    {OO, "BSAA 3"},                                                // 0x1B
    {OO, "BSAA 4"},                                                // 0x1C
    {OO, "BSAA 5"},                                                // 0x1D
    {OO, "BSAA 6"},                                                // 0x1E
    {OO, "BSAA 7"},                                                // 0x1F
    {OO, "LOAD (MR)"},                                             // 0x20
    {OO, "STOR (MR)"},                                             // 0x21
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x22
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x23
    {OO, "JUMP"},                                                  // 0x24
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x25
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x26
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x27
    {OO, "JPVC"},                                                  // 0x28
    {OO, "JPVS"},                                                  // 0x29
    {OO, "JPCC"},                                                  // 0x2A
    {OO, "JPCS"},                                                  // 0x2B
    {OO, "JPZC"},                                                  // 0x2C
    {OO, "JPZS"},                                                  // 0x2D
    {OO, "JPSC"},                                                  // 0x2E
    {OO, "JPSS"},                                                  // 0x2F
    {OO, "PUSH AA"},                                               // 0x30
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x31
    {OO, "PUSH FL"},                                               // 0x32
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x33
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x34
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x35
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x36
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x37
    {OO, "POPP AA"},                                               // 0x38
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x39
    {OO, "POPP FL"},                                               // 0x3A
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x3B
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x3C
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x3D
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x3E
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x3F
    {OO, "EXCH"},                                                  // 0x40
    {OO, "MOVE AB->AA"},                                           // 0x41
    {OO, "MOVE SL->AA"},                                           // 0x42
    {OO, "MOVE SH->AA"},                                           // 0x43
    {OO, "MOVE ML->AA"},                                           // 0x44
    {OO, "MOVE MH->AA"},                                           // 0x45
    {OO, "MOVE JL->AA"},                                           // 0x46
    {OO, "MOVE JH->AA"},                                           // 0x47
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x48
    {OO, "MOVE AA->AB"},                                           // 0x49
    {OO, "MOVE AA->SL"},                                           // 0x4A
    {OO, "MOVE AA->SH"},                                           // 0x4B
    {OO, "MOVE AA->ML"},                                           // 0x4C
    {OO, "MOVE AA->MH"},                                           // 0x4D
    {OO, "MOVE AA->JL"},                                           // 0x4E
    {OO, "MOVE AA->JH"},                                           // 0x4F
    {OO, "COMP"},                                                  // 0x50
    {OO, "SUBB"},                                                  // 0x51
    {OO, "ADDD"},                                                  // 0x52
    {OO, "ANDD"},                                                  // 0x53
    {OO, "ORRR"},                                                  // 0x54
    {OO, "XORR"},                                                  // 0x55
    {OO, "NAND"},                                                  // 0x56
    {OO, "NORR"},                                                  // 0x57
    {OO, "NOTT"},                                                  // 0x58
    {OO, "NEGA"},                                                  // 0x59
    {OO, "SHRL"},                                                  // 0x5A
    {OO, "SHLL"},                                                  // 0x5B
    {OO, "SHRA"},                                                  // 0x5C
    {OO, "SRLC"},                                                  // 0x5D
    {OO, "SLLC"},                                                  // 0x5E
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x5F
    {OO, "INCA"},                                                  // 0x60
    {OO, "DECA"},                                                  // 0x61
    {OO, "INCB"},                                                  // 0x62
    {OO, "DECB"},                                                  // 0x63
    {OO, "INCM"},                                                  // 0x64
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x65
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x66
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x67
    {OO, "LDA0"},                                                  // 0x68
    {OO, "LDA1"},                                                  // 0x69
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x6A
    {OO, "LDAF"},                                                  // 0x6B
    {OO, "LDB0"},                                                  // 0x6C
    {OO, "LDB1"},                                                  // 0x6D
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x6E
    {OO, "LDBF"},                                                  // 0x6F
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x70
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x71
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x72
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x73
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x74
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x75
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x76
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x77
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x78
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x79
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x7A
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x7B
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x7C
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x7D
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x7E
    {OO, "HALT"},                                                  // 0x7F
    {ON, "LDAL #$0"},                                              // 0x80
    {ON, "LDAL #$1"},                                              // 0x81
    {ON, "LDAL #$2"},                                              // 0x82
    {ON, "LDAL #$3"},                                              // 0x83
    {ON, "LDAL #$4"},                                              // 0x84
    {ON, "LDAL #$5"},                                              // 0x85
    {ON, "LDAL #$6"},                                              // 0x86
    {ON, "LDAL #$7"},                                              // 0x87
    {ON, "LDAL #$8"},                                              // 0x88
    {ON, "LDAL #$9"},                                              // 0x89
    {ON, "LDAL #$A"},                                              // 0x8A
    {ON, "LDAL #$B"},                                              // 0x8B
    {ON, "LDAL #$C"},                                              // 0x8C
    {ON, "LDAL #$D"},                                              // 0x8D
    {ON, "LDAL #$E"},                                              // 0x8E
    {ON, "LDAL #$F"},                                              // 0x8F
    {ON, "LDAH #$0"},                                              // 0x90
    {ON, "LDAH #$1"},                                              // 0x91
    {ON, "LDAH #$2"},                                              // 0x92
    {ON, "LDAH #$3"},                                              // 0x93
    {ON, "LDAH #$4"},                                              // 0x94
    {ON, "LDAH #$5"},                                              // 0x95
    {ON, "LDAH #$6"},                                              // 0x96
    {ON, "LDAH #$7"},                                              // 0x97
    {ON, "LDAH #$8"},                                              // 0x98
    {ON, "LDAH #$9"},                                              // 0x99
    {ON, "LDAH #$A"},                                              // 0x9A
    {ON, "LDAH #$B"},                                              // 0x9B
    {ON, "LDAH #$C"},                                              // 0x9C
    {ON, "LDAH #$D"},                                              // 0x9D
    {ON, "LDAH #$E"},                                              // 0x9E
    {ON, "LDAH #$F"},                                              // 0x9F
    {ON, "LDBL #$0"},                                              // 0xA0
    {ON, "LDBL #$1"},                                              // 0xA1
    {ON, "LDBL #$2"},                                              // 0xA2
    {ON, "LDBL #$3"},                                              // 0xA3
    {ON, "LDBL #$4"},                                              // 0xA4
    {ON, "LDBL #$5"},                                              // 0xA5
    {ON, "LDBL #$6"},                                              // 0xA6
    {ON, "LDBL #$7"},                                              // 0xA7
    {ON, "LDBL #$8"},                                              // 0xA8
    {ON, "LDBL #$9"},                                              // 0xA9
    {ON, "LDBL #$A"},                                              // 0xAA
    {ON, "LDBL #$B"},                                              // 0xAB
    {ON, "LDBL #$C"},                                              // 0xAC
    {ON, "LDBL #$D"},                                              // 0xAD
    {ON, "LDBL #$E"},                                              // 0xAE
    {ON, "LDBL #$F"},                                              // 0xAF
    {ON, "LDBH #$0"},                                              // 0xB0
    {ON, "LDBH #$1"},                                              // 0xB1
    {ON, "LDBH #$2"},                                              // 0xB2
    {ON, "LDBH #$3"},                                              // 0xB3
    {ON, "LDBH #$4"},                                              // 0xB4
    {ON, "LDBH #$5"},                                              // 0xB5
    {ON, "LDBH #$6"},                                              // 0xB6
    {ON, "LDBH #$7"},                                              // 0xB7
    {ON, "LDBH #$8"},                                              // 0xB8
    {ON, "LDBH #$9"},                                              // 0xB9
    {ON, "LDBH #$A"},                                              // 0xBA
    {ON, "LDBH #$B"},                                              // 0xBB
    {ON, "LDBH #$C"},                                              // 0xBC
    {ON, "LDBH #$D"},                                              // 0xBD
    {ON, "LDBH #$E"},                                              // 0xBE
    {ON, "LDBH #$F"},                                              // 0xBF
    {ON, "LJLL #$0"},                                              // 0xC0
    {ON, "LJLL #$1"},                                              // 0xC1
    {ON, "LJLL #$2"},                                              // 0xC2
    {ON, "LJLL #$3"},                                              // 0xC3
    {ON, "LJLL #$4"},                                              // 0xC4
    {ON, "LJLL #$5"},                                              // 0xC5
    {ON, "LJLL #$6"},                                              // 0xC6
    {ON, "LJLL #$7"},                                              // 0xC7
    {ON, "LJLL #$8"},                                              // 0xC8
    {ON, "LJLL #$9"},                                              // 0xC9
    {ON, "LJLL #$A"},                                              // 0xCA
    {ON, "LJLL #$B"},                                              // 0xCB
    {ON, "LJLL #$C"},                                              // 0xCC
    {ON, "LJLL #$D"},                                              // 0xCD
    {ON, "LJLL #$E"},                                              // 0xCE
    {ON, "LJLL #$F"},                                              // 0xCF
    {ON, "LJLH #$0"},                                              // 0xD0
    {ON, "LJLH #$1"},                                              // 0xD1
    {ON, "LJLH #$2"},                                              // 0xD2
    {ON, "LJLH #$3"},                                              // 0xD3
    {ON, "LJLH #$4"},                                              // 0xD4
    {ON, "LJLH #$5"},                                              // 0xD5
    {ON, "LJLH #$6"},                                              // 0xD6
    {ON, "LJLH #$7"},                                              // 0xD7  
    {ON, "LJLH #$8"},                                              // 0xD8
    {ON, "LJLH #$9"},                                              // 0xD9
    {ON, "LJLH #$A"},                                              // 0xDA
    {ON, "LJLH #$B"},                                              // 0xDB
    {ON, "LJLH #$C"},                                              // 0xDC
    {ON, "LJLH #$D"},                                              // 0xDD
    {ON, "LJLH #$E"},                                              // 0xDE
    {ON, "LJLH #$F"},                                              // 0xDF  
    {ON, "LJHL #$0"},                                              // 0xE0
    {ON, "LJHL #$1"},                                              // 0xE1
    {ON, "LJHL #$2"},                                              // 0xE2
    {ON, "LJHL #$3"},                                              // 0xE3
    {ON, "LJHL #$4"},                                              // 0xE4
    {ON, "LJHL #$5"},                                              // 0xE5
    {ON, "LJHL #$6"},                                              // 0xE6
    {ON, "LJHL #$7"},                                              // 0xE7
    {ON, "LJHL #$8"},                                              // 0xE8
    {ON, "LJHL #$9"},                                              // 0xE9
    {ON, "LJHL #$A"},                                              // 0xEA
    {ON, "LJHL #$B"},                                              // 0xEB
    {ON, "LJHL #$C"},                                              // 0xEC
    {ON, "LJHL #$D"},                                              // 0xED
    {ON, "LJHL #$E"},                                              // 0xEE
    {ON, "LJHL #$F"},                                              // 0xEF
    {ON, "LJHH #$0"},                                              // 0xF0
    {ON, "LJHH #$1"},                                              // 0xF1
    {ON, "LJHH #$2"},                                              // 0xF2
    {ON, "LJHH #$3"},                                              // 0xF3
    {ON, "LJHH #$4"},                                              // 0xF4
    {ON, "LJHH #$5"},                                              // 0xF5
    {ON, "LJHH #$6"},                                              // 0xF6
    {ON, "LJHH #$7"},                                              // 0xF7
    {ON, "LJHH #$8"},                                              // 0xF8
    {ON, "LJHH #$9"},                                              // 0xF9
    {ON, "LJHH #$A"},                                              // 0xFA
    {ON, "LJHH #$B"},                                              // 0xFB
    {ON, "LJHH #$C"},                                              // 0xFC
    {ON, "LJHH #$D"},                                              // 0xFD
    {ON, "LJHH #$E"},                                              // 0xFE
    {ON, "LJHH #$F"},                                              // 0xFF
};

#endif // SRC_HOMEMADE_CPU2_OPCODES_H_