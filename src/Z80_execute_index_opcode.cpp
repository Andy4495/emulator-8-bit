/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   v0.1.0    02/11/23  Andy4495 Read for first "release"
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
   0.3.0 04/16/2023 Andy4495  Tag v0.3.0
*/

#include "Z80.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
using std::cout;
using std::endl;
using std::setw;
using std::hex;

void Z80::execute_index_opcode() {  // IR[0] = 0xDD or 0xFD
    // Temporary storage when decoding register field in opcode
    uint8_t *r = nullptr, *r_ = nullptr;
    uint8_t temp;
    uint8_t* indexH;
    uint8_t* indexL;
    INDEX_REG idx;
    uint16_t disp_index;

    if (IR[0] == 0xdd) {
        indexH = &IXH;
        indexL = &IXL;
        idx = IX_REGISTER;
    } else {
        indexH = &IYH;
        indexL = &IYL;
        idx = IY_REGISTER;
    }

    // Calculate the index into memory when there is a displacement
    // value. Need to handle positive and negative displacements
    if (disp_pos)
        disp_index = getIndexReg(idx) + IR[2];
    else 
        disp_index = getIndexReg(idx) - IR[2];

    switch (IR[1]) {
        // ADD IX/Y, pp  (0x09, 0x19, 0x29, 0x39)
        case 0x09: case 0x19: case 0x29: case 0x39:
            // Determine which register we are working on:
            // Opcode 0  0  s  s  1  0  0  1
            switch ((IR[1] & 0x30) >> 4) {
                case 0b00:  // BC
                    if ((getIndexReg(idx) & 0x0fff) + (getBC() & 0x0fff)
                       > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    if ((uint32_t) getIndexReg(idx) + (uint32_t) getBC()
                       > (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setIndexReg(idx, getIndexReg(idx) + getBC());
                    break;
                case 0b01:  // DE
                    if ((getIndexReg(idx) & 0x0fff) + (getDE() & 0x0fff)
                       > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    if ((uint32_t) getIndexReg(idx) + (uint32_t) getDE()
                       > (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setIndexReg(idx, getIndexReg(idx) + getDE());
                    break;
                case 0b10:  // IX/Y
                    if ((getIndexReg(idx) & 0x0fff) +
                        (getIndexReg(idx) & 0x0fff) > 0xfff)
                            setFlag(H_BIT);
                    else
                            clearFlag(H_BIT);
                    if ((uint32_t) getIndexReg(idx) +
                        (uint32_t) getIndexReg(idx) > (uint32_t) 0xffff)
                            setFlag(C_BIT);
                    else
                            clearFlag(C_BIT);
                    setIndexReg(idx, getIndexReg(idx) + getIndexReg(idx));
                    break;
                case 0b11:  // SP
                    if ((getIndexReg(idx) & 0x0fff) + (SP & 0x0fff) > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    if ((uint32_t) getIndexReg(idx) + (uint32_t) SP
                       > (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setIndexReg(idx, getIndexReg(idx) + SP);
                    break;
                default: cout << "Invalid opcode: ADD IX/Y, ss" << endl; break;
            }
            clearFlag(N_BIT);
            break;

        // LD IX/Y, nn instructions (0x21)
        case 0x21:
            *indexH = IR[3];
            *indexL = IR[2];
            // Condition bits affected: None
            break;

        // LD (nn), IX/Y (0x22)
        case 0x22:
            memory[(IR[3] << 8) + IR[2] + 1] = *indexH;
            memory[(IR[3] << 8) + IR[2]]     = *indexL;
            // Condition bits affected: None
            break;

        // INC IX/Y (0x23)
        case 0x23:
            setIndexReg(idx, getIndexReg(idx) + 1);
            // Condition bits affected: None
            break;

        // LD IX/Y, (nn)  (0x2A)
        case 0x2a:
            *indexH = memory[(IR[3] << 8) + IR[2] + 1];
            *indexL = memory[(IR[3] << 8) + IR[2]];
            // Condition bits affected: None
            break;

        // DEC IX/Y (0x2B)
        case 0x2b:
            setIndexReg(idx, getIndexReg(idx) - 1);
            // Condition bits affected: None
            break;

        // INC (IX/Y + d)           (0x34)
        case 0x34:
            update_H(ADD, memory[disp_index], 1);
            memory[disp_index]
                = memory[disp_index] + 1;
            update_S(memory[disp_index]);
            update_Z(memory[disp_index]);
            if (memory[disp_index] == 0x80)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            clearFlag(N_BIT);
            break;

        // DEC (IX/Y + d)           (0x35)
        case 0x35:
            update_H(SUB, memory[disp_index], 1);
            memory[disp_index]
                = memory[disp_index] - 1;
            update_S(memory[disp_index]);
            update_Z(memory[disp_index]);
            if (memory[disp_index] == 0x7f)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            clearFlag(N_BIT);
            break;

        // LD (IX/Y + d), n         (0x36)
        case 0x36:
            memory[disp_index] = IR[3];
            // Condition bits affected: None
            break;

        // LD B, (IX/Y + d)         (0x46)
        case 0x46:
            B = memory[disp_index];
            // Condition bits affected: None
            break;

        // LD C, (IX/Y + d)         (0x4E)
        case 0x4e:
            C = memory[disp_index];
            // Condition bits affected: None
            break;

        // LD D, (IX/Y + d)         (0x56)
        case 0x56:
            D = memory[disp_index];
            // Condition bits affected: None
            break;

        // LD E, (IX/Y + d)         (0x5E)
        case 0x5e:
            E = memory[disp_index];
            // Condition bits affected: None
            break;

        // LD H, (IX/Y + d)         (0x66)
        case 0x66:
            H = memory[disp_index];
            // Condition bits affected: None
            break;

        // LD L, (IX/Y + d)         (0x6E)
        case 0x6e:
            L = memory[disp_index];
            // Condition bits affected: None
           break;

        // LD (IX/Y + d), B         (0x70)
        case 0x70:
            memory[disp_index] = B;
            // Condition bits affected: None
            break;

        // LD (IX/Y + d), B         (0x71)
        case 0x71:
            memory[disp_index] = C;
            // Condition bits affected: None
            break;

        // LD (IX/Y + d), B         (0x72)
        case 0x72:
            memory[disp_index] = D;
            // Condition bits affected: None
            break;

        // LD (IX/Y + d), B         (0x73)
        case 0x73:
            memory[disp_index] = E;
            // Condition bits affected: None
            break;

        // LD (IX/Y + d), B         (0x74)
        case 0x74:
            memory[disp_index] = H;
            // Condition bits affected: None
            break;

        // LD (IX/Y + d), B         (0x75)
        case 0x75:
            memory[disp_index] = L;
            // Condition bits affected: None
            break;

        // LD (IX/Y + d), B         (0x77)
        case 0x77:
            memory[disp_index] = A;
            // Condition bits affected: None
            break;

        // LD (IX/Y + d), B         (0x7E)
        case 0x7e:
            A = memory[disp_index];
            // Condition bits affected: None
            break;

        // ADD A, (IX/Y + d)         (0x86)
        case 0x86:
            update_V(ADD, A, memory[disp_index]);
            update_H(ADD, A, memory[disp_index]);
            update_C(ADD, A, memory[disp_index]);
            A += memory[disp_index];
            update_S(A);
            update_Z(A);
            clearFlag(N_BIT);
            break;

        // ADC A, (IX/Y + d)         (0x8E)
        case 0x8e:
            temp = testFlag(C_BIT);  // Save the carry bit
            update_V(ADC, A, memory[disp_index]);
            update_H(ADC, A, memory[disp_index]);
            update_C(ADC, A, memory[disp_index]);
            A += memory[disp_index] + temp;
            update_S(A);
            update_Z(A);
            clearFlag(N_BIT);
            break;

        // SUB A, (IX/Y + d)         (0x96)
        case 0x96:
            update_C(SUB, A, memory[disp_index]);
            update_H(SUB, A, memory[disp_index]);
            update_V(SUB, A, memory[disp_index]);
            A -= memory[disp_index];
            update_S(A);
            update_Z(A);
            setFlag(N_BIT);
            break;

        // SBC A, (IX/Y + d)         (0x9E)
        case 0x9e:
            temp = testFlag(C_BIT);  // Save the carry bit
            update_V(SBC, A, memory[disp_index]);
            update_H(SBC, A, memory[disp_index]);
            update_C(SBC, A, memory[disp_index]);
            A = A - memory[disp_index] - temp;
            update_S(A);
            update_Z(A);
            setFlag(N_BIT);
            break;

        // AND A, (IX/Y + d)         (0xA6)
        case 0xa6:
            A &= memory[disp_index];
            update_S(A);
            update_Z(A);
            setFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // XOR A, (IX/Y + d)         (0xAE)
        case 0xae:
            A ^= memory[disp_index];
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // OR A, (IX/Y + d)         (0xB6)
        case 0xb6:
            A |= memory[disp_index];
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // CP A, (IX/Y + d)         (0xBE)
        case 0xbe:
            update_S(A - memory[disp_index]);
            update_Z(A - memory[disp_index]);
            update_H(SUB, A, memory[disp_index]);
            update_V(SUB, A, memory[disp_index]);
            setFlag(N_BIT);
            update_C(SUB, A, memory[disp_index]);
            break;

        // POP IX/Y     (0xE1)
        case 0xe1:
            *indexL = memory[SP++];
            *indexH = memory[SP++];
            // Condition bits affected: None
            break;

        // EX (SP), IX/Y (0xE3)
        case 0xe3:
            temp         = *indexH;
            *indexH          = memory[SP+1];
            memory[SP+1] = temp;
            temp         = *indexL;
            *indexL          = memory[SP];
            memory[SP]   = temp;
            // Condition bits affected: None
            break;

        // PUSH IX/Y     (0xE5)
        case 0xe5:
            memory[--SP] = *indexH;
            memory[--SP] = *indexL;
            // Condition bits affected: None
            break;

        // JP (IX/Y)    (0xE9)
        case 0xe9:
            setPC(*indexH, *indexL);
            // Condition bits affected: None
            break;

        // LD SP, IX/Y   (0xF9)
        case 0xf9:
            SP = getIndexReg(idx);
            // Condition bits affected: None
            break;

        // ******************************* //
        // Undocumented instructions below //
        // ******************************* //

        // INC r instructions (0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C)
        case 0x04: case 0x0c: case 0x14: case 0x1c:
        case 0x24: case 0x2c:            case 0x3c:
            // Opcode 0  0  r  r  r  1  0  0
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: INC r" << endl; break;
            }
            if (r != nullptr) {
                if (*r == 0x7f)
                    setFlag(PV_BIT);
                else
                    clearFlag(PV_BIT);
                update_H(ADD, *r, 1);
                (*r)++;
                update_S(*r);
                update_Z(*r);
                clearFlag(N_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            }
            break;

        // DEC r instructions (0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D)
        case 0x05: case 0x0d: case 0x15: case 0x1d:
        case 0x25: case 0x2d:            case 0x3d:
            // Opcode 0  0  r  r  r  1  0  1
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: DEC r" << endl; break;
            }
            if (r != nullptr) {
                if (*r == 0x80)
                    setFlag(PV_BIT);
                else
                    clearFlag(PV_BIT);
                update_H(SUB, *r, 1);
                (*r)--;
                update_S(*r);
                update_Z(*r);
                setFlag(N_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            }
            break;

        // LD r, n instructions (0x06/0x0e - 0x26/0x3e)
        case 0x06: case 0x0e: case 0x16: case 0x1e:
        case 0x26: case 0x2e:            case 0x3e:
            // Opcode 0  0  r  r  r  1  1  0
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: LD r, n" << endl; break;
            }
            if (r != nullptr) {
                *r = IR[2];        // LD r, n
             } else {
                cout << "Error: nullptr." << endl;
            }           
            // Condition bits affected: None
            break;

        // LD r, r' instructions (0x40 - 0x7f,
        // not including 0xX6, 0xXE, 0x70 - 0x77)
        case 0x40: case 0x41: case 0x42: case 0x43:
        case 0x44: case 0x45:            case 0x47:
        case 0x48: case 0x49: case 0x4a: case 0x4b:
        case 0x4c: case 0x4d:            case 0x4f:
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55:            case 0x57:
        case 0x58: case 0x59: case 0x5a: case 0x5b:
        case 0x5c: case 0x5d:            case 0x5f:
        case 0x60: case 0x61: case 0x62: case 0x63:
        case 0x64: case 0x65:            case 0x67:
        case 0x68: case 0x69: case 0x6a: case 0x6b:
        case 0x6c: case 0x6d:            case 0x6f:
        // 0x70 - 0x77 implemented separately
        case 0x78: case 0x79: case 0x7a: case 0x7b:
        case 0x7c: case 0x7d:            case 0x7f:
            // Determine which registers we are working on:
            // Opcode 0  1  r  r  r  r' r' r'
            switch (IR[1] & 0x07) {
                case 0b000: r_ = &B; break;
                case 0b001: r_ = &C; break;
                case 0b010: r_ = &D; break;
                case 0b011: r_ = &E; break;
                case 0b100: r_ = indexH; break;
                case 0b101: r_ = indexL; break;
                case 0b111: r_ = &A; break;
                default: cout << "Invalid opcode: LD r, r' r'" << endl; break;
            }
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: LD r, r' r" << endl; break;
            }
            if (r != nullptr) {
                *r = *r_;        // LD r, r'
            } else {
                cout << "Error: nullptr." << endl;
            }
            // Condition bits affected: None
            break;

        // ADD A, r instructions (0x80 - 0x85, 0x87)
        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85:            case 0x87:
            // Opcode 1  0  0  0  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: ADD A, r" << endl; break;
            }
            if (r != nullptr) {
                update_V(ADD, A, *r);
                update_H(ADD, A, *r);
                update_C(ADD, A, *r);
                A += *r;
                update_S(A);
                update_Z(A);
                clearFlag(N_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            } 
            break;

        // ADC A, r instructions (0x88 - 0x8D, 0x8F)
        case 0x88: case 0x89: case 0x8a: case 0x8b:
        case 0x8c: case 0x8d:            case 0x8f:
            // Opcode 1  0  0  0  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: ADC A, r" << endl; break;
            }
            temp = testFlag(C_BIT);
            if (r != nullptr) {
                update_V(ADC, A, *r);
                update_H(ADC, A, *r);
                update_C(ADC, A, *r);
                A = A + *r + temp;
                update_S(A);
                update_Z(A);
                clearFlag(N_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            }            
            break;

        // SUB A, r instructions (0x90 - 0x95, 0x97)
        case 0x90: case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95:            case 0x97:
            // Opcode 1  0  0  1  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: SUB A, r" << endl; break;
            }
            if (r != nullptr) {
                update_V(SUB, A, *r);
                update_H(SUB, A, *r);
                update_C(SUB, A, *r);
                A = A - *r;
                update_S(A);
                update_Z(A);
                setFlag(N_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            }
            break;

        // SBC A, r instructions (0x98 - 0x9D, 0x9F)
        case 0x98: case 0x99: case 0x9a: case 0x9b:
        case 0x9c: case 0x9d:            case 0x9f:
            // Opcode 1  0  0  1  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: SBC A, r" << endl; break;
            }
            temp = testFlag(C_BIT);
            if (r != nullptr) {
                update_V(SBC, A, *r);
                update_H(SBC, A, *r);
                update_C(SBC, A, *r);
                A = A - *r - temp;
                update_S(A);
                update_Z(A);
                setFlag(N_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            }
            break;

        // AND A, r instructions (0xA0 - 0xA5, 0xA7)
        case 0xa0: case 0xa1: case 0xa2: case 0xa3:
        case 0xa4: case 0xa5:            case 0xa7:
            // Opcode 1  0  1  0  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: AND A, r" << endl; break;
            }
            if (r != nullptr) {
                A = A & *r;
                update_S(A);
                update_Z(A);
                setFlag(H_BIT);
                update_P(A);
                clearFlag(N_BIT);
                clearFlag(C_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            }
            break;

        // XOR A, r instructions (0xA8 - 0xAD, 0xAF)
        case 0xa8: case 0xa9: case 0xaa: case 0xab:
        case 0xac: case 0xad:            case 0xaf:
            // Opcode 1  0  1  0  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: XOR A, r" << endl; break;
            }
            if (r != nullptr) {
                A = A ^ *r;
                update_S(A);
                update_Z(A);
                clearFlag(H_BIT);
                update_P(A);
                clearFlag(N_BIT);
                clearFlag(C_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            }
            break;

        // OR A, r instructions (0xB0 - 0xB5, 0xB7)
        case 0xb0: case 0xb1: case 0xb2: case 0xb3:
        case 0xb4: case 0xb5:            case 0xb7:
            // Opcode 1  0  1  1  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: OR A, r" << endl; break;
            }
            if (r != nullptr) {
                A = A | *r;
                update_S(A);
                update_Z(A);
                clearFlag(H_BIT);
                update_P(A);
                clearFlag(N_BIT);
                clearFlag(C_BIT);
            } else {
                cout << "Error: nullptr." << endl;
            }
            break;

        // CP r instructions (0xB8 - 0xBD, 0xBF)
        case 0xb8: case 0xb9: case 0xba: case 0xbb:
        case 0xbc: case 0xbd:            case 0xbf:
            // Opcode 1  0  1  1  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = indexH; break;
                case 0b101: r = indexL; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: CP r" << endl; break;
            }
            // Compare only; register contents unchanged
            if (r != nullptr) {
                update_S(A - *r);
                update_Z(A - *r);
                update_H(SUB, A, *r);
                update_V(SUB, A, *r);
                setFlag(N_BIT);
                update_C(SUB, A, *r);
            } else {
                cout << "Error: nullptr." << endl;
            }
            break;

        default:
            // Invalid opcodes are disassembled with DEFB directives and
            // do not need special handling here. 
            break;
    }
}
