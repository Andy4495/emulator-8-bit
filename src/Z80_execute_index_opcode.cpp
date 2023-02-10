/* Z80 Emulator 
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
*/

// Z80 core definitions
#include "Z80.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>
#include <iomanip>

void Z80::execute_index_opcode() {  // IR[0] = 0xDD or 0xFD

    unsigned char *r = nullptr, *r_ = nullptr;   // Temporary storage when decoding register field in opcode
    unsigned char Temp;
    unsigned char* IndexH;
    unsigned char* IndexL;
    INDEX_REG idx;

    if (IR[0] == 0xdd) { IndexH = &IXH; IndexL = &IXL; idx = IX_REGISTER;}
    else               { IndexH = &IYH; IndexL = &IYL; idx = IY_REGISTER;}


    switch (IR[1]) {

        // ADD IX/Y, ss  (0x09, 0x19, 0x29, 0x39)
        case 0x09: case 0x19: case 0x29: case 0x39:
            // Determine which register we are working on:
            // Opcode 0  0  s  s  1  0  0  1
            if ((IR[1] & 0x30) == 0x30) { // Need special handling for SP since it is modeled as 16 bits instead of two 8-bit registers
                setIndexReg(idx, getIndexReg(idx) + SP);   // IX/Y += SP
            }
            else {
                switch ((IR[1] & 0x30) >> 4) {
                    case 0b00: r = &B; r_ = &C; setIndexReg(idx, getIndexReg(idx) + getBC()); break;
                    case 0b01: r = &D; r_ = &E; setIndexReg(idx, getIndexReg(idx) + getDE()); break;
                    case 0b10: r = &H; r_ = &L; setIndexReg(idx, getIndexReg(idx) + getHL()); break;
                    default: cout << "Invalid opcode: ADD IX/Y, ss" << endl; break;
                }
            }
            /// Need to implement condition bits, may need another state ///
            break;

        // LD IX/Y, nn instructions (0x21)
        case 0x21:
            *IndexH = IR[3];
            *IndexL = IR[2];
            break;

        // LD (nn), IX/Y (0x22)
        case 0x22:
            memory[(IR[2]<<8) + IR[1] + 1] = *IndexH;
            memory[(IR[2]<<8) + IR[1]]     = *IndexL;
            // Condition bits affected: None
            break;

        // INC IX/Y (0x23)
        case 0x23:
            setIndexReg(idx, getIndexReg(idx) + 1);
            // Condition bits affected: None
            break;

        // LD IX/Y, (nn)  (0x2A)
        case 0x3a: 
            *IndexH = memory[(IR[2]<<8) + IR[1] + 1];
            *IndexL = memory[(IR[2]<<8) + IR[1]];
            // Condition bits affected: None
            break;

        // DEC IX/Y (0x2B)
        case 0x2b:
            setIndexReg(idx, getIndexReg(idx) - 1);
            // Condition bits affected: None
            break;

        // INC (IX/Y + d)           (0x34)
        case 0x34:
            memory[getIndexReg(idx) + IR[2]] = memory[getIndexReg(idx) + IR[2]] + 1;
            update_S(memory[getIndexReg(idx) + IR[2]]);
            update_Z(memory[getIndexReg(idx) + IR[2]]);
            /// Implement H flag
            if (memory[getIndexReg(idx) + IR[2]] == 0x80) setFlag(PV_BIT); else clearFlag(PV_BIT);
            clearFlag(N_BIT);
            break;
            
        // DEC (IX/Y + d)           (0x35)
        case 0x35:
            memory[getIndexReg(idx) + IR[2]] = memory[getIndexReg(idx) + IR[2]] - 1;
            update_S(memory[getIndexReg(idx) + IR[2]]);
            update_Z(memory[getIndexReg(idx) + IR[2]]);
            /// Implement H flag
            if (memory[getIndexReg(idx) + IR[2]] == 0x7f) setFlag(PV_BIT); else clearFlag(PV_BIT);
            clearFlag(N_BIT);
            break;

        // LD (IX/Y + d), n         (0x36)
        case 0x36:
            memory[getIndexReg(idx) + IR[2]] = IR[3];
            break;

        // LD B, (IX/Y + d)         (0x46)
        case 0x46:
            B = memory[getIndexReg(idx) + IR[2]];
            break;

        // LD C, (IX/Y + d)         (0x4E)
        case 0x4e:
            C = memory[getIndexReg(idx) + IR[2]];
            break;

        // LD D, (IX/Y + d)         (0x56)
        case 0x56:
            D = memory[getIndexReg(idx) + IR[2]];
            break;
           
        // LD E, (IX/Y + d)         (0x5E)
        case 0x5e:
            E = memory[getIndexReg(idx) + IR[2]];
            break;

        // LD H, (IX/Y + d)         (0x66)
        case 0x66:
            H = memory[getIndexReg(idx) + IR[2]];
            break;

        // LD L, (IX/Y + d)         (0x6E)
        case 0x6e:
            L = memory[getIndexReg(idx) + IR[2]];
            break;

        // LD (IX/Y + d), B         (0x70)
        case 0x70:
            memory[getIndexReg(idx) + IR[2]] = B;
            break;

        // LD (IX/Y + d), B         (0x71)
        case 0x71:
            memory[getIndexReg(idx) + IR[2]] = C;
            break;

        // LD (IX/Y + d), B         (0x72)
        case 0x72:
            memory[getIndexReg(idx) + IR[2]] = D;
            break;

        // LD (IX/Y + d), B         (0x73)
        case 0x73:
            memory[getIndexReg(idx) + IR[2]] = E;
            break;

        // LD (IX/Y + d), B         (0x74)
        case 0x74:
            memory[getIndexReg(idx) + IR[2]] = H;
            break;

        // LD (IX/Y + d), B         (0x75)
        case 0x75:
            memory[getIndexReg(idx) + IR[2]] = L;
            break;

        // LD (IX/Y + d), B         (0x77)
        case 0x77:
            memory[getIndexReg(idx) + IR[2]] = A;
            break;

        // LD (IX/Y + d), B         (0x7E)
        case 0x7e:
            A = memory[getIndexReg(idx) + IR[2]];
            break;

        // ADD A, (IX/Y + d)         (0x86)
        case 0x86:
            update_V(ADD, A, memory[getIndexReg(idx) + IR[2]]);
            update_C(ADD, A, memory[getIndexReg(idx) + IR[2]]);
            A += memory[getIndexReg(idx) + IR[2]];
            update_S(A);
            update_Z(A);
            /// Implement H flag
            clearFlag(N_BIT);
            break;

        // ADC A, (IX/Y + d)         (0x8E)
        case 0x8e:
            Temp = testFlag(C_BIT); // Save the carry bit
            update_V(ADC, A, memory[getIndexReg(idx) + IR[2]]);
            update_C(ADC, A, memory[getIndexReg(idx) + IR[2]]);
            A += memory[getIndexReg(idx) + IR[2]] + Temp;
            update_S(A);
            update_Z(A);
            /// Implement H flag
            clearFlag(N_BIT);
            break;

        // SUB A, (IX/Y + d)         (0x96)
        case 0x96:
            update_C(SUB, A, memory[getIndexReg(idx) + IR[2]]);
            update_V(SUB, A, memory[getIndexReg(idx) + IR[2]]);
            A += memory[getIndexReg(idx) + IR[2]];
            update_S(A);
            update_Z(A);
            /// implemenet H bit
            setFlag(N_BIT);
            break;

        // SBC A, (IX/Y + d)         (0x9E)
        case 0x9e:
            Temp = testFlag(C_BIT); // Save the carry bit
            update_V(SBC, A, memory[getIndexReg(idx) + IR[2]]);
            update_C(SBC, A, memory[getIndexReg(idx) + IR[2]]);
            A += memory[getIndexReg(idx) + IR[2]] + Temp;
            update_S(A);
            update_Z(A);
            /// Implement H flag
            setFlag(N_BIT);
            break;

        // AND A, (IX/Y + d)         (0xA6)
        case 0xa6:
            A &= memory[getIndexReg(idx) + IR[2]];
            update_S(A);
            update_Z(A);
            setFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // XOR A, (IX/Y + d)         (0xAE)
        case 0xae:
            A ^= memory[getIndexReg(idx) + IR[2]];
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // OR A, (IX/Y + d)         (0xB6)
        case 0xb6:
            A |= memory[getIndexReg(idx) + IR[2]];
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            clearFlag(C_BIT);
            break;

        // CP A, (IX/Y + d)         (0xBE)
        case 0xbe:
            update_S(A - memory[getIndexReg(idx) + IR[2]]);
            update_Z(A - memory[getIndexReg(idx) + IR[2]]);
            /// Need to implement H flag
            update_V(SUB, A, memory[getIndexReg(idx) + IR[2]]);
            setFlag(N_BIT);
            update_C(SUB, A, memory[getIndexReg(idx) + IR[2]]);
            break;

        // POP IX/Y     (0xE1)
        case 0xe1:
            *IndexH  = memory[SP++];
            *IndexL = memory[SP++];
            // Condition bits affected: None
            break;   

        // EX (SP), IX/Y (0xE3)
        case 0xe3:
            Temp         = *IndexH;
            *IndexH          = memory[SP+1];
            memory[SP+1] = Temp;
            Temp         = *IndexL;
            *IndexL          = memory[SP];
            memory[SP]   = Temp;
            // Condition bits affected: None
            break;

        // PUSH IX/Y     (0xE5)
        case 0xe5:
            memory[--SP] = *IndexH;
            memory[--SP] = *IndexL;
            // Condition bits affected: None
            break;      

        // JP (IX/Y)    (0xE9)
        case 0xe9:
            setPC(*IndexH, *IndexL);
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
        case 0x04: case 0x0c: case 0x14: case 0x1c: case 0x24: case 0x2c:       case 0x3c:
            // Opcode 0  0  r  r  r  1  0  0 
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: INC r" << endl; break;
            }
            (*r)++;
            update_S(*r);
            update_Z(*r);
            /// Implement H flag
            if (*r == 0x80) setFlag(PV_BIT); else clearFlag(PV_BIT);
            clearFlag(N_BIT);
            break;

        // DEC r instructions (0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D)
        case 0x05: case 0x0d: case 0x15: case 0x1d: case 0x25: case 0x2d:       case 0x3d:
            // Opcode 0  0  r  r  r  1  0  1 
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: DEC r" << endl; break;
            }
            (*r)--;
            update_S(*r);
            update_Z(*r);
            /// Implement H flag
            if (*r == 0x7f) setFlag(PV_BIT); else clearFlag(PV_BIT);
            clearFlag(N_BIT);
            break;

        // LD r, n instructions (0x06/0x0e - 0x26/0x3e)
        case 0x06: case 0x0e: case 0x16: case 0x1e: case 0x26: case 0x2e:       case 0x3e:
            // Opcode 0  0  r  r  r  1  1  0
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: LD r, n" << endl; break;
            }
            *r = IR[2];        // LD r, n
            // Condition bits affected: None
            break;

        // LD r, r' instructions (0x40 - 0x7f, not including 0xX6, 0xXE, 0x70 - 0x77)
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45:       case 0x47:
        case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d:       case 0x4f:
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55:       case 0x57:
        case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d:       case 0x5f:
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65:       case 0x67:
        case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d:       case 0x6f:
        // 0x70 - 0x77 implemented separately
        case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d:       case 0x7f:
            // Determine which registers we are working on:
            // Opcode 0  1  r  r  r  r' r' r'
            switch (IR[1] & 0x07) {
                case 0b000: r_ = &B; break;
                case 0b001: r_ = &C; break;
                case 0b010: r_ = &D; break;
                case 0b011: r_ = &E; break;
                case 0b100: r_ = IndexH; break;
                case 0b101: r_ = IndexL; break;
                case 0b111: r_ = &A; break;
                default: cout << "Invalid opcode: LD r, r' r'" << endl; break;
            }
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: LD r, r' r" << endl; break;
            }
            *r = *r_;        // LD r, r'
            // Condition bits affected: None
            break;

        // ADD A, r instructions (0x80 - 0x85, 0x87)
        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85:       case 0x87:
            // Opcode 1  0  0  0  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: ADD A, r" << endl; break;
            }
            update_V(ADD, A, *r);
            update_C(ADD, A, *r);
            A += *r;
            update_S(A);
            update_Z(A);
            /// Implement H flag
            clearFlag(N_BIT);
            break;

        // ADC A, r instructions (0x88 - 0x8D, 0x8F)
        case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d:       case 0x8f:
            // Opcode 1  0  0  0  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: ADC A, r" << endl; break;
            }
            Temp = testFlag(C_BIT);
            update_V(ADC, A, *r);
            update_C(ADC, A, *r);
            A = A + *r + Temp;
            update_S(A);
            update_Z(A);
            /// Implement H Flag
            clearFlag(N_BIT);
            break;

        // SUB A, r instructions (0x90 - 0x95, 0x97)
        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95:       case 0x97:
            // Opcode 1  0  0  1  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: SUB A, r" << endl; break;
            }
            update_C(SUB, A, *r);
            update_V(SUB, A, *r);
            A = A - *r;
            update_S(A);
            update_Z(A);
            /// Implement H bit
            setFlag(N_BIT);
            break;

        // SBC A, r instructions (0x98 - 0x9D, 0x9F)
        case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d:       case 0x9f:
            // Opcode 1  0  0  1  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: SBC A, r" << endl; break;
            }
            Temp = testFlag(C_BIT);
            update_V(SBC, A, *r);
            update_C(SBC, A, *r);
            A = A - *r - Temp;
            update_S(A);
            update_Z(A);
            /// Need to implement H flag
            setFlag(N_BIT);
            break;

        // AND A, r instructions (0xA0 - 0xA5, 0xA7)
        case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5:       case 0xa7:
            // Opcode 1  0  1  0  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
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

        // XOR A, r instructions (0xA8 - 0xAD, 0xAF)
        case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad:       case 0xaf:
            // Opcode 1  0  1  0  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
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

        // OR A, r instructions (0xB0 - 0xB5, 0xB7)
        case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5:       case 0xb7:
            // Opcode 1  0  1  1  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
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

        // CP r instructions (0xB8 - 0xBD, 0xBF)
        case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd:       case 0xbf:
            // Opcode 1  0  1  1  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = IndexH; break;
                case 0b101: r = IndexL; break;
                case 0b110: r = &memory[getHL()]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: CP r" << endl; break;
            }
            // Compare only; register contents unchanged
            update_S(A - *r);
            update_Z(A - *r);
            /// Implement H flag
            update_V(SUB, A, *r);
            setFlag(N_BIT);
            update_C(SUB, A, *r);
            break;

        default: 
            cout << "Execution not defined: ";
            if (idx == IX_REGISTER) cout << "0xdd";
            else                    cout << "oxfd";
            cout << hex << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}

