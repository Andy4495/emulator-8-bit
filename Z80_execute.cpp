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
    unsigned char F_;

    switch (IR[0]) {
        case 0x00:  // NOP -- no flags affected
            break;

        // ************* 8-bit Load Group *************
        // Flags are not affected by these instructions
        // ********************************************
        //
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
                case 0b11: r = &A; F_ = F.S|F.Z|F.X1|F.H|F.X2|F.PV|F.N|F.C; r_ = &F_; break;
                default: cout << "Invalid opcode" << endl; break;
            }
            memory[--SP] = *r;
            memory[--SP] = *r_;
            // Condition bits affected: None
            break;      

        // POP qq     (0xc6, 0xd6, 0xe6, 0xf6)
        case 0xc6: case 0xd6: case 0xe6: case 0xf6:
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00: r = &B; r_ = &C; break;
                case 0b01: r = &D; r_ = &E; break;
                case 0b10: r = &H; r_ = &L; break;
                case 0b11: r = &A; r_ = &F_; break;
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
            F_ = D;
            D  = H;
            H  = F_;
            F_ = E;
            E  = L;
            L  = F_;
            // Condition bits affected: None
            break;

        // EX AF, AF'  (0x08)
        case 0x08:
            F_     = A;
            A      = Aprime;
            Aprime = A;
            F_        = F.S;
            F.S       = Fprime.S;
            Fprime.S  = F_;
            F_        = F.Z;
            F.Z       = Fprime.Z;
            Fprime.Z  = F_;
            F_        = F.X1;
            F.X1      = Fprime.X1;
            Fprime.X1 = F_;
            F_        = F.H;
            F.H       = Fprime.H;
            Fprime.H  = F_;
            F_        = F.X2;
            F.X2      = Fprime.X2;
            Fprime.X2 = F_;
            F_        = F.PV;
            F.PV      = Fprime.PV;
            Fprime.PV = F_;
            F_        = F.N;
            F.N       = Fprime.N;
            Fprime.N  = F_;
            F_        = F.C;
            F.C       = Fprime.C;
            Fprime.C  = F_;
            // Condition bits affected: None
            break;

        // EXX (0xd9)
        case 0xd9:
            F_     = B;
            B      = Bprime;
            Bprime = F_;
            F_     = C;
            C      = Cprime;
            Cprime = F_;
            F_     = D;
            D      = Dprime;
            Dprime = F_;
            F_     = E;
            E      = Eprime;
            Eprime = F_;
            F_     = H;
            H      = Hprime;
            Hprime = F_;
            F_     = L;
            L      = Lprime;
            Lprime = F_;
            // Condition bits affected: None
            break;

        // EX (SP), HL (0xe3)
        case 0xe3:
            F_           = H;
            H            = memory[SP+1];
            memory[SP+1] = F_;
            F_           = L;
            L            = memory[SP];
            memory[SP]   = F_;
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
