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
    unsigned char *r, *r_;   // Temporary storage when decoding register field in opcode

    switch (IR[0]) {
        case 0x00:  // NOP -- no flags affected
            break;

        // ************* 8-bit load Group *************
        // Flags are not affected by these instructions
        // ********************************************
        // LD r, r' instructions (0x40 - 0x7f)
        // LD r, (HL) 
        // LD (HL), r
        // Instructions with register bit pattern 0b110 represent (HL) which are listed in 
        // separate subgroupts in Z80 user manual, but can be implemented here,
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
                case 0b110: r_ = &memory[(H<<8) + L];   // (HL)
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
                case 0b110: r = &memory[(H<<8) + L];    // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            *r = *r_;        // LD r, r"
            // Condition bits affected: None
            break;

        // LD r, n instructions (0x06/0x0e - 0x36/0x3e)
        // LD (HL), n           (0x36)
        // Instruction 0x36 with register bit pattern 0b110 represents (HL) which is listed in a
        // separate subgroupt in Z80 user manual, but can be implemented here
        case 0x06: case 0x0e: case 0x16: case 0x1e: case 0x26: case 0x2e: case 0x36: case 0x3e:
            // Opcode 0  0  r  r  r  1  1  0
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[(H<<8) + L];    // (HL)
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



/// Example        
        case 0x05:  // DEC B -- S, Z, H, P/V, N
            B--;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, SUB, B, 1);
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
            F.C = (B & 0x80) ? 1 : 0;
            B = B << 1;
            B = (B & 0xFE) | F.C;
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
            F.C = (memory[IX+IR[2]] & 0x80) ? 1 : 0;
            memory[IX+IR[2]] = memory[IX+IR[2]] << 1;
            memory[IX+IR[2]] = (memory[IX+IR[2]] & 0xFE) | F.C;
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
            F.C = (memory[IY+IR[2]] & 0x80) ? 1 : 0;
            memory[IY+IR[2]] = memory[IY+IR[2]] << 1;
            memory[IY+IR[2]] = (memory[IY+IR[2]] & 0xFE) | F.C;
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
