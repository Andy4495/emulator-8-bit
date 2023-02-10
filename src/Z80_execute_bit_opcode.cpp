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

void Z80::execute_bit_opcode() {  // IR[0] == 0xCB
    unsigned char *r = nullptr;   // Temporary storage when decoding register field in opcode
    unsigned char tempC;          // Temporarily store carry bit for rotate operations

    switch (IR[1]) {
        // RLC r     (0xCB00 - 0xCB07)
        // RLC (HL)  (0xCB06)
        case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
            // Opcode 0  0  0  0  0  r  r  r
            // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            if (*r & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            *r = *r << 1;
            *r = (*r & 0xFE) | testFlag(C_BIT);
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // RL r    (0xCB10 - 0xCB17)
        // RL (HL)  (0xCB06)
        case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
            // Opcode 0  0  0  1  0  r  r  r
            // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            tempC = testFlag(C_BIT);
            if (*r & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            *r = *r << 1;
            *r = (*r & 0xFE) | tempC;
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // RRC r     (0xCB08 - 0xCB0f)
        // RRC (HL)  (0xCB0e)
        case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f:
            // Opcode 0  0  0  0  1  r  r  r
            // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            if (*r & 0x01) setFlag(C_BIT); else clearFlag(C_BIT);
            *r = *r >> 1;
            *r = (*r & 0x7f) | (testFlag(C_BIT) << 7);
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // RR  r    (0xCB18 - 0xCB1F)
        // RR (HL)  (0xCB1E)
        case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f:
            // Opcode 0  0  0  1  1  r  r  r
            // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            tempC = testFlag(C_BIT);
            if (*r & 0x01) setFlag(C_BIT); else clearFlag(C_BIT);
            *r = *r >> 1;
            *r = (*r & 0x7F) | (tempC << 7);
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // SLA r     (0xCB20 - 0xCB27)
        // SLA (HL)  (0xCB26)
        case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
            // Opcode 0  0  1  0  0  r  r  r
            // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            if (*r & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            *r = *r << 1;
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // SRA  r    (0xCB28 - 0xCB2F)
        // SRA (HL)  (0xCB2E)
        case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
            // Opcode 0  0  1  0  1  r  r  r
            // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            tempC = *r & 0x80;    // Save the sign bit
            if (*r & 0x01) setFlag(C_BIT); else clearFlag(C_BIT);
            *r = *r >> 1;
            *r = (*r & 0x7F) | tempC;
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // Undocumented "Shift Left Set", sometimes listed as SLL "Shift Left Logical"
        // See article https://spectrumcomputing.co.uk/page.php?issue_id=333&page=51
        // SLS r     (0xCB30 - 0xCB37)
        // SLS (HL)  (0xCB36)
        case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
            // Opcode 0  1  1  0  0  r  r  r
            // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            if (*r & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            *r = *r << 1;
            *r |= 0x01;
            update_S(*r);
            update_Z(*r);
            clearFlag(N_BIT);
            update_P(*r);
            clearFlag(H_BIT);
            break;

        // SRL  r    (0xCB38 - 0xCB3F)
        // SRL (HL)  (0xCB3E)
        case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f:
            // Opcode 0  0  1  0  1  r  r  r
            // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            if (*r & 0x01) setFlag(C_BIT); else clearFlag(C_BIT);
            *r = *r >> 1;
            *r = (*r & 0x7F);
            clearFlag(S_BIT);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // BIT b, r (0xCB40 - 0xCB7F)
        // BIT b, (HL)  (0xX6, 0xXE)
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
        case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
        case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
        case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
        case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
        // Opcode 0  1  b  b  b  r  r  r  
        // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            // Decode the bit position, b
            if (r != nullptr) {
                if ((*r) >> (IR[1] >> ((IR[1] & 0x38) >> 3)) == 0) setFlag(Z_BIT);
                else clearFlag(Z_BIT);
                setFlag(H_BIT);
                clearFlag(N_BIT);
            }
            break;

        // SET b, r (0xCBC0 - 0xCBFF)
        // SET b, (HL)  (0xX6, 0xXE)
        case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
        case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
        case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
        case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
        case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
        case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
        case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
        case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff:
        // Opcode 1  1  b  b  b  r  r  r  
        // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            // Decode the bit position, b
            if (r != nullptr) {
                *r |= (1 << ((IR[1] & 0x38) >> 3));
            }
            break;

        // RES b, r (0xCB80 - 0xCBbF)
        // RES b, (HL)  (0xX6, 0xXE)
        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
        case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
        case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
        case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
        case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
        case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
        case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
        // Opcode 1  0  b  b  b  r  r  r  
        // Decode the register, r
            switch (IR[1] & 0x07) {
                case 0b000:
                    r = &B;
                    break;
                case 0b001:
                    r = &C;
                    break;
                case 0b010:
                    r = &D;
                    break;
                case 0b011:
                    r = &E;
                    break;
                case 0b100:
                    r = &H;
                    break;
                case 0b101:
                    r = &L;
                    break;
                case 0b110:
                    r = &memory[getHL()];
                    break;
                case 0b111:
                    r = &A;
                    break;
                default: 
                    r = nullptr;
                    cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
                    break;
            }
            // Decode the bit position, b
            if (r != nullptr) {
                *r &= ~(1 << ((IR[1] & 0x38) >> 3));
            }
            break;

        default: 
            cout << "Execution not defined: 0xcb" << hex << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}
