/* Homemade CPU Emulator 
   Copyright 2025 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   See https://github.com/Andy4495/Homemade-CPU for the CPU this emulates

   0.1  09-Oct-2025  Andy4495  Initial Creation


*/

// Homemade CPU Opcodes
//
// Structure to define Homemade CPU opcodes and mnemonics
// All instructions are of length 2

#ifndef SRC_HOMEMADE_CPU_OPCODES_H_
#define SRC_HOMEMADE_CPU_OPCODES_H_

// Specify the instruction byte structure
// O -> Opcode
// N -> Data byte
// U -> Unused byte
enum CPU_OPCODE_STRUCTURE {ON, OU};

struct Homemade_CPU_opcodes {      // The opcode value is implicit in the array index
    CPU_OPCODE_STRUCTURE s;
    const char* mn;       // Instruction mnemonic, not including data
};

// The actual opcode value is implicitly represented by the array index
const Homemade_CPU_opcodes opcodes[] = {
    {ON, "LOAD #dd"},                                              // 0x00
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x01
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x02
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x03
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x04
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x05
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x06
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x07
    {ON, "LOAD (mm)"},                                             // 0x08
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x09
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0A
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0B
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0C
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0D
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x0F
    {ON, "LOAD AC"},                                               // 0x10
    {ON, "LOAD FL"},                                               // 0x11
    {ON, "LOAD SH"},                                               // 0x12
    {ON, "LOAD SL"},                                               // 0x13
    {ON, "LOAD MH"},                                               // 0x14
    {ON, "LOAD JH"},                                               // 0x15
    {ON, "LOAD JL"},                                               // 0x16
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x17
    {ON, "STOR (mm)"},                                             // 0x18
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x19
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1A
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1B
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1C
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1D
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x1F   
    {ON, "STOR AC"},                                               // 0x20
    {ON, "STOR FL"},                                               // 0x21
    {ON, "STOR SH"},                                               // 0x22
    {ON, "STOR SL"},                                               // 0x23
    {ON, "STOR MH"},                                               // 0x24
    {ON, "STOR JH"},                                               // 0x25
    {ON, "STOR JL"},                                               // 0x26
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x27
    {ON, "PUSH AC"},                                               // 0x28
    {ON, "PUSH FL"},                                               // 0x29
    {ON, "PUSH SH"},                                               // 0x2A
    {ON, "PUSH SL"},                                               // 0x2B
    {ON, "PUSH MH"},                                               // 0x2C
    {ON, "PUSH JH"},                                               // 0x2D
    {ON, "PUSH JL"},                                               // 0x2E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x2F   
    {ON, "PUSH #dd"},                                              // 0x30
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x31
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x32
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x33
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x34
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x35
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x36
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x37
    {ON, "POPP AC"},                                               // 0x38
    {ON, "POPP FL"},                                               // 0x39
    {ON, "POPP SH"},                                               // 0x3A
    {ON, "POPP SL"},                                               // 0x3B
    {ON, "POPP MH"},                                               // 0x3C
    {ON, "POPP JH"},                                               // 0x3D
    {ON, "POPP JL"},                                               // 0x3E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x3F    
    {ON, "COMP #dd"},                                              // 0x40
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x41
    {ON, "SUBB #dd"},                                              // 0x42
    {ON, "DEFB $%02x, $%02"},                                      // 0x43
    {ON, "ADDD #dd"},                                              // 0x44
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x45
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x46
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x47
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x48
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x49
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4A
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4B
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4C
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4D
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x4F    
    {ON, "ANDD #dd"},                                              // 0x50
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x51
    {ON, "ORRR #dd"},                                              // 0x52
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x53
    {ON, "NOTT #dd"},                                              // 0x54
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x55
    {ON, "XORR #dd"},                                              // 0x56
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x57
    {ON, "NAND #dd"},                                              // 0x58
    {ON, "NORR #dd"},                                              // 0x59
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5A
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5B
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5C
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5D
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x5F    
    {ON, "SHRL"},                                                  // 0x60
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x61
    {ON, "SHLL"},                                                  // 0x62
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x63
    {ON, "SHRA"},                                                  // 0x64
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x65
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x66
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x67
    {ON, "ROTR"},                                                  // 0x68
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x69
    {ON, "RRTC"},                                                  // 0x6A
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6B
    {ON, "ROTL"},                                                  // 0x6C
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6D
    {ON, "RLTC"},                                                  // 0x6E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x6F    
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x70
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x71
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x72
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x73
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x74
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x75
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x76
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x77
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x78
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x79
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7A
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7B
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7C
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7D
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x7F    
    {ON, "BITC 0"},                                                // 0x80
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x81
    {ON, "BITC 1"},                                                // 0x82
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x83
    {ON, "BITC 2"},                                                // 0x84
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x85
    {ON, "BITC 3"},                                                // 0x86
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x87
    {ON, "BITC 4"},                                                // 0x88
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x89
    {ON, "BITC 5"},                                                // 0x8A
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x8B
    {ON, "BITC 6"},                                                // 0x8C
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x8D
    {ON, "BITC 7"},                                                // 0x8E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x8F    
    {ON, "BITS 0"},                                                // 0x90
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x91
    {ON, "BITS 1"},                                                // 0x92
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x93
    {ON, "BITS 2"},                                                // 0x94
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x95
    {ON, "BITS 3"},                                                // 0x96
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x97
    {ON, "BITS 4"},                                                // 0x98
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x99
    {ON, "BITS 5"},                                                // 0x9A
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x9B
    {ON, "BITS 6"},                                                // 0x9C
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x9D
    {ON, "BITS 7"},                                                // 0x9E
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0x9F    
    {ON, "CLRV"},                                                  // 0xA0
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA1
    {ON, "CLRS"},                                                  // 0xA2
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA3
    {ON, "CLRC"},                                                  // 0xA4
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA5
    {ON, "CLRZ"},                                                  // 0xA6
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA7
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA8
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xA9
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAA
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAB
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAC
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAD
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAE
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xAF    
    {ON, "SETV"},                                                  // 0xB0
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB1
    {ON, "SETS"},                                                  // 0xB2
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB3
    {ON, "SETC"},                                                  // 0xB4
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB5
    {ON, "SETZ"},                                                  // 0xB6
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB7
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB8
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xB9
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBA
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBB
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBC
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBD
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBE
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xBF    
    {ON, "JPVC"},                                                  // 0xC0
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC1
    {ON, "JPSC"},                                                  // 0xC2
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC3
    {ON, "JPCC"},                                                  // 0xC4
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC5
    {ON, "JPZC"},                                                  // 0xC6
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC7
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC8
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xC9
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCA
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCB
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCC
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCD
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCE
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xCF    
    {ON, "JPVS"},                                                  // 0xD0
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD1
    {ON, "JPSS"},                                                  // 0xD2
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD3
    {ON, "JPCS"},                                                  // 0xD4
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD5
    {ON, "JPZS"},                                                  // 0xD6
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD7
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD8
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xD9
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDA
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDB
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDC
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDD
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDE
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xDF    
    {ON, "JUMP"},                                                  // 0xE0
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE1
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE2
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE3
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE4
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE5
    {ON, "JUMP JL"},                                               // 0xE6
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE7
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE8
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xE9
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEA
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEB
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEC
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xED
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEE
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xEF    
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF0
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF1
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF2
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF3
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF4
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF5
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF6
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF7
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF8
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xF9
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xFA
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xFB
    {ON, "NOOP"},                                                  // 0xFC
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xFD
    {ON, "DEFB $%02x, $%02x    ;Invalid opcode"},                  // 0xFE
    {ON, "HALT"},                                                  // 0xFF

};

#endif // SRC_HOMEMADE_CPU_OPCODES_H_