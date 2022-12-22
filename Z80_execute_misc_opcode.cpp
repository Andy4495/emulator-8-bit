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

void Z80::execute_misc_opcode() {  // IR[0] = 0xED

    // All instructions in this super-group have opcodes that start with 0xED, 
    // and the specific instruction is represented by the 2nd byte of the opcode.
    // Note that there are some Z180-specific instructions in this group. However,
    // this emulator only supports Z80, so Z180 instructions are left unimplemented.

    unsigned char *r = nullptr;   // Temporary storage when decoding register field in opcode

    switch (IR[1]) {

        // Invalid opcodes, CPU executes NOP in its place
        case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
        case 0x08: case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e: case 0x0f: 
        case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
        case 0x18: case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f: 
        case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
        case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f: 
        case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
        case 0x38: case 0x39: case 0x3a: case 0x3b: case 0x3c: case 0x3d: case 0x3e: case 0x3f: 
        case 0x4c: case 0x4e: 
        case 0x54: case 0x55: case 0x5c: case 0x5d: 
        case 0x65: case 0x66: case 0x6c: case 0x6d: case 0x6e: 
        case 0x74: case 0x75: case 0x76: case 0x77: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
        case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
        case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f: 
        case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
        case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f: 
        case 0xa4: case 0xa5: case 0xa6: case 0xa7: case 0xac: case 0xad: case 0xae: case 0xaf: 
        case 0xb4: case 0xb5: case 0xb6: case 0xb7: case 0xbc: case 0xbd: case 0xbe: case 0xbf: 
        case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
        case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf: 
        case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
        case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf: 
        case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
        case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef: 
        case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
        case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff: 
            break;

        // ************* Input and Output Group **************
        // ***************************************************

        // IN r, (C)  (0xED40, 0xED48, 0xED50, 0xED58, 0xED60, 0xED68, 0xED78)
        // Note that while 0x70 is related, it is an undocumented opcdode which behaves
        // a little differently and is implemented below
        case 0x40: case 0x48: case 0x50: case 0x58: case 0x60: case 0x68: case 0x78:
            // Determine the register to use
            // Opcode: 0  1  r  r  r  0  0  0
            switch ((IR[1] & 0x38) << 3) {
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
                case 0b111:
                    r = &A;
                    break;
                default:
                    cout << "Invalid opcode: IN r, (C) 0xed70" << endl;
                    break;
            }
            *r = in[C];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, IO, *r, 0);
            break;
        
        // IN (C) (0xED70)
        // Undocumented. Reads input port pointed to by C and updates flags only; does not retain value read from port
        case 0x70:
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, IO, *r, 0);
            break;

        // INI (0xEDA2)
        case 0xA2: 
            memory[(H<<8) + L] = in[C];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;

        // INIR (0xEDB2)
        case 0xB2: 
            memory[(H<<8) + L] = in[C];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0) {
                setFlag(Z_BIT); 
            }
            else {
                clearFlag(Z_BIT);
                PC = PC - 2;
            }
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;  

        // IND (0xEDAA)
        case 0xAA:
            memory[(H<<8) + L] = in[C];
            B--;
            L--;
            if (L == 0xFF) H--;  // Decrement H if L wrapped around 0->255 (borrow)
            if (B == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;                    

        // INDR (0xEDBA)
        case 0xBA: 
            memory[(H<<8) + L] = in[C];
            B--;
            L--;
            if (L == 0xFF) H--;  // Decrement H if L wrapped around 0->255 (borrow)
            if (B == 0) {
                setFlag(Z_BIT); 
            }
            else {
                clearFlag(Z_BIT);
                PC = PC - 2;
            }
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;  

        // OUT (C), r  (0xED41, 0xED49, 0xED51, 0xED59, 0xED61, 0xED69, 0xED79)
        // Note that while 0x70 is related, it is an undocumented opcdode which behaves
        // a little differently and is implemented below
        case 0x41: case 0x49: case 0x51: case 0x59: case 0x61: case 0x69: case 0x79:
            // Determine the register to use
            // Opcode: 0  1  r  r  r  0  0  1
            switch ((IR[1] & 0x38) << 3) {
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
                case 0b111:
                    r = &A;
                    break;
                default:
                    cout << "Invalid opcode OUT (C), r  0xed71" << endl;
                    break;
            }
            out[C] = *r;
            break;
        
        // OUT (C), 0 (0xED71)
        // Undocumented. Outputs 0 to port pointed to by C. Flags unaffected.
        case 0x71:
            out[C] = 0;
            break;

        // OUTI (0xEDA3)
        case 0xA3:
            out[C] = memory[(H<<8) + L];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;        

        // OTIR (0xEDB3)
        case 0xB3: 
            out[C] = memory[(H<<8) + L];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0) {
                setFlag(Z_BIT); 
            }
            else {
                clearFlag(Z_BIT);
                PC = PC - 2;
            }
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;  

        // OUTD (0xEDAB)
        case 0xAB:
            out[C] = memory[(H<<8) + L];
            B--;
            L--;
            if (L == 0xFF) H--;  // Decrement H if L wrapped around 0->255 (borrow)
            if (B == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;                    

        // OTDR (0xEDBB)
        case 0xBB: 
            out[C] = memory[(H<<8) + L];
            B--;
            L--;
            if (L == 0xFF) H--;  // Decrement H if L wrapped around 0->255 (borrow)
            if (B == 0) {
                setFlag(Z_BIT); 
            }
            else {
                clearFlag(Z_BIT);
                PC = PC - 2;
            }
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;  

        default: 
            cout << "Execution not defined: 0xed" << hex << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}
