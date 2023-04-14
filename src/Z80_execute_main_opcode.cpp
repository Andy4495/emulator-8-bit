/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   v0.1.0    02/11/23  Andy4495 Read for first "release"
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
*/

#include "Z80.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <cstdint>
using std::cout;
using std::endl;
using std::setw;
using std::hex;

void Z80::execute_main_opcode() {
    // Temporary storage when decoding register field in opcode
    uint8_t *r = nullptr, *r_ = nullptr;
    uint8_t temp;
    uint8_t adjustment;  // used by DAA
    uint8_t upperN;      // used by DAA
    uint8_t lowerN;      // used by DAA

    switch (IR[0]) {
        // ************* 8-bit Load Group *************
        // ********************************************
        //
        // LD r, r' instructions (0x40 - 0x7F)
        // LD r, (HL)
        // LD (HL), r
        // Instructions with register bit pattern 0b110 represent (HL)
        // which are listed in separate subgroups in Z80 user manual,
        // but can be implemented here, with the exeption of opcode 0x76 HALT
        // which is implemented separately
        case 0x40: case 0x41: case 0x42: case 0x43:
        case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x48: case 0x49: case 0x4a: case 0x4b:
        case 0x4c: case 0x4d: case 0x4e: case 0x4f:
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55: case 0x56: case 0x57:
        case 0x58: case 0x59: case 0x5a: case 0x5b:
        case 0x5c: case 0x5d: case 0x5e: case 0x5f:
        case 0x60: case 0x61: case 0x62: case 0x63:
        case 0x64: case 0x65: case 0x66: case 0x67:
        case 0x68: case 0x69: case 0x6a: case 0x6b:
        case 0x6c: case 0x6d: case 0x6e: case 0x6f:
        case 0x70: case 0x71: case 0x72: case 0x73:
        case 0x74: case 0x75:            case 0x77:
        case 0x78: case 0x79: case 0x7a: case 0x7b:
        case 0x7c: case 0x7d: case 0x7e: case 0x7f:
            // Determine which registers we are working on:
            // Opcode 0  1  r  r  r  r' r' r'
            switch (IR[0] & 0x07) {
                case 0b000: r_ = &B; break;
                case 0b001: r_ = &C; break;
                case 0b010: r_ = &D; break;
                case 0b011: r_ = &E; break;
                case 0b100: r_ = &H; break;
                case 0b101: r_ = &L; break;
                case 0b110: r_ = &memory[getHL()]; break;  // (HL)
                case 0b111: r_ = &A; break;
                default: cout << "Invalid opcode: LD r, r'" << endl; break;
            }
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: LD r, r'" << endl; break;
            }
            *r = *r_;        // LD r, r'
            // Condition bits affected: None
            break;

        // LD r, n instructions (0x06/0x0E - 0x36/0x3E)
        // LD (HL), n           (0x36)
        // Instruction 0x36 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0x06: case 0x0e: case 0x16: case 0x1e:
        case 0x26: case 0x2e: case 0x36: case 0x3e:
            // Opcode 0  0  r  r  r  1  1  0
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;  // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: LD r, n" << endl; break;
            }
            *r = IR[1];        // LD r, n
            // Condition bits affected: None
            break;

        // LD A, (BC)  (0x0A)
        case 0x0a:
            A = memory[getBC()];
            // Condition bits affected: None
            break;

        // LD A, (DE)  (0x1A)
        case 0x1a:
            A = memory[getDE()];
            // Condition bits affected: None
            break;

        // LD A, (nn)  (0x3A)
        case 0x3a:
            A = memory[(IR[2] << 8) + IR[1]];
            // Condition bits affected: None
            break;

        // LD (BC), A  (0x02)
        case 0x02:
            memory[getBC()] = A;   // LD A, (BC)
            // Condition bits affected: None
            break;

        // LD (DE),A  (0x12)
        case 0x12:
            memory[getDE()] = A;
            // Condition bits affected: None
            break;

        // LD (nn),A  (0x32)
        case 0x32:
            memory[(IR[2] << 8) + IR[1]] = A;
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
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00: B = IR[2]; C = IR[1]; break;
                case 0b01: D = IR[2]; E = IR[1]; break;
                case 0b10: H = IR[2]; L = IR[1]; break;
                case 0b11: setSP(IR[2], IR[1]);  break;
                default: cout << "Invalid opcode: LD dd, nn" << endl; break;
            }
            // Condition bits affected: None
            break;

        // LD HL, (nn)     (0x2A)
        case 0x2a:
                H = memory[(IR[2] << 8) + IR[1] + 1];
                L = memory[(IR[2] << 8) + IR[1]];
            // Condition bits affected: None
            break;

        // LD (nn), HL (0x22)
        case 0x22:
            memory[(IR[2] << 8) + IR[1] + 1] = H;
            memory[(IR[2] << 8) + IR[1]]     = L;
            // Condition bits affected: None
            break;

        // LD SP, HL   (0xF9)
        case 0xf9:
            SP = getHL();
            // Condition bits affected: None
            break;

        // PUSH qq     (0xC5, 0xD5, 0xE5, 0xF5)
        case 0xc5: case 0xd5: case 0xe5: case 0xf5:
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00: r = &B; r_ = &C; break;
                case 0b01: r = &D; r_ = &E; break;
                case 0b10: r = &H; r_ = &L; break;
                case 0b11: r = &A; r_ = &F; break;
                default: cout << "Invalid opcode PUSH qq" << endl; break;
            }
            memory[--SP] = *r;
            memory[--SP] = *r_;
            // Condition bits affected: None
            break;

        // POP qq     (0xC1, 0xD1, 0xE1, 0xF1)
        case 0xc1: case 0xd1: case 0xe1: case 0xf1:
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00: r = &B; r_ = &C; break;
                case 0b01: r = &D; r_ = &E; break;
                case 0b10: r = &H; r_ = &L; break;
                case 0b11: r = &A; r_ = &F; break;
                default: cout << "Invalid opcode: POP qq" << endl; break;
            }
            *r_ = memory[SP++];
            *r  = memory[SP++];
            // Condition bits affected: None
            break;

        // ************ Exchange, Block Transfer, and Search Group ************
        // Flags are not affected by these instructions
        // ********************************************************************
        //
        // EX DE, HL (0xEB)
        case 0xeb:
            temp = D;
            D  = H;
            H  = temp;
            temp = E;
            E  = L;
            L  = temp;
            // Condition bits affected: None
            break;

        // EX AF, AF'  (0x08)
        case 0x08:
            temp   = A;
            A      = Aprime;
            Aprime = temp;
            temp   = F;
            F      = Fprime;
            Fprime = temp;
            // Condition bits affected: None
            break;

        // EXX (0xD9)
        case 0xd9:
            temp     = B;
            B      = Bprime;
            Bprime = temp;
            temp     = C;
            C      = Cprime;
            Cprime = temp;
            temp     = D;
            D      = Dprime;
            Dprime = temp;
            temp     = E;
            E      = Eprime;
            Eprime = temp;
            temp     = H;
            H      = Hprime;
            Hprime = temp;
            temp     = L;
            L      = Lprime;
            Lprime = temp;
            // Condition bits affected: None
            break;

        // EX (SP), HL (0xE3)
        case 0xe3:
            temp         = H;
            H            = memory[SP+1];
            memory[SP+1] = temp;
            temp         = L;
            L            = memory[SP];
            memory[SP]   = temp;
            // Condition bits affected: None
            break;

        // ************* 8-Bit Arithmetic Group *************
        // **************************************************
        // ADD A, r instructions (0x80 - 0x85, 0x87)
        // ADD A, (HL)           (0x86)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85: case 0x86: case 0x87:
            // Opcode 1  0  0  0  0  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: ADD A, r" << endl; break;
            }
            update_V(ADD, A, *r);
            update_H(ADD, A, *r);
            update_C(ADD, A, *r);
            if ((A & 0xf) + (*r & 0xf) > 0xf)
                setFlag(H_BIT);
            else
                clearFlag(H_BIT);
            A += *r;
            update_S(A);
            update_Z(A);
            clearFlag(N_BIT);
            break;

        // ADD A, n    (0xC6)
        case 0xc6:
            update_V(ADD, A, IR[1]);
            update_H(ADD, A, IR[1]);
            update_C(ADD, A, IR[1]);
            if ((A & 0xf) + (IR[1] & 0xf) > 0xf)
                setFlag(H_BIT);
            else
                clearFlag(H_BIT);
            A += IR[1];
            update_S(A);
            update_Z(A);
            clearFlag(N_BIT);
            break;

        // ADC A, r instructions (0x88 - 0x8D, 0x8F)
        // ADC A, (HL)           (0x8E)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0x88: case 0x89: case 0x8a: case 0x8b:
        case 0x8c: case 0x8d: case 0x8e: case 0x8f:
            // Opcode 1  0  0  0  1  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: ADC A, r" << endl; break;
            }
            temp = testFlag(C_BIT);
            update_V(ADC, A, *r);
            update_H(ADC, A, *r);
            update_C(ADC, A, *r);
            if ((A & 0xf) + (*r & 0xf) + temp > 0xf)
                setFlag(H_BIT);
            else
                clearFlag(H_BIT);
            A = A + *r + temp;
            update_S(A);
            update_Z(A);
            clearFlag(N_BIT);
            break;

        // ADC A, n    (0xCE)
        case 0xce:
            temp = testFlag(C_BIT);
            update_V(ADC, A, IR[1]);
            update_H(ADC, A, IR[1]);
            update_C(ADC, A, IR[1]);
            if ((A & 0xf) + (IR[1] & 0xf) + temp > 0xf)
                setFlag(H_BIT);
            else
                clearFlag(H_BIT);
            A = A + IR[1] + temp;
            update_S(A);
            update_Z(A);
            clearFlag(N_BIT);
            break;

        // SUB A, r instructions (0x90 - 0x95, 0x97)
        // SUB A, (HL)           (0x96)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0x90: case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95: case 0x96: case 0x97:
            // Opcode 1  0  0  1  0  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;  // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: SUB A, r" << endl; break;
            }
            update_V(SUB, A, *r);
            update_H(SUB, A, *r);
            update_C(SUB, A, *r);
            A = A - *r;
            update_S(A);
            update_Z(A);
            setFlag(N_BIT);
            break;

        // SUB A, n    (0xD6)
        case 0xd6:
            update_V(SUB, A, IR[1]);
            update_H(SUB, A, IR[1]);
            update_C(SUB, A, IR[1]);
            A = A - IR[1];
            update_S(A);
            update_Z(A);
            setFlag(N_BIT);
            break;

        // SBC A, r instructions (0x98 - 0x9D, 0x9F)
        // SBC A, (HL)           (0x9E)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0x98: case 0x99: case 0x9a: case 0x9b:
        case 0x9c: case 0x9d: case 0x9e: case 0x9f:
            // Opcode 1  0  0  1  1  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: SBC A, r" << endl; break;
            }
            temp = testFlag(C_BIT);
            update_V(SBC, A, *r);
            update_H(SBC, A, *r);
            update_C(SBC, A, *r);
            A = A - *r - temp;
            update_S(A);
            update_Z(A);
            setFlag(N_BIT);
            break;

        // SBC A, n    (0xDE)
        case 0xde:
            temp = testFlag(C_BIT);
            update_V(SBC, A, IR[1]);
            update_H(SBC, A, IR[1]);
            update_C(SBC, A, IR[1]);
            A = A - IR[1] - temp;
            update_S(A);
            update_Z(A);
            setFlag(N_BIT);
            break;

        // AND A, r instructions (0xA0 - 0xA5, 0xA7)
        // AND A, (HL)           (0xA6)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0xa0: case 0xa1: case 0xa2: case 0xa3:
        case 0xa4: case 0xa5: case 0xa6: case 0xa7:
            // Opcode 1  0  1  0  0  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: AND A, r" << endl; break;
            }
            A = A & *r;
            update_S(A);
            update_Z(A);
            setFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // AND A, n    (0xE6)
        case 0xe6:
            A = A & IR[1];
            update_S(A);
            update_Z(A);
            setFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // OR A, r instructions (0xB0 - 0xB5, 0xB7)
        // OR A, (HL)           (0xB6)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0xb0: case 0xb1: case 0xb2: case 0xb3:
        case 0xb4: case 0xb5: case 0xb6: case 0xb7:
            // Opcode 1  0  1  1  0  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: OR A, r" << endl; break;
            }
            A = A | *r;
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // OR A, n    (0xF6)
        case 0xf6:
            A = A | IR[1];
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // XOR A, r instructions (0xA8 - 0xAD, 0xAF)
        // XOR A, (HL)           (0xAE)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0xa8: case 0xa9: case 0xaa: case 0xab:
        case 0xac: case 0xad: case 0xae: case 0xaf:
            // Opcode 1  0  1  0  1  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: XOR A, r" << endl; break;
            }
            A = A ^ *r;
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // XOR A, n    (0xEE)
        case 0xee:
            A = A ^ IR[1];
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // CP r instructions (0xB8 - 0xBD, 0xBF)
        // CP (HL)           (0xBE)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0xb8: case 0xb9: case 0xba: case 0xbb:
        case 0xbc: case 0xbd: case 0xbe: case 0xbf:
            // Opcode 1  0  1  1  1  r  r  r
            switch ((IR[0] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: CP r" << endl; break;
            }
            // Compare only; register contents unchanged
            update_S(A - *r);
            update_Z(A - *r);
            update_H(SUB, A, *r);
            update_V(SUB, A, *r);
            setFlag(N_BIT);
            update_C(SUB, A, *r);
            break;

        // CP n    (0xFE)
        case 0xfe:
            // Compare only; register contents unchanged
            update_S(A - IR[1]);
            update_Z(A - IR[1]);
            update_H(SUB, A, IR[1]);
            update_V(SUB, A, IR[1]);
            setFlag(N_BIT);
            update_C(SUB, A, IR[1]);
            break;

        // INC r instructions (0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C)
        // INC (HL)           (0x34)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0x04: case 0x0c: case 0x14: case 0x1c:
        case 0x24: case 0x2c: case 0x34: case 0x3c:
            // Opcode 0  0  r  r  r  1  0  0
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: INC r" << endl; break;
            }
            if (*r == 0x7f)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            update_H(ADD, *r, 1);
            (*r)++;
            update_S(*r);
            update_Z(*r);
            clearFlag(N_BIT);
            break;

        // DEC r instructions (0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D)
        // DEC (HL)           (0x35)
        // Instruction 0x86 with register bit pattern 0b110 represents (HL)
        // which is listed in a separate subgroup in Z80 user manual, but
        // can be implemented here
        case 0x05: case 0x0d: case 0x15: case 0x1d:
        case 0x25: case 0x2d: case 0x35: case 0x3d:
            // Opcode 0  0  r  r  r  1  0  1
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = &H; break;
                case 0b101: r = &L; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: DEC r" << endl; break;
            }
            if (*r == 0x80)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            update_H(SUB, *r, 1);
            (*r)--;
            update_S(*r);
            update_Z(*r);
            setFlag(N_BIT);
            break;

        // *** General Purpose Arithmetic and CPU Control Groups ***
        // *********************************************************

        // DAA (0x27)
        case 0x27:
            adjustment = 0;
            upperN = (A & 0xf0) >> 4;
            lowerN =  A & 0x0f;
            // Case I
            if ((testFlag(C_BIT) == 0) && (testFlag(H_BIT) == 0)
                && (upperN >= 0x0) && (upperN <= 0x9)
                && (lowerN >= 0x0) && (lowerN <= 0x9)) {
                    adjustment = 0x00;
                    clearFlag(C_BIT);
            } else
            // Case II
            if (((testFlag(C_BIT) == 0) && (testFlag(H_BIT) == 0)
                && (upperN >= 0x0) && (upperN <= 0x8)
                && (lowerN >= 0xa) && (lowerN <= 0xf)) ||
               ((testFlag(C_BIT) == 0) && (testFlag(H_BIT) == 1)
                && (upperN >= 0x0) && (upperN <= 0x9)
                && (lowerN >= 0x0) && (lowerN <= 0x3))) {
                    adjustment = 0x06;
                    clearFlag(C_BIT);
            } else
            // Case III
            if (((testFlag(N_BIT) == 0) && (testFlag(C_BIT) == 0)
                && (testFlag(H_BIT) == 0)
                && (upperN >= 0xa) && (upperN <= 0xf)
                && (lowerN >= 0x0) && (lowerN <= 0x9)) ||
               ((testFlag(C_BIT) == 1) && (testFlag(H_BIT) == 0)
                && (upperN >= 0x0) && (upperN <= 0x2)
                && (lowerN >= 0x0) && (lowerN <= 0x9))) {
                    adjustment = 0x60;
                    setFlag(C_BIT);
            } else
            // Case IV
            if (((testFlag(N_BIT) == 0) && (testFlag(C_BIT) == 0)
                && (testFlag(H_BIT) == 0)
                && (upperN >= 0x9) && (upperN <= 0xf)
                && (lowerN >= 0xa) && (lowerN <= 0xf)) ||
               ((testFlag(N_BIT) == 0) && (testFlag(C_BIT) == 0)
                && (testFlag(H_BIT) == 1)
                && (upperN >= 0xa) && (upperN <= 0xf)
                && (lowerN >= 0x0) && (lowerN <= 0x3)) ||
               ((testFlag(C_BIT) == 1) && (testFlag(H_BIT) == 0)
                && (upperN >= 0x0) && (upperN <= 0x2)
                && (lowerN >= 0xa) && (lowerN <= 0xf)) ||
               ((testFlag(C_BIT) == 1) && (testFlag(H_BIT) == 1)
                && (upperN >= 0x0) && (upperN <= 0x3)
                && (lowerN >= 0x0) && (lowerN <= 0x3))) {
                    adjustment = 0x66;
                    setFlag(C_BIT);
            } else
            // Case V
            if ( (testFlag(N_BIT) == 1) && (testFlag(C_BIT) == 0) && (testFlag(H_BIT) == 1) )
            {
                    adjustment = 0xFA;
                    clearFlag(C_BIT);
            } else
            // Case VI
            if ( (testFlag(N_BIT) == 1) && (testFlag(C_BIT) == 1) && (testFlag(H_BIT) == 0) )
            {
                    adjustment = 0xA0;
                    setFlag(C_BIT);
            } else
            // Case VII
            if ( (testFlag(N_BIT) == 1) && (testFlag(C_BIT) == 1) && (testFlag(H_BIT) == 1) )
            {
                    adjustment = 0x9A;
                    setFlag(C_BIT);
            } else {
                // Any combination beyond the above cases are not covered
                // by the operation table for the DAA instruction
                cout << "Undefined adjustment for DAA 0x27 - N:"  << hex
                     << (uint16_t) testFlag(N_BIT) << " C: "
                     << (uint16_t) testFlag(C_BIT) << " H: "
                     << (uint16_t) testFlag(H_BIT) << " A: 0x"
                     << (uint16_t) A << endl;
            }
            // H Flag update depends on N, current H value, and low-order nibble 
            if (testFlag(N_BIT) == 0) {
                if ((A & 0x0f) <= 9) clearFlag(H_BIT);
                else setFlag(H_BIT);
            } else { // N Flag is set
                if (testFlag(H_BIT) == 0) {
                    clearFlag(H_BIT);
                } else {
                    if ((A & 0x0f) <= 5) setFlag(H_BIT);
                    else clearFlag(H_BIT);
                }
            }
            A += adjustment;
            update_S(A);
            update_Z(A);
            update_P(A);
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
            } else {
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
            // Condition bits affected: None
            break;

        // HALT (0x76)
        case 0x76:
            Halt = true;
            // Halt remains in effect until an interrupt is received
            // Actual Z80 executes NOPs while halted to keep memory refreshed
            // Condition bits affected: None
            break;

        // DI  (0xF3)
        case 0xf3:
            IFF1 = 0;
            IFF2 = 0;
            // Condition bits affected: None
            break;

        // EI  (0xFB)
        case 0xfb:
            IFF1 = 1;
            IFF2 = 1;
            // Condition bits affected: None
            break;

        // ************* 16-bit Arithmetic Group *************
        // ***************************************************

        // ADD HL, ss  (0x09, 0x19, 0x29, 0x39)
        case 0x09: case 0x19: case 0x29: case 0x39:
            // Determine which register we are working on:
            // Opcode 0  0  s  s  1  0  0  1
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00:  // BC
                    if ((getHL() & 0x0fff) + (getBC() & 0x0fff) > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    if ((uint32_t) getHL() + (uint32_t) getBC()
                       > (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() + getBC());
                    break;
                case 0b01:  // DE
                    if ((getHL() & 0x0fff) + (getDE() & 0x0fff) > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    if ((uint32_t) getHL() + (uint32_t) getDE()
                       > (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() + getDE());
                    break;
                case 0b10:  // HL
                    if ((getHL() & 0x0fff) + (getHL() & 0x0fff) > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    if ((uint32_t) getHL() + (uint32_t) getHL()
                       > (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() + getHL());
                    break;
                case 0b11:  // SP
                    if ((getHL() & 0x0fff) + (SP & 0x0fff) > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    if ((uint32_t) getHL() + (uint32_t) SP > (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() + SP);
                    break;
                default: cout << "Invalid opcode: ADD HL, ss" << endl; break;
            }
            clearFlag(N_BIT);
            break;

        // INC ss (0x03, 0x013, 0x23, 0x33)
        case 0x03: case 0x13: case 0x23: case 0x33:
            // Determine which register we are working on:
            // Opcode 0  0  s  s  0  0  1  1
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00: setBC(getBC() + 1); break;
                case 0b01: setDE(getDE() + 1); break;
                case 0b10: setHL(getHL() + 1); break;
                case 0b11: SP++;               break;
                default: cout << "Invalid opcode: INC ss" << endl; break;
            }
            // Condition bits affected: None
            break;

        // DEC ss (0x0B, 0x01B, 0x2B, 0x3B)
        case 0x0b: case 0x1b: case 0x2b: case 0x3b:
            // Determine which register we are working on:
            // Opcode 0  0  s  s  1  0  1  1
            switch ((IR[0] & 0x30) >> 4) {
                case 0b00: setBC(getBC() - 1); break;
                case 0b01: setDE(getDE() - 1); break;
                case 0b10: setHL(getHL() - 1); break;
                case 0b11: SP--;               break;
                default: cout << "Invalid opcode: DEC ss" << endl; break;
            }
            // Condition bits affected: None
            break;

        // ************* Rotate and Shift Group **************
        // ***************************************************

        // RLCA (0x07)
        case 0x07:
            if (A & 0x80)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            A = A << 1;
            // Carry contains previous bit 7, so rotate that over to bit 0
            if (testFlag(C_BIT))
                A |= 0x01;
            else
                A &= 0xfe;
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // RLA (0x17)
        case 0x17:
            temp = testFlag(C_BIT);
            if (A & 0x80)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            A = A << 1;
            if (temp)
                A |= 0x01;
            else
                A &= 0xfe;
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // RRCA (0x0F)
        case 0x0f:
            if (A & 0x01)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            A = A >> 1;
            // Carry contains previous bit 0, so rotate that over to bit 7
            if (testFlag(C_BIT))
                A |= 0x80;
            else
                A &= 0x7f;
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // RRA (0x1F)
        case 0x1f:
            temp = testFlag(C_BIT);
            if (A & 0x01)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            A = A >> 1;
            if (temp)
                A |= 0x80;
            else
                A &= 0x7f;
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // ************* Jump Group **************
        // ***************************************

        // JP nn (0xC3):
        case 0xc3:
            setPC(IR[2], IR[1]);
            // Condition bits affected: None
            break;

        // JP cc, nn (0xC2, 0xCA, 0xD2, 0xDA, 0xE2, 0xEA, 0xF2, 0xFA)
        case 0xc2: case 0xCA: case 0xD2: case 0xDA:
        case 0xE2: case 0xEA: case 0xF2: case 0xFA:
            // Determine which flag to check
            // Opcode 1  1  c  c  c  0  1  0
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000:  // NZ (Z == 0)
                    if (!testFlag(Z_BIT)) setPC(IR[2], IR[1]);
                    break;
                case 0b001:  // Z (Z == 1)
                    if (testFlag(Z_BIT)) setPC(IR[2], IR[1]);
                    break;
                case 0b010:  // NC (C == 0)
                    if (!testFlag(C_BIT)) setPC(IR[2], IR[1]);
                    break;
                case 0b011:  // C (C == 1)
                    if (testFlag(C_BIT)) setPC(IR[2], IR[1]);
                    break;
                case 0b100:  // PO (PV == 0)
                    if (!testFlag(PV_BIT)) setPC(IR[2], IR[1]);
                    break;
                case 0b101:  // PE (PV == 1)
                    if (testFlag(PV_BIT)) setPC(IR[2], IR[1]);
                    break;
                case 0b110:  // P (S == 0)
                    if (!testFlag(S_BIT)) setPC(IR[2], IR[1]);
                    break;
                case 0b111:  // N (S == 1)
                    if (testFlag(S_BIT)) setPC(IR[2], IR[1]);
                    break;
                default:
                    cout << "Invalid opcode: JP cc, nn" << endl;
                    break;
            }
            // Condition bits affected: None
            break;

        // JR e (0x18)
        case 0x18:
            if (IR[1] & 0x80)
                PC = PC - (uint8_t) ~IR[1] - 1;
            else
                PC += IR[1];
            // Condition bits affected: None
            break;

        // JR C, e (0x38)
        case 0x38:
            if (testFlag(C_BIT)) {
                if (IR[1] & 0x80)
                    PC = PC - (uint8_t) ~IR[1] - 1;
                else
                    PC += IR[1];
            }
            // Condition bits affected: None
            break;

        // JR NC, e (0x30)
        case 0x30:
            if (!testFlag(C_BIT)) {
                if (IR[1] & 0x80)
                    PC = PC - (uint8_t) ~IR[1] - 1;
                else
                    PC += IR[1];
            }
            // Condition bits affected: None
            break;

        // JR Z, e (0x28)
        case 0x28:
            if (testFlag(Z_BIT)) {
                if (IR[1] & 0x80)
                    PC = PC - (uint8_t) ~IR[1] - 1;
                else
                    PC += IR[1];
            }
            // Condition bits affected: None
            break;

        // JR NZ, e (0x20)
        case 0x20:
            if (!testFlag(Z_BIT)) {
                if (IR[1] & 0x80)
                    PC = PC - (uint8_t) ~IR[1] - 1;
                else
                    PC += IR[1];
            }
            // Condition bits affected: None
            break;

        // JP (HL) (0xE9)
        case 0xe9:
            setPC(H, L);
            // Condition bits affected: None
            break;

        // DJNZ e (0x10)
        case 0x10:
            B--;
            if (B) {
                if (IR[1] & 0x80)
                    PC = PC - (uint8_t) ~IR[1] - 1;
                else
                    PC += IR[1];
            }
            // Condition bits affected: None
            break;

        // ************* Call and Return Group **************
        // **************************************************

        // CALL nn (0xCD)
        case 0xcd:
            memory[--SP] = (PC>>8);
            memory[--SP] = (PC & 0xFF);
            setPC(IR[2], IR[1]);
            // Condition bits affected: None
            break;

        // CALL cc, nn (0xC4, 0xCC, 0xD4, 0xDC, 0xe4, 0xEC, 0xF4, 0xFC)
        case 0xc4: case 0xcc: case 0xd4: case 0xdc:
        case 0xe4: case 0xec: case 0xf4: case 0xfc:
        // Determine which flag to check
        // Opcode: 1  1  c  c  c  1  0  0
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000:  // NZ (Z == 0)
                    if (!testFlag(Z_BIT)) {
                        memory[--SP] = (PC>>8);
                        memory[--SP] = (PC & 0xFF);
                        setPC(IR[2], IR[1]);
                    }
                    break;
                case 0b001:  // Z (Z == 1)
                    if (testFlag(Z_BIT)) {
                        memory[--SP] = (PC>>8);
                        memory[--SP] = (PC & 0xFF);
                        setPC(IR[2], IR[1]);
                    }
                    break;
                case 0b010:  // NC (C == 0)
                    if (!testFlag(C_BIT)) {
                        memory[--SP] = (PC>>8);
                        memory[--SP] = (PC & 0xFF);
                        setPC(IR[2], IR[1]);
                    }
                    break;
                case 0b011:  // C (C == 1)
                    if (testFlag(C_BIT)) {
                        memory[--SP] = (PC>>8);
                        memory[--SP] = (PC & 0xFF);
                        setPC(IR[2], IR[1]);
                    }
                    break;
                case 0b100:  // PO (PV == 0)
                    if (!testFlag(PV_BIT)) {
                        memory[--SP] = (PC>>8);
                        memory[--SP] = (PC & 0xFF);
                        setPC(IR[2], IR[1]);
                    }
                    break;
                case 0b101:  // PE (PV == 1)
                    if (testFlag(PV_BIT)) {
                        memory[--SP] = (PC>>8);
                        memory[--SP] = (PC & 0xFF);
                        setPC(IR[2], IR[1]);
                    }
                    break;
                case 0b110:  // P (S == 0)
                    if (!testFlag(S_BIT)) {
                        memory[--SP] = (PC>>8);
                        memory[--SP] = (PC & 0xFF);
                        setPC(IR[2], IR[1]);
                    }
                    break;
                case 0b111:  // N (S == 1)
                    if (testFlag(S_BIT)) {
                        memory[--SP] = (PC>>8);
                        memory[--SP] = (PC & 0xFF);
                        setPC(IR[2], IR[1]);
                    }
                    break;
                default:
                    cout << "Invalid opcode: CALL cc, nn" << endl;
                    break;
            }
            // Condition bits affected: None
            break;

        // RET (0xC9)
        case 0xc9:
            setPC(memory[SP+1], memory[SP]);
            SP += 2;
            // Condition bits affected: None
            break;

        // RET cc (0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8)
        case 0xc0: case 0xc8: case 0xd0: case 0xd8:
        case 0xe0: case 0xe8: case 0xf0: case 0xf8:
        // Determine which flag to check
        // Opcode: 1  1  c  c  c  0  0  0
            switch ((IR[0] & 0x38) >> 3) {
                case 0b000:  // NZ (Z == 0)
                    if (!testFlag(Z_BIT))
                        {setPC(memory[SP+1], memory[SP]); SP += 2;}
                    break;
                case 0b001:  // Z (Z == 1)
                    if (testFlag(Z_BIT))
                        {setPC(memory[SP+1], memory[SP]); SP += 2;}
                    break;
                case 0b010:  // NC (C == 0)
                    if (!testFlag(C_BIT))
                        {setPC(memory[SP+1], memory[SP]); SP += 2;}
                    break;
                case 0b011:  // C (C == 1)
                    if (testFlag(C_BIT))
                        {setPC(memory[SP+1], memory[SP]); SP += 2;}
                    break;
                case 0b100:  // PO (PV == 0)
                    if (!testFlag(PV_BIT))
                        {setPC(memory[SP+1], memory[SP]); SP += 2;}
                    break;
                case 0b101:  // PE (PV == 1)
                    if (testFlag(PV_BIT))
                        {setPC(memory[SP+1], memory[SP]); SP += 2;}
                    break;
                case 0b110:  // P (S == 0)
                    if (!testFlag(S_BIT))
                        {setPC(memory[SP+1], memory[SP]); SP += 2;}
                    break;
                case 0b111:  // N (S == 1)
                    if (testFlag(S_BIT))
                        {setPC(memory[SP+1], memory[SP]); SP += 2;;}
                    break;
                default:
                    cout << "Invalid opcode: RET cc" << endl;
                    break;
            }
            // Condition bits affected: None
            break;

        // RST p  (0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF)
        case 0xc7: case 0xcf: case 0xd7: case 0xdf:
        case 0xe7: case 0xef: case 0xf7: case 0xff:
            memory[--SP] = (PC>>8);
            memory[--SP] = (PC & 0xFF);
            PC = 0x8 * ((IR[0] & 0x38) >> 3);
            // Condition bits affected: None
            break;

        // ************* Input and Output Group **************
        // **************************************************
        // IN A, (n) (0xDB)
        case 0xdb:
            A = in[IR[1]];
            // Condition bits affected: None
            break;

        // OUT (n), A (0xD3)
        case 0xd3:
            out[IR[1]] = A;
            // Condition bits affected: None
            break;

        default:
            // Invalid opcodes are disassembled with DEFB directives and
            // do not need special handling here. 
            break;
    }
}
