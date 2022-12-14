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

    unsigned char *r  = nullptr;   // Temporary storage when decoding register field in opcode
    unsigned char *r_ = nullptr;
    unsigned short Temp16;
    unsigned char  Temp8;

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
        case 0xa2: 
            memory[getHL()] = in[C];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;

        // INIR (0xEDB2)
        case 0xb2: 
            memory[getHL()] = in[C];
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
        case 0xaa:
            memory[getHL()] = in[C];
            B--;
            L--;
            if (L == 0xFF) H--;  // Decrement H if L wrapped around 0->255 (borrow)
            if (B == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;                    

        // INDR (0xEDBA)
        case 0xba: 
            memory[getHL()] = in[C];
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
        case 0xa3:
            out[C] = memory[getHL()];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;        

        // OTIR (0xEDB3)
        case 0xb3: 
            out[C] = memory[getHL()];
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
        case 0xab:
            out[C] = memory[getHL()];
            B--;
            L--;
            if (L == 0xFF) H--;  // Decrement H if L wrapped around 0->255 (borrow)
            if (B == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't touch them here
            break;                    

        // OTDR (0xEDBB)
        case 0xbb: 
            out[C] = memory[getHL()];
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

        // LD A, I (0xED57)
        case 0x57:
            A = I;
            update_flags(S_BIT|Z_BIT, ADD, A, 0);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            if (IFF2 == 1) setFlag(PV_BIT);
            else clearFlag(PV_BIT);
            break;

        // LD A, R (0xED5F)
        case 0x5f:
            A = R;
            update_flags(S_BIT|Z_BIT, ADD, A, 0);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            if (IFF2 == 1) setFlag(PV_BIT);
            else clearFlag(PV_BIT);
            break;

        // LD I, A (0xED47)
        case 0x47: 
            I = A;
            break;

        // LD R, A (0xED4F)
        case 0x4f: 
            R = A; 
            break;

        // LD dd, (nn) (0xED4B, 0xED5B, 0xED6B, 0xED7B)
        case 0x4b: case 0x5b: case 0x6b: case 0x7b: 
            // Determine which register we are working on:
            // Opcode 0  1  d  d  1  0  1  1
            switch ((IR[1] & 0x30) >> 4) {
                case 0b00: B = memory[(IR[3]<<8) + IR[2] + 1]; C = memory[(IR[3]<<8) + IR[2]]; break;
                case 0b01: D = memory[(IR[3]<<8) + IR[2] + 1]; E = memory[(IR[3]<<8) + IR[2]]; break;
                case 0b10: H = memory[(IR[3]<<8) + IR[2] + 1]; L = memory[(IR[3]<<8) + IR[2]]; break;
                case 0b11: 
                    SP = ((memory[(IR[3]<<8) + IR[2] + 1])<<8) + memory[(IR[3]<<8) + IR[2]];
                    break;
                default: cout << "Invalid opcode: LD  dd, (nn)" << endl; break;
            }
            break;

        // LD (nn), dd (0xED43, 0xED53, 0xED63, 0xED73)
        case 0x43: case 0x53: case 0x63: case 0x73: 
            // Determine which register we are working on:
            // Opcode 0  1  d  d  0  0  1  1
            switch ((IR[1] & 0x30) >> 4) {
                case 0b00: memory[(IR[3]<<8) + IR[2] + 1] = B; memory[(IR[3]<<8) + IR[2]] = C; break;
                case 0b01: memory[(IR[3]<<8) + IR[2] + 1] = D; memory[(IR[3]<<8) + IR[2]] = E; break;
                case 0b10: memory[(IR[3]<<8) + IR[2] + 1] = H; memory[(IR[3]<<8) + IR[2]] = L; break;
                case 0b11: 
                    memory[(IR[3]<<8) + IR[2] + 1] = (SP & 0xFF00)>>8;
                    memory[(IR[3]<<8) + IR[2]] = SP & 0xff;
                    break;
                default: cout << "Invalid opcode: LD  (nn), dd" << endl; break;
            }
            break;

        // LDI (0xEDA0)
        case 0xa0:
            memory[(D>>8) + E] = memory[(H>>8) + L];
            E++;
            if (E == 0) D++;
            L++;
            if (L == 0) H++;
            C--;
            if (C == 0xff) B--;
            if (B | C) setFlag(PV_BIT);
            else clearFlag(PV_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // LDIR (0xEDB0)
        case 0xb0:
            memory[(D>>8) + E] = memory[(H>>8) + L];
            E++;
            if (E == 0) D++;
            L++;
            if (L == 0) H++;
            C--;
            if (C == 0xff) B--;
            if (B | C) {
                setFlag(PV_BIT);
                PC -= 2;
            }
            else clearFlag(PV_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // LDD (0xEDA8)
        case 0xa8:
            memory[(D>>8) + E] = memory[(H>>8) + L];
            E--;
            if (E == 0xff) D--;
            L--;
            if (L == 0xff) H--;
            C--;
            if (C == 0xff) B--;
            if (B | C) setFlag(PV_BIT);
            else clearFlag(PV_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // LDIR (0xEDB8)
        case 0xb8:
            memory[(D>>8) + E] = memory[(H>>8) + L];
            E--;
            if (E == 0xff) D--;
            L--;
            if (L == 0xff) H--;
            C--;
            if (C == 0xff) B--;
            if (B | C) {
                setFlag(PV_BIT);
                PC -= 2;
            }
            else clearFlag(PV_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // CPI (0xEDA1)
        case 0xa1:
            update_flags(S_BIT|H_BIT|Z_BIT, SUB, A, memory[(H>>8) + L]);
            L++;
            if (L == 0) H++;
            C--;
            if (C == 0xff) B--;
            if (B | C) setFlag(PV_BIT);
            else clearFlag(PV_BIT);
            setFlag(N_BIT);
            break;

        // CPIR (0xEDB1)
        case 0xb1:
            update_flags(S_BIT|H_BIT|Z_BIT, SUB, A, memory[(H>>8) + L]);
            L++;
            if (L == 0) H++;
            C--;
            if (C == 0xff) B--;
            if (B | C) {
                setFlag(PV_BIT);
                PC -=2;
            }
            else clearFlag(PV_BIT);
            setFlag(N_BIT);
            break;
            
        // CPD (0xEDA9)
        case 0xa9:
            update_flags(S_BIT|H_BIT|Z_BIT, SUB, A, memory[(H>>8) + L]);
            L--;
            if (L == 0xff) H--;
            C--;
            if (C == 0xff) B--;
            if (B | C) setFlag(PV_BIT);
            else clearFlag(PV_BIT);
            setFlag(N_BIT);
            break;

        // CPDR (0xEDB9)
        case 0xb9:
            update_flags(S_BIT|H_BIT|Z_BIT, SUB, A, memory[(H>>8) + L]);
            L--;
            if (L == 0xff) H--;
            C--;
            if (C == 0xff) B--;
            if (B | C) {
                setFlag(PV_BIT);
                PC -=2;
            }
            else clearFlag(PV_BIT);
            setFlag(N_BIT);
            break;

        // NEG (0xED44)
        case 0x44:
            if (A == 0x80) setFlag(PV_BIT);
            else clearFlag(PV_BIT);
            A = 0 - A;
            update_flags(S_BIT|Z_BIT|H_BIT|N_BIT|C_BIT, SUB, 0, A);
            break;

        // IM 0 (0xED46)
        case 0x46: 
            INT_MODE = 0;
            break;

        // IM 1 (0xED56)
        case 0x56: 
            INT_MODE = 1;
            break;

        // IM 2 (0xED5E)
        case 0x5e: 
            INT_MODE = 2;
            break;
            
        // ADC HL, ss  (0x4A, 0x5A, 0x6A, 0x7A)
        case 0x4a: case 0x5a: case 0x6a: case 0x7a:
            Temp16 = getHL();
            // Determine which register we are working on:
            // Opcode 0  1  s  s  1  0  1  0
            switch ((IR[1] & 0x30) >> 4) {
                case 0b00: setHL(getHL() + getBC() + testFlag(C_BIT)); break;
                case 0b01: setHL(getHL() + getDE() + testFlag(C_BIT)); break;
                case 0b10: setHL(getHL() + getHL() + testFlag(C_BIT)); break;
                case 0b11: setHL(getHL() + SP      + testFlag(C_BIT)); break;
                default: cout << "Invalid opcode: ADD HL, ss" << endl; break;
            }
            /// Need to implement condition bits ///
            break;

        // SBC HL, ss  (0x42, 0x52, 0x62, 0x72)
        case 0x42: case 0x52: case 0x62: case 0x72:
            Temp16 = getHL();
            // Determine which register we are working on:
            // Opcode 0  1  s  s  0  0  1  0
            switch ((IR[1] & 0x30) >> 4) {
                case 0b00: setHL(getHL() - getBC() - testFlag(C_BIT)); break;
                case 0b01: setHL(getHL() - getDE() - testFlag(C_BIT)); break;
                case 0b10: setHL(getHL() - getHL() - testFlag(C_BIT)); break;
                case 0b11: setHL(getHL() - SP      - testFlag(C_BIT)); break;
                default: cout << "Invalid opcode: ADD HL, ss" << endl; break;
            }
            /// Need to implement condition bits ///
            break;

        // RLD (0xED6F)
        case 0x6f: 
            Temp8 = A; 
            A = (A & 0xf0) | ((memory[getHL()] & 0xf0) >> 4);
            memory[getHL()] = ((memory[getHL()] & 0x0f) << 4) + (Temp8 & 0x0f);
            if (A & 0x80) setFlag(S_BIT);
            else clearFlag(S_BIT);
            if (A == 0) setFlag(Z_BIT);
            else clearFlag(Z_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            update_flags(PV_BIT, BIT, A, 0);
            break;

        // RRD (0xED67)
        case 0x67: 
            Temp8 = A; 
            A = (A & 0xf0) | (memory[getHL()] & 0x0f);
            memory[getHL()] = ((memory[getHL()] & 0xf0) >> 4) + ((Temp8 & 0x0f) << 4);
            if (A & 0x80) setFlag(S_BIT);
            else clearFlag(S_BIT);
            if (A == 0) setFlag(Z_BIT);
            else clearFlag(Z_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            update_flags(PV_BIT, BIT, A, 0);
            break;

        // RETI (0xED4D)
        case 0x4d:
            PC = (memory[SP + 1] << 8) + memory[SP];
            SP += 2;
            break;

        // RETN (0xED45)
        case 0x45:
            PC = (memory[SP + 1] << 8) + memory[SP];
            IFF1 = IFF2;
            break;

        default: 
            cout << "Execution not defined: 0xed" << hex << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}
