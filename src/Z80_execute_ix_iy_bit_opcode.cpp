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
using std::cout;
using std::endl;
using std::setw;
using std::hex;

// IX: IR[0,1,2] = 0xDDCBdd, IY: IR[0,1,2] = 0xFDCBdd
void Z80::execute_ix_iy_bit_opcode() {
    // Temporary storage when decoding register field in opcode
    uint8_t *r = nullptr;
    uint8_t temp;
    // Index into memory[] array based on IX or IY plus displacement in opcode
    uint8_t index;

    if (IR[0] == 0xDD) index = getIX() + IR[2];
    else               index = getIY() + IR[2];

    switch (IR[3]) {
        // RLC (IX/IY + d), r     (0xXDCB00dd - 0xXDCB07dd)
        case 0x00: case 0x01: case 0x02: case 0x03:
        case 0x04: case 0x05: case 0x06: case 0x07:
            // Opcode 0  0  0  0  0  r  r  r
            // Decode the register, r
            switch (IR[3] & 0x07) {
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
                case 0b110:         // Doesn't put results in a register
                    r = nullptr;
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            if (memory[index] & 0x80)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            // Carry flag contains previous bit 7, so rotate into bit 0
            memory[index] = ((memory[index] << 1) & 0xfe) | testFlag(C_BIT);
            if (r != nullptr) {
                *r = memory[index];
            }
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // RL (IX/IY + d), r    (0xXDCBdd10 - 0xXDCBdd17)
        case 0x10: case 0x11: case 0x12: case 0x13:
        case 0x14: case 0x15: case 0x16: case 0x17:
            // Opcode 0  0  0  1  0  r  r  r
            // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;    // Doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            temp = testFlag(C_BIT);

            if (memory[index] & 0x80)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            memory[index] = ((memory[index] << 1) & 0xfe) | temp;
            if (r != nullptr) {
                *r = memory[index];
            }
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // RRC (IX/IY + d), r     (0xXDCBdd08 - 0xXDCBdd0f)
        case 0x08: case 0x09: case 0x0a: case 0x0b:
        case 0x0c: case 0x0d: case 0x0e: case 0x0f:
            // Opcode 0  0  0  0  1  r  r  r
            // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;     // Doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            if (memory[index] & 0x01)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            memory[index] = ((memory[index] >> 1) & 0x7f)
                            | (testFlag(C_BIT) << 7);
            if (r != nullptr) {
                *r = memory[index];
            }
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // RR  (IX/IY + d), r    (0xXDCBdd18 - 0xXDCBdd1F)
        case 0x18: case 0x19: case 0x1a: case 0x1b:
        case 0x1c: case 0x1d: case 0x1e: case 0x1f:
            // Opcode 0  0  0  1  1  r  r  r
            // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;  // This one doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            temp = testFlag(C_BIT);
            if (memory[index] & 0x01)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            memory[index] = ((memory[index] << 1) & 0x7f) | (temp << 7);
            if (r != nullptr) {
                *r = memory[index];
            }
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // SLA (IX/IY + d), r     (0xXDCBdd20 - 0xXDCBdd27)
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x25: case 0x26: case 0x27:
            // Opcode 0  0  1  0  0  r  r  r
            // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;  // This one doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            if (memory[index] & 0x80)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            memory[index] = memory[index] << 1;
            if (r != nullptr) {
                *r = memory[index];
            }
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // SRA  (IX/IY + d), r    (0xXDCBdd28 - 0xXDCBdd2F)
        case 0x28: case 0x29: case 0x2a: case 0x2b:
        case 0x2c: case 0x2d: case 0x2e: case 0x2f:
            // Opcode 0  0  1  0  1  r  r  r
            // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;  // This one doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            temp = memory[index] & 0x80;    // Save the sign bit
            if (memory[index] & 0x01)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            memory[index] = ((memory[index] >> 1) & 0x7f) | temp;
            if (r != nullptr) {
                *r = memory[index];
            }
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // Undocumented "Shift Left Set", aka SLL "Shift Left Logical"
        // See article https://spectrumcomputing.co.uk/page.php?issue_id=333&page=51
        // SLS (IX/IY + d)     (0xXDCBdd30 - 0xXDCBdd37)
        case 0x30: case 0x31: case 0x32: case 0x33:
        case 0x34: case 0x35: case 0x36: case 0x37:
            // Opcode 0  1  1  0  0  r  r  r
            // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;  // This one doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            if (memory[index] & 0x80)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            memory[index] = (memory[index] << 1) | 0x01;
            if (r != nullptr) {
                *r = memory[index];
            }
            update_S(*r);
            update_Z(*r);
            clearFlag(N_BIT);
            update_P(*r);
            clearFlag(H_BIT);
            break;

        // SRL  (IX/IY + d), r    (0xXDCBdd38 - 0xXDCBdd3F)
        case 0x38: case 0x39: case 0x3a: case 0x3b:
        case 0x3c: case 0x3d: case 0x3e: case 0x3f:
            // Opcode 0  0  1  0  1  r  r  r
            // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;  // This one doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            if (memory[index] & 0x01)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            memory[index] = (memory[index] >> 1) & 0x7f;
            if (r != nullptr) {
                *r = memory[index];
            }
            clearFlag(S_BIT);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // BIT (IX/IY + d) (0xXDCBdd40 - 0xXDCBdd7F)
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
        case 0x74: case 0x75: case 0x76: case 0x77:
        case 0x78: case 0x79: case 0x7a: case 0x7b:
        case 0x7c: case 0x7d: case 0x7e: case 0x7f:
        // Opcode 0  1  b  b  b  r  r  r
        // The IX/IY bit instructions do not act on registers, so
        // don't need to decode register field (r) in opcode
        // Decode the bit position, b
            if (((memory[index]) >> (IR[3] >> ((IR[3] & 0x38) >> 3))
               & 0x01) == 0)
                setFlag(Z_BIT);
            else
                clearFlag(Z_BIT);
            setFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // SET (IX/IY + d) (0xXDCBddC0 - 0xXDCBddFF)
        case 0xc0: case 0xc1: case 0xc2: case 0xc3:
        case 0xc4: case 0xc5: case 0xc6: case 0xc7:
        case 0xc8: case 0xc9: case 0xca: case 0xcb:
        case 0xcc: case 0xcd: case 0xce: case 0xcf:
        case 0xd0: case 0xd1: case 0xd2: case 0xd3:
        case 0xd4: case 0xd5: case 0xd6: case 0xd7:
        case 0xd8: case 0xd9: case 0xda: case 0xdb:
        case 0xdc: case 0xdd: case 0xde: case 0xdf:
        case 0xe0: case 0xe1: case 0xe2: case 0xe3:
        case 0xe4: case 0xe5: case 0xe6: case 0xe7:
        case 0xe8: case 0xe9: case 0xea: case 0xeb:
        case 0xec: case 0xed: case 0xee: case 0xef:
        case 0xf0: case 0xf1: case 0xf2: case 0xf3:
        case 0xf4: case 0xf5: case 0xf6: case 0xf7:
        case 0xf8: case 0xf9: case 0xfa: case 0xfb:
        case 0xfc: case 0xfd: case 0xfe: case 0xff:
        // Opcode 1  1  b  b  b  r  r  r
        // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;  // This one doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            // Decode the bit position, b
            memory[index] |= (1 << ((IR[3] & 0x38) >> 3));
            if (r != nullptr) *r = memory[index];
            break;

        // RES (IX/IY + d) (0xXDCBdd80 - 0xXDCBddbF)
        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85: case 0x86: case 0x87:
        case 0x88: case 0x89: case 0x8a: case 0x8b:
        case 0x8c: case 0x8d: case 0x8e: case 0x8f:
        case 0x90: case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95: case 0x96: case 0x97:
        case 0x98: case 0x99: case 0x9a: case 0x9b:
        case 0x9c: case 0x9d: case 0x9e: case 0x9f:
        case 0xa0: case 0xa1: case 0xa2: case 0xa3:
        case 0xa4: case 0xa5: case 0xa6: case 0xa7:
        case 0xa8: case 0xa9: case 0xaa: case 0xab:
        case 0xac: case 0xad: case 0xae: case 0xaf:
        case 0xb0: case 0xb1: case 0xb2: case 0xb3:
        case 0xb4: case 0xb5: case 0xb6: case 0xb7:
        case 0xb8: case 0xb9: case 0xba: case 0xbb:
        case 0xbc: case 0xbd: case 0xbe: case 0xbf:
        // Opcode 1  0  b  b  b  r  r  r
        // Decode the register, r
            switch (IR[3] & 0x07) {
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
                    r = nullptr;  // This one doesn't put results in a register
                    break;
                case 0b111:
                    r = &A;
                    break;
                default:
                    r = nullptr;
                    cout << "Invalid opcode: 0xXDCBdd" << hex
                         << setw(2) << (unsigned int) IR[3] << endl;
                    break;
            }
            // Decode the bit position, b
            memory[index] &= ~(1 << ((IR[3] & 0x38) >> 3));
            if (r != nullptr) *r = memory[index];
            break;

        default:
            // Invalid opcodes are disassembled with DEFB directives and
            // do not need special handling here. 
            break;
    }
}
