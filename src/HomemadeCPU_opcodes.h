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
    {ON, "LDAL #$0"},                                              // 0x00
    {ON, "LDAL #$1"},                                              // 0x01
    {ON, "LDAL #$2"},                                              // 0x02
    {ON, "LDAL #$3"},                                              // 0x03
    {ON, "LDAL #$4"},                                              // 0x04
    {ON, "LDAL #$5"},                                              // 0x05
    {ON, "LDAL #$6"},                                              // 0x06
    {ON, "LDAL #$7"},                                              // 0x07
    {ON, "LDAL #$8"},                                              // 0x08
    {ON, "LDAL #$9"},                                              // 0x09
    {ON, "LDAL #$A"},                                              // 0x0A
    {ON, "LDAL #$B"},                                              // 0x0B
    {ON, "LDAL #$C"},                                              // 0x0C
    {ON, "LDAL #$D"},                                              // 0x0D
    {ON, "LDAL #$E"},                                              // 0x0E
    {ON, "LDAL #$F"},                                              // 0x0F
    {ON, "LDAH #$0"},                                              // 0x10
    {ON, "LDAH #$1"},                                              // 0x11
    {ON, "LDAH #$2"},                                              // 0x12
    {ON, "LDAH #$3"},                                              // 0x13
    {ON, "LDAH #$4"},                                              // 0x14
    {ON, "LDAH #$5"},                                              // 0x15
    {ON, "LDAH #$6"},                                              // 0x16
    {ON, "LDAH #$7"},                                              // 0x17
    {ON, "LDAH #$8"},                                              // 0x18
    {ON, "LDAH #$9"},                                              // 0x19
    {ON, "LDAH #$A"},                                              // 0x1A
    {ON, "LDAH #$B"},                                              // 0x1B
    {ON, "LDAH #$C"},                                              // 0x1C
    {ON, "LDAH #$D"},                                              // 0x1D
    {ON, "LDAH #$E"},                                              // 0x1E
    {ON, "LDAH #$F"},                                              // 0x1F
    {ON, "LDBL #$0"},                                              // 0x20
    {ON, "LDBL #$1"},                                              // 0x21
    {ON, "LDBL #$2"},                                              // 0x22
    {ON, "LDBL #$3"},                                              // 0x23
    {ON, "LDBL #$4"},                                              // 0x24
    {ON, "LDBL #$5"},                                              // 0x25
    {ON, "LDBL #$6"},                                              // 0x26
    {ON, "LDBL #$7"},                                              // 0x27
    {ON, "LDBL #$8"},                                              // 0x28
    {ON, "LDBL #$9"},                                              // 0x29
    {ON, "LDBL #$A"},                                              // 0x2A
    {ON, "LDBL #$B"},                                              // 0x2B
    {ON, "LDBL #$C"},                                              // 0x2C
    {ON, "LDBL #$D"},                                              // 0x2D
    {ON, "LDBL #$E"},                                              // 0x2E
    {ON, "LDBL #$F"},                                              // 0x2F
    {ON, "LDBH #$0"},                                              // 0x30
    {ON, "LDBH #$1"},                                              // 0x31
    {ON, "LDBH #$2"},                                              // 0x32
    {ON, "LDBH #$3"},                                              // 0x33
    {ON, "LDBH #$4"},                                              // 0x34
    {ON, "LDBH #$5"},                                              // 0x35
    {ON, "LDBH #$6"},                                              // 0x36
    {ON, "LDBH #$7"},                                              // 0x37
    {ON, "LDBH #$8"},                                              // 0x38
    {ON, "LDBH #$9"},                                              // 0x39
    {ON, "LDBH #$A"},                                              // 0x3A
    {ON, "LDBH #$B"},                                              // 0x3B
    {ON, "LDBH #$C"},                                              // 0x3C
    {ON, "LDBH #$D"},                                              // 0x3D
    {ON, "LDBH #$E"},                                              // 0x3E
    {ON, "LDBH #$F"},                                              // 0x3F
    {ON, "LJLL #$0"},                                              // 0x40
    {ON, "LJLL #$1"},                                              // 0x41
    {ON, "LJLL #$2"},                                              // 0x42
    {ON, "LJLL #$3"},                                              // 0x43
    {ON, "LJLL #$4"},                                              // 0x44
    {ON, "LJLL #$5"},                                              // 0x45
    {ON, "LJLL #$6"},                                              // 0x46
    {ON, "LJLL #$7"},                                              // 0x47
    {ON, "LJLL #$8"},                                              // 0x48
    {ON, "LJLL #$9"},                                              // 0x49
    {ON, "LJLL #$A"},                                              // 0x4A
    {ON, "LJLL #$B"},                                              // 0x4B
    {ON, "LJLL #$C"},                                              // 0x4C
    {ON, "LJLL #$D"},                                              // 0x4D
    {ON, "LJLL #$E"},                                              // 0x4E
    {ON, "LJLL #$F"},                                              // 0x4F
    {ON, "LJLH #$0"},                                              // 0x50
    {ON, "LJLH #$1"},                                              // 0x51
    {ON, "LJLH #$2"},                                              // 0x52
    {ON, "LJLH #$3"},                                              // 0x53
    {ON, "LJLH #$4"},                                              // 0x54
    {ON, "LJLH #$5"},                                              // 0x55
    {ON, "LJLH #$6"},                                              // 0x56
    {ON, "LJLH #$7"},                                              // 0x57  
    {ON, "LJLH #$8"},                                              // 0x58
    {ON, "LJLH #$9"},                                              // 0x59
    {ON, "LJLH #$A"},                                              // 0x5A
    {ON, "LJLH #$B"},                                              // 0x5B
    {ON, "LJLH #$C"},                                              // 0x5C
    {ON, "LJLH #$D"},                                              // 0x5D
    {ON, "LJLH #$E"},                                              // 0x5E
    {ON, "LJLH #$F"},                                              // 0x5F  
    {ON, "LJHL #$0"},                                              // 0x60
    {ON, "LJHL #$1"},                                              // 0x61
    {ON, "LJHL #$2"},                                              // 0x62
    {ON, "LJHL #$3"},                                              // 0x63
    {ON, "LJHL #$4"},                                              // 0x64
    {ON, "LJHL #$5"},                                              // 0x65
    {ON, "LJHL #$6"},                                              // 0x66
    {ON, "LJHL #$7"},                                              // 0x67
    {ON, "LJHL #$8"},                                              // 0x68
    {ON, "LJHL #$9"},                                              // 0x69
    {ON, "LJHL #$A"},                                              // 0x6A
    {ON, "LJHL #$B"},                                              // 0x6B
    {ON, "LJHL #$C"},                                              // 0x6C
    {ON, "LJHL #$D"},                                              // 0x6D
    {ON, "LJHL #$E"},                                              // 0x6E
    {ON, "LJHL #$F"},                                              // 0x6F
    {ON, "LJHH #$0"},                                              // 0x70
    {ON, "LJHH #$1"},                                              // 0x71
    {ON, "LJHH #$2"},                                              // 0x72
    {ON, "LJHH #$3"},                                              // 0x73
    {ON, "LJHH #$4"},                                              // 0x74
    {ON, "LJHH #$5"},                                              // 0x75
    {ON, "LJHH #$6"},                                              // 0x76
    {ON, "LJHH #$7"},                                              // 0x77
    {ON, "LJHH #$8"},                                              // 0x78
    {ON, "LJHH #$9"},                                              // 0x79
    {ON, "LJHH #$A"},                                              // 0x7A
    {ON, "LJHH #$B"},                                              // 0x7B
    {ON, "LJHH #$C"},                                              // 0x7C
    {ON, "LJHH #$D"},                                              // 0x7D
    {ON, "LJHH #$E"},                                              // 0x7E
    {ON, "LJHH #$F"},                                              // 0x7F
    {OO, "BCAA 0"},                                                // 0x80
    {OO, "BCAA 1"},                                                // 0x81
    {OO, "BCAA 2"},                                                // 0x82
    {OO, "BCAA 3"},                                                // 0x83
    {OO, "BCAA 4"},                                                // 0x84
    {OO, "BCAA 5"},                                                // 0x85
    {OO, "BCAA 6"},                                                // 0x86
    {OO, "BCAA 7"},                                                // 0x87
    {OO, "BSAA 0"},                                                // 0x88
    {OO, "BSAA 1"},                                                // 0x89
    {OO, "BSAA 2"},                                                // 0x8A
    {OO, "BSAA 3"},                                                // 0x8B
    {OO, "BSAA 4"},                                                // 0x8C
    {OO, "BSAA 5"},                                                // 0x8D
    {OO, "BSAA 6"},                                                // 0x8E
    {OO, "BSAA 7"},                                                // 0x8F
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x90
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x91
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x92
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x93
    {OO, "BCFL 4               ; or BCFL V"},                      // 0x94
    {OO, "BCFL 5               ; or BCFL C"},                      // 0x95
    {OO, "BCFL 6               ; or BCFL Z"},                      // 0x96
    {OO, "BCFL 7               ; or BCFL S"},                      // 0x97
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x98
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x99
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x9A
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0x9B
    {OO, "BSFL 4               ; or BSFL V"},                      // 0x9C
    {OO, "BSFL 5               ; or BSFL C"},                      // 0x9D
    {OO, "BSFL 6               ; or BSFL Z"},                      // 0x9E
    {OO, "BSFL 7               ; or BSFL Z"},                      // 0x9F
    {OO, "LDA0"},                                                  // 0xA0
    {OO, "LDA1"},                                                  // 0xA1
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xA2
    {OO, "LDAF"},                                                  // 0xA3
    {OO, "LDB0"},                                                  // 0xA4
    {OO, "LDB1"},                                                  // 0xA5
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xA6
    {OO, "LDBF"},                                                  // 0xA7
    {OO, "LDSF"},                                                  // 0xA8
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xA9
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xAA
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xAB
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xAC
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xAD
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xAE
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xAF
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB0
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB1
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB2
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB3
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB4
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB5
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB6
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB7
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB8
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xB9
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xBA
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xBB
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xBC
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xBD
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xBE
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xBF
    {OO, "LOAD (MR)"},                                             // 0xC0
    {OO, "LOAD AB"},                                               // 0xC1
    {OO, "LOAD SL"},                                               // 0xC2
    {OO, "LOAD SH"},                                               // 0xC3
    {OO, "LOAD ML"},                                               // 0xC4
    {OO, "LOAD MH"},                                               // 0xC5
    {OO, "LOAD JL"},                                               // 0xC6
    {OO, "LOAD JH"},                                               // 0xC7
    {OO, "STOR (MR)"},                                             // 0xC8
    {OO, "STOR AB"},                                               // 0xC9
    {OO, "STOR SL"},                                               // 0xCA
    {OO, "STOR SH"},                                               // 0xCB
    {OO, "STOR ML"},                                               // 0xCC
    {OO, "STOR MH"},                                               // 0xCD
    {OO, "STOR JL"},                                               // 0xCE
    {OO, "STOR JH"},                                               // 0xCF
    {OO, "PUSH AA"},                                               // 0xD0
    {OO, "PUSH AB"},                                               // 0xD1
    {OO, "PUSH FL"},                                               // 0xD2
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xD3
    {OO, "PUSH ML"},                                               // 0xD4
    {OO, "PUSH MH"},                                               // 0xD5
    {OO, "PUSH JL"},                                               // 0xD6
    {OO, "PUSH JH"},                                               // 0xD7
    {OO, "POPP AA"},                                               // 0xD8
    {OO, "POPP AB"},                                               // 0xD9
    {OO, "POPP FL"},                                               // 0xDA
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xDB
    {OO, "POPP ML"},                                               // 0xDC
    {OO, "POPP MH"},                                               // 0xDD
    {OO, "POPP JL"},                                               // 0xDE
    {OO, "POPP JH"},                                               // 0xDF
    {OO, "COMP"},                                                  // 0xE0
    {OO, "SUBB"},                                                  // 0xE1
    {OO, "ADDD"},                                                  // 0xE2
    {OO, "ANDD"},                                                  // 0xE3
    {OO, "ORRR"},                                                  // 0xE4
    {OO, "XORR"},                                                  // 0xE5
    {OO, "NAND"},                                                  // 0xE6
    {OO, "NORR"},                                                  // 0xE7
    {OO, "NOTT"},                                                  // 0xE8
    {OO, "NEGG"},                                                  // 0xE9
    {OO, "SHRL"},                                                  // 0xEA
    {OO, "SHLL"},                                                  // 0xEB
    {OO, "SHRA"},                                                  // 0xEC
    {OO, "SRLC"},                                                  // 0xEE
    {OO, "SLLC"},                                                  // 0xEE
    {NN, "DEFB $%02x           ;Invalid opcode"},                  // 0xEF
    {OO, "JPVC"},                                                  // 0xF0
    {OO, "JPVS"},                                                  // 0xF1
    {OO, "JPCC"},                                                  // 0xF2
    {OO, "JPCS"},                                                  // 0xF3
    {OO, "JPZC"},                                                  // 0xF4
    {OO, "JPZS"},                                                  // 0xF5
    {OO, "JPSC"},                                                  // 0xF6
    {OO, "JPSS"},                                                  // 0xF7
    {OO, "INCA"},                                                  // 0xF8
    {OO, "DECA"},                                                  // 0xF9
    {OO, "INCB"},                                                  // 0xFA
    {OO, "DECB"},                                                  // 0xFB
    {OO, "INCM"},                                                  // 0xFC
    {OO, "JUMP"},                                                  // 0xFD
    {OO, "NOOP"},                                                  // 0xFE
    {OO, "HALT"},                                                  // 0xFF
};

#endif // SRC_HOMEMADE_CPU2_OPCODES_H_