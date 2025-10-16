/* Homemade CPU Emulator, Version 1
   Copyright 2025 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   See https://github.com/Andy4495/Homemade-CPU for the CPU this emulates

   This code is based off version 1 of the CPU design. It has been superseded by Version 2.

   0.1  09-Oct-2025  Andy4495  Initial Creation


*/

// Homemade CPU Opcodes
//
// Structure to define Homemade CPU opcodes and mnemonics
// All instructions are of length 2

#ifndef SRC_HOMEMADE_CPU1_OPCODES_H_
#define SRC_HOMEMADE_CPU1_OPCODES_H_

// Specify the instruction byte structure
// O -> Opcode
// N -> Data byte
// U -> Unused byte
enum CPUv1_OPCODE_STRUCTURE {ON, OU, NN};

struct Homemade_CPUv1_opcodes {      // The opcode value is implicit in the array index
    CPUv1_OPCODE_STRUCTURE s;
    const char* mn;       // Instruction mnemonic, not including data
};

// The actual opcode value is implicitly represented by the array index
const Homemade_CPUv1_opcodes opcodes[] = {
    {ON, "LOAD #$%02x"},                                           // 0x00
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x01
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x02
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x03
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x04
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x05
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x06
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x07
    {ON, "LOAD ($%02x)"},                                          // 0x08
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x09
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0A
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0B
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0C
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0D
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0F
    {OU, "LOAD AC"},                                               // 0x10
    {OU, "LOAD FL"},                                               // 0x11
    {OU, "LOAD SH"},                                               // 0x12
    {OU, "LOAD SL"},                                               // 0x13
    {OU, "LOAD MH"},                                               // 0x14
    {OU, "LOAD JH"},                                               // 0x15
    {OU, "LOAD JL"},                                               // 0x16
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x17
    {ON, "STOR ($%02x)"},                                          // 0x18
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x19
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1A
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1B
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1C
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1D
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1F   
    {OU, "STOR AC"},                                               // 0x20
    {OU, "STOR FL"},                                               // 0x21
    {OU, "STOR SH"},                                               // 0x22
    {OU, "STOR SL"},                                               // 0x23
    {OU, "STOR MH"},                                               // 0x24
    {OU, "STOR JH"},                                               // 0x25
    {OU, "STOR JL"},                                               // 0x26
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x27
    {OU, "PUSH AC"},                                               // 0x28
    {OU, "PUSH FL"},                                               // 0x29
    {OU, "PUSH SH"},                                               // 0x2A
    {OU, "PUSH SL"},                                               // 0x2B
    {OU, "PUSH MH"},                                               // 0x2C
    {OU, "PUSH JH"},                                               // 0x2D
    {OU, "PUSH JL"},                                               // 0x2E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x2F   
    {ON, "PUSH #$%02x"},                                           // 0x30
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x31
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x32
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x33
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x34
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x35
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x36
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x37
    {OU, "POPP AC"},                                               // 0x38
    {OU, "POPP FL"},                                               // 0x39
    {OU, "POPP SH"},                                               // 0x3A
    {OU, "POPP SL"},                                               // 0x3B
    {OU, "POPP MH"},                                               // 0x3C
    {OU, "POPP JH"},                                               // 0x3D
    {OU, "POPP JL"},                                               // 0x3E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x3F    
    {ON, "COMP #$%02x"},                                           // 0x40
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x41
    {ON, "SUBB #$%02x"},                                           // 0x42
    {NN, "DEFB $%02x, $%02"},                                      // 0x43
    {ON, "ADDD #$%02x"},                                           // 0x44
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x45
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x46
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x47
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x48
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x49
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4A
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4B
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4C
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4D
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4F    
    {ON, "ANDD #$%02x"},                                           // 0x50
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x51
    {ON, "ORRR #$%02x"},                                           // 0x52
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x53
    {ON, "XORR #$%02x"},                                           // 0x54
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x55
    {ON, "NAND #$%02x"},                                           // 0x56
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x57
    {ON, "NORR #$%02x"},                                           // 0x58
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x59
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5A
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5B
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5C
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5D
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5F    
    {OU, "NOTT"},                                                  // 0x60
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x61
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x62
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x63
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x64
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x65
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x66
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x67
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x68
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x69
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6A
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6B
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6C
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6D
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6F    
    {OU, "SHRL"},                                                  // 0x70
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x71
    {OU, "SHLL"},                                                  // 0x72
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x73
    {OU, "SHRA"},                                                  // 0x74
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x75
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x76
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x77
    {OU, "ROTR"},                                                  // 0x78
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x79
    {OU, "RRTC"},                                                  // 0x7A
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7B
    {OU, "ROTL"},                                                  // 0x7C
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7D
    {OU, "RLTC"},                                                  // 0x7E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7F    
    {OU, "BITC 0"},                                                // 0x80
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x81
    {OU, "BITC 1"},                                                // 0x82
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x83
    {OU, "BITC 2"},                                                // 0x84
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x85
    {OU, "BITC 3"},                                                // 0x86
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x87
    {OU, "BITC 4"},                                                // 0x88
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x89
    {OU, "BITC 5"},                                                // 0x8A
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x8B
    {OU, "BITC 6"},                                                // 0x8C
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x8D
    {OU, "BITC 7"},                                                // 0x8E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x8F    
    {OU, "BITS 0"},                                                // 0x90
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x91
    {OU, "BITS 1"},                                                // 0x92
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x93
    {OU, "BITS 2"},                                                // 0x94
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x95
    {OU, "BITS 3"},                                                // 0x96
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x97
    {OU, "BITS 4"},                                                // 0x98
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x99
    {OU, "BITS 5"},                                                // 0x9A
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x9B
    {OU, "BITS 6"},                                                // 0x9C
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x9D
    {OU, "BITS 7"},                                                // 0x9E
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x9F    
    {OU, "CLRV"},                                                  // 0xA0
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA1
    {OU, "CLRS"},                                                  // 0xA2
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA3
    {OU, "CLRC"},                                                  // 0xA4
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA5
    {OU, "CLRZ"},                                                  // 0xA6
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA7
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA8
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA9
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAA
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAB
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAC
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAD
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAE
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAF    
    {OU, "SETV"},                                                  // 0xB0
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB1
    {OU, "SETS"},                                                  // 0xB2
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB3
    {OU, "SETC"},                                                  // 0xB4
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB5
    {OU, "SETZ"},                                                  // 0xB6
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB7
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB8
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB9
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBA
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBB
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBC
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBD
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBE
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBF    
    {ON, "JPVC #$%02x"},                                           // 0xC0
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC1
    {ON, "JPSC #$%02x"},                                           // 0xC2
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC3
    {ON, "JPCC #$%02x"},                                           // 0xC4
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC5
    {ON, "JPZC #$%02x"},                                           // 0xC6
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC7
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC8
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC9
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCA
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCB
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCC
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCD
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCE
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCF    
    {ON, "JPVS #$%02x"},                                           // 0xD0
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD1
    {ON, "JPSS #$%02x"},                                           // 0xD2
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD3
    {ON, "JPCS #$%02x"},                                           // 0xD4
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD5
    {ON, "JPZS #$%02x"},                                           // 0xD6
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD7
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD8
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD9
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDA
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDB
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDC
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDD
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDE
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDF    
    {ON, "JUMP #$%02x"},                                           // 0xE0
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE1
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE2
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE3
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE4
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE5
    {OU, "JUMP JL"},                                               // 0xE6
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE7
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE8
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE9
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEA
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEB
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEC
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xED
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEE
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEF    
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF0
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF1
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF2
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF3
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF4
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF5
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF6
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF7
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF8
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF9
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xFA
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xFB
    {OU, "NOOP"},                                                  // 0xFC
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xFD
    {NN, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xFE
    {OU, "HALT"},                                                  // 0xFF

};

#endif // SRC_HOMEMADE_CPU1_OPCODES_H_