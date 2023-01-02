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

void Z80::execute_ix_opcode() {  // IR[0] = 0xDD

    unsigned char *r = nullptr, *r_ = nullptr;   // Temporary storage when decoding register field in opcode
    unsigned char Temp;
    unsigned short Temp16;

    switch (IR[1]) {

        // ADD IX, ss  (0x09, 0x19, 0x29, 0x39)
        case 0x09: case 0x19: case 0x29: case 0x39:
            // Determine which register we are working on:
            // Opcode 0  0  s  s  1  0  0  1
            if ((IR[1] & 0x30) == 0x30) { // Need special handling for SP since it is modeled as 16 bits instead of two 8-bit registers
                IX += SP;
            }
            else {
                switch ((IR[1] & 0x30) >> 4) {
                    case 0b00: r = &B; r_ = &C; break;
                    case 0b01: r = &D; r_ = &E; break;
                    case 0b10: r = &H; r_ = &L; break;
                    default: cout << "Invalid opcode: ADD IX, ss" << endl; break;
                }
                IX += (*r<<8) + *r_;
            }
            /// Need to implement condition bits, may need another state ///
            break;

        // LD IX, nn instructions (0x21)
        case 0x21:
            IX = (IR[3]<<8) + IR[2];
            break;

        // INC r instructions (0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C)
        case 0x04: case 0x0c: case 0x14: case 0x1c: case 0x24: case 0x2c:       case 0x3C:
            // Opcode 0  0  r  r  r  1  0  0 
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: INC r" << endl; break;
            }
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, ADD, *r, 1);
            (*r)++;
            break;

        // DEC r instructions (0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D)
        case 0x05: case 0x0d: case 0x15: case 0x1d: case 0x25: case 0x2d:       case 0x3d:
            // Opcode 0  0  r  r  r  1  0  1 
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: DEC r" << endl; break;
            }
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SUB, *r, 1);
            (*r)--;
            break;

        // LD r, n instructions (0x06/0x0e - 0x26/0x3e)
        case 0x06: case 0x0e: case 0x16: case 0x1e: case 0x26: case 0x2e:       case 0x3e:
            // Opcode 0  0  r  r  r  1  1  0
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
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
                case 0b100: r_ = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r_ = (unsigned char*) &IX; break;         // IXL
                case 0b111: r_ = &A; break;
                default: cout << "Invalid opcode: LD r, r' r'" << endl; break;
            }
            switch ((IR[1] & 0x38) >> 3) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
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
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: ADD A, r" << endl; break;
            }
            A += *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, ADD, A, *r);
            break;

        // ADC A, r instructions (0x88 - 0x8D, 0x8F)
        case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d:       case 0x8f:
            // Opcode 1  0  0  0  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: ADC A, r" << endl; break;
            }
            A = A + *r + testFlag(C_BIT);
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, ADC, A, *r);
            break;

        // SUB A, r instructions (0x90 - 0x95, 0x97)
        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95:       case 0x97:
            // Opcode 1  0  0  1  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: SUB A, r" << endl; break;
            }
            A = A - *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SUB, A, *r);
            break;

        // SBC A, r instructions (0x98 - 0x9D, 0x9F)
        case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d:       case 0x9f:
            // Opcode 1  0  0  1  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: SBC A, r" << endl; break;
            }
            A = A - *r - testFlag(C_BIT);
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SBC, A, *r);
            break;

        // AND A, r instructions (0xA0 - 0xA5, 0xA7)
        case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5:       case 0xa7:
            // Opcode 1  0  1  0  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: AND A, r" << endl; break;
            }
            A = A & *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, AND, A, *r);
            break;

        // XOR A, r instructions (0xA8 - 0xAD, 0xAF)
        case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad:       case 0xaf:
            // Opcode 1  0  1  0  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: XOR A, r" << endl; break;
            }
            A = A ^ *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, XOR, A, *r);
            break;

        // OR A, r instructions (0xB0 - 0xB5, 0xB7)
        case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5:       case 0xb7:
            // Opcode 1  0  1  1  0  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: OR A, r" << endl; break;
            }
            A = A | *r;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, OR, A, *r);
            break;

        // CP r instructions (0xB8 - 0xBD, 0xBF)
        case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd:       case 0xbf:
            // Opcode 1  0  1  1  1  r  r  r
            switch ((IR[1] & 0x07)) {
                case 0b000: r = &B; break;
                case 0b001: r = &C; break;
                case 0b010: r = &D; break;
                case 0b011: r = &E; break;
                case 0b100: r = (unsigned char*) &IX + 1; break;     // IXH
                case 0b101: r = (unsigned char*) &IX; break;         // IXL
                case 0b110: r = &memory[(H<<8) + L]; break;   // (HL)
                case 0b111: r = &A; break;
                default: cout << "Invalid opcode: CP r" << endl; break;
            }
            // Compare only; register contents unchanged
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT|C_BIT, SUB, A, *r);
            break;

        default: 
            cout << "Execution not defined: 0xdd" << hex << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}

