// Z80 core definitions
#include "Z80.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>
#include <iomanip>

#define MAX_BUFFER 8

void Z80::execute() {
    switch (IR[0]) {
        case 0xCB: 
            execute_bit_opcode();
            break;

        case 0xDD: 
            if (IR[1] == 0xCB) execute_ix_bit_opcode();
            else execute_ix_opcode();
            break;

        case 0xED: 
            execute_misc_opcode();
            break;

        case 0xFD:
            if (IR[1] == 0xCB) execute_iy_bit_opcode();
            else execute_iy_opcode();
            break;

        default:
            execute_main_opcode();
            break;
    }
}

void Z80::execute_main_opcode() {
    unsigned char *r = nullptr, *r_ = nullptr;   // Temporary storage when decoding register field in opcode
    unsigned char Temp;
    unsigned short Temp16;

    switch (IR[0]) {

        // ************* 8-bit Load Group *************
        // Flags are not affected by these instructions
        // ********************************************
        //
        // LD r, r' instructions (0x40 - 0x7f)
        // LD r, (HL) 
        // LD (HL), r
        // Instructions with register bit pattern 0b110 represent (HL) which are listed in 
        // separate subgroups in Z80 user manual, but can be implemented here,
        // with the exeption of Opcodes 0x76 HALT which is implemented elsewhere
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
        case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
        case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75:            case 0x77:
        case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
            // Determine which registers we are working on:
            // Opcode 0  1  r  r  r  r' r' r'
            switch (IR[0] & 0x07) {
                case 0b000: r_ = &B; break;
                case 0b001: r_ = &C; break;
                case 0b010: r_ = &D; break;
                case 0b011: r_ = &E; break;
                case 0b100: r_ = &H; break;
                case 0b101: r_ = &L; break;
                case 0b110: r_ = &memory[(H<<8) + L]; break;  // (HL)
                case 0b111: r_ = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            *r = *r_;        // LD r, r"
            // Condition bits affected: None
            break;

        // LD r, n instructions (0x06/0x0e - 0x36/0x3e)
        // LD (HL), n           (0x36)
        // Instruction 0x36 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0x06: case 0x0e: case 0x16: case 0x1e: case 0x26: case 0x2e: case 0x36: case 0x3e:
            // Opcode 0  0  r  r  r  1  1  0
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;  // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            *r = IR[1];        // LD r, n
            // Condition bits affected: None
            break;

        // LD A, (BC)  (0A)
        case 0x0A: 
            A = memory[(B<<8) + C];
            // Condition bits affected: None
            break;

        // LD A, (DE)  (1A)
        case 0x1A: 
            A = memory[(D<<8) + E];
            // Condition bits affected: None
            break;

        // LD A, (nn)  (3A)
        case 0x3A: 
            A = memory[(IR[2]<<8) + IR[1]];
            // Condition bits affected: None
            break;

        // LD (BC), A  (02)
        case 0x02: 
            memory[(B<<8) + C] = A;   // LD A, (BC)
            // Condition bits affected: None
            break;

        // LD (DE),A  (12)
        case 0x12: 
            memory[(D<<8) + E] = A;
            // Condition bits affected: None
            break;

        // LD (nn),A  (32)
        case 0x32: 
            memory[(IR[2]<<8) + IR[1]] = A;
            // Condition bits affected: None
            break;


        // ************* 16-bit Load Group *************
        // Flags are not affected by these instructions
        // ********************************************
        //
        // LD dd, nn instructions (0x01, 0x11, 0x21, 0x31)
        case 0x01: case 0x11: case 0x21: case 0x31:
            // Determine which register we are working on:
            // Opcode 0  0  d  d  0  0  0  1
            if ((IR[0] & 0x30) == 0x30) { // Need special handling for SP since it is modeled as 16 bits instead of two 8-bit registers
                SP = (IR[2]<<8) + IR[1];
            }
            else {
                switch ((IR[0] & 0x30) >> 4) {
                    case 0b00: r = &B; r_ = &C; break;
                    case 0b01: r = &D; r_ = &E; break;
                    case 0b10: r = &H; r_ = &L; break;
                    default: cout << "Invalid opcode" << endl; break;
                }
                *r  = IR[2];
                *r_ = IR[1];
            }
            // Condition bits affected: None
            break;

        // LD HL, (nn)     (0x2a)
        case 0x2a:
                H = memory[(IR[2]<<8) + IR[1] + 1];
                L = memory[(IR[2]<<8) + IR[1]];
            // Condition bits affected: None
            break;

        // LD (nn), HL (0x22)
        case 0x22:
            memory[(IR[2]<<8) + IR[1] + 1] = H;
            memory[(IR[2]<<8) + IR[1]]     = L;
            // Condition bits affected: None
            break;

        // LD SP, HL   (0xf9)
        case 0xf9:
            SP = (H<<8) + L;
            // Condition bits affected: None
            break;

        // PUSH qq     (0xc5, 0xd5, 0xe5, 0xf5)
        case 0xc5: case 0xd5: case 0xe5: case 0xf5:
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00: r = &B; r_ = &C; break;
                case 0b01: r = &D; r_ = &E; break;
                case 0b10: r = &H; r_ = &L; break;
                case 0b11: r = &A; r_ = &F; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            memory[--SP] = *r;
            memory[--SP] = *r_;
            // Condition bits affected: None
            break;      

        // POP qq     (0xc1, 0xd1, 0xe1, 0xf1)
        case 0xc1: case 0xd1: case 0xe1: case 0xf1:
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00: r = &B; r_ = &C; break;
                case 0b01: r = &D; r_ = &E; break;
                case 0b10: r = &H; r_ = &L; break;
                case 0b11: r = &A; r_ = &F; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            *r  = memory[SP++];
            *r_ = memory[SP++];
            // Condition bits affected: None
            break;      

        // ************* Exchange, Block Transfer, and Search Group *************
        // Flags are not affected by these instructions
        // **********************************************************************
        //
        // EX DE, HL (0xeb)
        case 0xeb:
            Temp = D;
            D  = H;
            H  = Temp;
            Temp = E;
            E  = L;
            L  = Temp;
            // Condition bits affected: None
            break;

        // EX AF, AF'  (0x08)
        case 0x08:
            Temp   = A;
            A      = Aprime;
            Aprime = Temp;
            Temp   = F;
            F      = Fprime;
            Fprime = Temp;
            // Condition bits affected: None
            break;

        // EXX (0xd9)
        case 0xd9:
            Temp     = B;
            B      = Bprime;
            Bprime = Temp;
            Temp     = C;
            C      = Cprime;
            Cprime = Temp;
            Temp     = D;
            D      = Dprime;
            Dprime = Temp;
            Temp     = E;
            E      = Eprime;
            Eprime = Temp;
            Temp     = H;
            H      = Hprime;
            Hprime = Temp;
            Temp     = L;
            L      = Lprime;
            Lprime = Temp;
            // Condition bits affected: None
            break;

        // EX (SP), HL (0xe3)
        case 0xe3:
            Temp           = H;
            H            = memory[SP+1];
            memory[SP+1] = Temp;
            Temp           = L;
            L            = memory[SP];
            memory[SP]   = Temp;
            // Condition bits affected: None
            break;

        // ************* 8-Bit Arithmetic Group *************
        // **************************************************
        // ADD A, r instructions (0x80 - 0x85, 0x87)
        // ADD A, (HL)           (0x86)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
            // Opcode 1  0  0  0  0  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            A += *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, ADD, A, *r);
            break;

        // ADD A, n    (0xC6)
        case 0xC6:
            A += memory[IR[1]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, ADD, A, memory[IR[1]]);
            break;

        // ADC A, r instructions (0x88 - 0x8D, 0x8F)
        // ADC A, (HL)           (0x8E)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
            // Opcode 1  0  0  0  1  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            A = A + *r + testFlag(C_BIT);
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, ADC, A, *r);
            break;

        // ADC A, n    (0xCE)
        case 0xCE:
            A = A + memory[IR[1]] + testFlag(C_BIT);
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, ADC, A, memory[IR[1]]);
            break;

        // SUB A, r instructions (0x90 - 0x95, 0x97)
        // SUB A, (HL)           (0x96)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
            // Opcode 1  0  0  1  0  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;  // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            A = A - *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SUB, A, *r);
            break;

        // SUB A, n    (0xd6)
        case 0xd6:
            A = A - memory[IR[1]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SUB, A, memory[IR[1]]);
            break;

        // SBC A, r instructions (0x98 - 0x9D, 0x9F)
        // SBC A, (HL)           (0x9E)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
            // Opcode 1  0  0  1  1  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            A = A - *r - testFlag(C_BIT);
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SBC, A, *r);
            break;

        // SBC A, n    (0xDE)
        case 0xde:
            A = A - memory[IR[1]] - testFlag(C_BIT);
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SBC, A, memory[IR[1]]);
            break;

        // AND A, r instructions (0xA0 - 0xA5, 0xA7)
        // AND A, (HL)           (0xA6)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
            // Opcode 1  0  1  0  0  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            A = A & *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, AND, A, *r);
            break;

        // AND A, n    (0xe6)
        case 0xe6:
            A = A & memory[IR[1]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, AND, A, memory[IR[1]]);
            break;

        // OR A, r instructions (0xB0 - 0xB5, 0xB7)
        // OR A, (HL)           (0xB6)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
            // Opcode 1  0  1  1  0  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            A = A | *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, OR, A, *r);
            break;

        // OR A, n    (0xf6)
        case 0xf6:
            A = A | memory[IR[1]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, OR, A, memory[IR[1]]);
            break;

        // XOR A, r instructions (0xA8 - 0xAD, 0xAF)
        // XOR A, (HL)           (0xAE)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
            // Opcode 1  0  1  0  1  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            A = A ^ *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, XOR, A, *r);
            break;

        // XOR A, n    (0xee)
        case 0xee:
            A = A ^ memory[IR[1]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, XOR, A, memory[IR[1]]);
            break;

        // CP r instructions (0xB8 - 0xBD, 0xBF)
        // CP (HL)           (0xBE)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
            // Opcode 1  0  1  1  1  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            // Compare only; register contents unchanged
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SUB, A, *r);
            break;

        // CP n    (0xfe)
        case 0xfe:
            // Compare only; register contents unchanged
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SUB, A, memory[IR[1]]);
            break;

        // INC r instructions (0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C)
        // INC (HL)           (0x34)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0x04: case 0x0c: case 0x14: case 0x1c: case 0x24: case 0x2c: case 0x34: case 0x3c:
            // Opcode 0  0  r  r  r  1  0  0 
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, ADD, *r, 1);
            (*r)++;
            break;

        // DEC r instructions (0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D)
        // DEC (HL)           (0x35)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroup in Z80 user manual, but can be implemented here
        case 0x05: case 0x0d: case 0x15: case 0x1d: case 0x25: case 0x2d: case 0x35: case 0x3d:
            // Opcode 0  0  r  r  r  1  0  1 
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SUB, *r, 1);
            (*r)--;
            break;

        // ************* General Purpose Arithmetic and CPU Control Groups *************
        // *****************************************************************************
        
        // DAA (0x27)
        /// *** Need to implement *** ///
        case 0x27:
            break;

        // CPL (0x2F)
        case 0x2f:
            A = ~A;
            setFlag(H_BIT);
            setFlag(N_BIT);
            break;

        // CCF (0x3F)
        case 0x3f:
            if (testFlag(C_BIT)) {
                clearFlag(C_BIT); 
                setFlag(H_BIT);
            }
            else  {
                setFlag(C_BIT);
                clearFlag(H_BIT);
            }
            clearFlag(N_BIT);
            break;

        // SCF (0x37)
        case 0x37:
            setFlag(C_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // NOP (0x00)
        case 0x00:
            break;
        
        // HALT (0x76)
        case 0x76: 
            Halt = 1;
            break;

        // DI  (0xF3)
        case 0xf3:
            IFF1 = 0;
            IFF2 = 0;
            break;

        // EI  (0xFB)
        case 0xfb: 
            IFF1 = 1;
            IFF2 = 1;
            break;

        // ************* 16-bit Arithmetic Group *************
        // ***************************************************
        
        // ADD HL, ss  (0x09, 0x19, 0x29, 0x39)
        case 0x09: case 0x19: case 0x29: case 0x39:
            Temp16 = (H << 8) + L;
            // Determine which register we are working on:
            // Opcode 0  0  s  s  1  0  0  1
            if ((IR[0] & 0x30) == 0x30) { // Need special handling for SP since it is modeled as 16 bits instead of two 8-bit registers
                /// Need to implement ///
            }
            else {
                switch ((IR[0] & 0x30) >> 4) {
                    case 0b00: r = &B; r_ = &C; break;
                    case 0b01: r = &D; r_ = &E; break;
                    case 0b10: r = &H; r_ = &L; break;
                    default: cout << "Invalid opcode" << endl; break;
                }
                /// Need to implement ///
            }
            /// Need to implement condition bits, may need another state ///
            break;

        // INC ss (0x03, 0x013, 0x23, 0x33)
        case 0x03: case 0x13: case 0x23: case 0x33: 
            // Determine which register we are working on:
            // Opcode 0  0  s  s  0  0  1  1
            if ((IR[0] & 0x30) == 0x30) { // Need special handling for SP since it is modeled as 16 bits instead of two 8-bit registers
                SP++;
            }
            else {
                switch ((IR[0] & 0x30) >> 4) {
                    case 0b00: Temp16 = (B<<8) + C; Temp16++; B = (Temp16 & 0xff00)>>8; C = (Temp16 & 0x00ff); break;
                    case 0b01: Temp16 = (D<<8) + E; Temp16++; D = (Temp16 & 0xff00)>>8; E = (Temp16 & 0x00ff); break;
                    case 0b10: Temp16 = (H<<8) + L; Temp16++; H = (Temp16 & 0xff00)>>8; L = (Temp16 & 0x00ff); break;
                    default: cout << "Invalid opcode" << endl; break;
                }
                /// Need to implement ///
            }
            // Condition bits affected: None
            break;
            
        // DEC ss (0x0b, 0x01b, 0x2b, 0x3b)
        case 0x0b: case 0x1b: case 0x2b: case 0x3b: 
            // Determine which register we are working on:
            // Opcode 0  0  s  s  1  0  1  1
            if ((IR[0] & 0x30) == 0x30) { // Need special handling for SP since it is modeled as 16 bits instead of two 8-bit registers
                SP--;
            }
            else {
                switch ((IR[0] & 0x30) >> 4) {
                    case 0b00: Temp16 = (B<<8) + C; Temp16--; B = (Temp16 & 0xff00)>>8; C = (Temp16 & 0x00ff); break;
                    case 0b01: Temp16 = (D<<8) + E; Temp16--; D = (Temp16 & 0xff00)>>8; E = (Temp16 & 0x00ff); break;
                    case 0b10: Temp16 = (H<<8) + L; Temp16--; H = (Temp16 & 0xff00)>>8; L = (Temp16 & 0x00ff); break;
                    default: cout << "Invalid opcode" << endl; break;
                }
                /// Need to implement ///
            }
            // Condition bits affected: None
            break;

        // ************* Rotate and Shift Group **************
        // ***************************************************
        
        // RLCA (0x07)
        case 0x07: 
            if (A & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            A = A << 1;
            if (testFlag(C_BIT)) A |= 0x01; else A &= 0xfe;
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // RLA (0x17)
        case 0x17:
            Temp = testFlag(C_BIT);
            if (A & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            A = A << 1; 
            if (Temp) A |= 0x01; else A &= 0xfe;
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // RRCA (0x0F)
        case 0x0f:
            if (A & 0x01) setFlag(C_BIT); else clearFlag(C_BIT);
            A = A >> 1; 
            if (testFlag(C_BIT)) A |= 0x80; else A &= 0x7f;
            clearFlag(H_BIT);
            clearFlag(N_BIT);

        // RRA (0x1F)
        case 0x1f:
            Temp = testFlag(C_BIT);
            A = A >> 1;
            if (Temp) A |= 0x80; else A &= 0x7f;
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        default: 
            cout << "Execution not defined: 0x" << hex << setw(2) << (unsigned int) IR[0] << endl;
            break;
    }
}

void Z80::execute_misc_opcode() {  // IR[0] = 0xED

    switch (IR[1]) {
        case 0x00:  // Invalid opcode, executing NOP
            break;

        default: 
            cout << "Execution not defined: 0x" << hex << setw(2) << (unsigned int) IR[0] 
                 << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}

void Z80::execute_bit_opcode() {  // IR[0] == 0xCB

    switch (IR[1]) {
        case 0x00:  // RLC B
            if (B & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            B = B << 1;
            B = (B & 0xFE) | testFlag(C_BIT);
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, BIT, B, 0);
            break;

        default: 
            cout << "Execution not defined: 0x" << hex << setw(2) << (unsigned int) IR[0] 
                 << setw(2) << (unsigned int) IR[1] << endl;
           break;
    }
}

void Z80::execute_ix_opcode() {  // IR[0] = 0xDD

    switch (IR[1]) {
        case 0x00:  // NOP -- no flags affected
            break;

        case 0x04:  // INC B
            B++;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, ADD, B, 1);
            break;            

        default: 
            cout << "Execution not defined: 0x" << hex << setw(2) << (unsigned int) IR[0] 
                 << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}

void Z80::execute_ix_bit_opcode() {  // IR[0,1] = 0xDDCB

    switch (IR[3]) {
        case 0x00:  // RLC (IX+d), B
            if (memory[IX+IR[2]] & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            memory[IX+IR[2]] = memory[IX+IR[2]] << 1;
            memory[IX+IR[2]] = (memory[IX+IR[2]] & 0xFE) | testFlag(C_BIT);
            B = memory[IX+IR[2]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, BIT, memory[IX+IR[2]], 0);
            break;

        default: 
            cout << "Execution not defined: 0x" << hex << setw(2) << (unsigned int) IR[0] 
                 << setw(2) << (unsigned int) IR[1] << "nn" 
                 << setw(3) << (unsigned int) IR[3] << endl;
            break;
    }
}

void Z80::execute_iy_opcode() { // IR[0] = 0xFD

    switch (IR[1]) {
        case 0x00:  // NOP -- no flags affected
            break;

        case 0x04:  // INC B
            B++;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, ADD, B, 1);
            break;            

        default: 
            cout << "Execution not defined: 0x" << hex << setw(2) << (unsigned int) IR[0] 
                 << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}

void Z80::execute_iy_bit_opcode() {  // IR[0,1] = 0xFDCB

    switch (IR[3]) {
        case 0x00:  // RLC (IY+d), B
            if (memory[IY+IR[2]] & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            memory[IY+IR[2]] = memory[IY+IR[2]] << 1;
            memory[IY+IR[2]] = (memory[IY+IR[2]] & 0xFE) | testFlag(C_BIT);
            B = memory[IY+IR[2]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, BIT, memory[IY+IR[2]], 0);
            break;

        default: 
            cout << "Execution not defined: 0x" << hex << setw(2) << (unsigned int) IR[0] 
                 << setw(2) << (unsigned int) IR[1] << "nn" 
                 << setw(3) << (unsigned int) IR[3] << endl;
            break;
    }
}
