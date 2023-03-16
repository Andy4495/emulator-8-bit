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

void Z80::execute_misc_opcode() {  // IR[0] = 0xED
    /*
    // All instructions in this super-group have opcodes that start with
    // 0xED, and the specific instruction is represented by the 2nd byte
    // of the opcode. Note that there are some Z180-specific instructions
    // in this group. However, this emulator only supports Z80, so Z180
    // instructions are left unimplemented.
    */

    // Temporary storage when decoding register field in opcode
    uint8_t *r  = nullptr;
    uint8_t  temp;

    switch (IR[1]) {
        // Invalid opcodes, CPU executes NOP in its place
        case 0x00: case 0x01: case 0x02: case 0x03:
        case 0x04: case 0x05: case 0x06: case 0x07:
        case 0x08: case 0x09: case 0x0a: case 0x0b:
        case 0x0c: case 0x0d: case 0x0e: case 0x0f:
        case 0x10: case 0x11: case 0x12: case 0x13:
        case 0x14: case 0x15: case 0x16: case 0x17:
        case 0x18: case 0x19: case 0x1a: case 0x1b:
        case 0x1c: case 0x1d: case 0x1e: case 0x1f:
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x25: case 0x26: case 0x27:
        case 0x28: case 0x29: case 0x2a: case 0x2b:
        case 0x2c: case 0x2d: case 0x2e: case 0x2f:
        case 0x30: case 0x31: case 0x32: case 0x33:
        case 0x34: case 0x35: case 0x36: case 0x37:
        case 0x38: case 0x39: case 0x3a: case 0x3b:
        case 0x3c: case 0x3d: case 0x3e: case 0x3f:
        case 0x4c: case 0x4e:
        case 0x54: case 0x55: case 0x5c: case 0x5d:
        case 0x65: case 0x66: case 0x6c: case 0x6d:
        case 0x6e:
        case 0x74: case 0x75: case 0x76: case 0x77:
        case 0x7c: case 0x7d: case 0x7e: case 0x7f:
        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85: case 0x86: case 0x87:
        case 0x88: case 0x89: case 0x8a: case 0x8b:
        case 0x8c: case 0x8d: case 0x8e: case 0x8f:
        case 0x90: case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95: case 0x96: case 0x97:
        case 0x98: case 0x99: case 0x9a: case 0x9b:
        case 0x9c: case 0x9d: case 0x9e: case 0x9f:
        case 0xa4: case 0xa5: case 0xa6: case 0xa7:
        case 0xac: case 0xad: case 0xae: case 0xaf:
        case 0xb4: case 0xb5: case 0xb6: case 0xb7:
        case 0xbc: case 0xbd: case 0xbe: case 0xbf:
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
            break;

        // ************* Input and Output Group **************
        // ***************************************************

        // IN r, (C)  (0xED40, 0xED48, 0xED50, 0xED58, 0xED60, 0xED68, 0xED78)
        // Note that while 0x70 is related, it is an undocumented opcdode which
        // behaves a little differently and is implemented separately
        case 0x40: case 0x48: case 0x50: case 0x58:
        case 0x60: case 0x68:            case 0x78:
            // Determine the register to use
            // Opcode: 0  1  r  r  r  0  0  0
            switch ((IR[1] & 0x38) >> 3) {
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
            update_S(*r);
            update_Z(*r);
            clearFlag(H_BIT);
            update_P(*r);
            clearFlag(N_BIT);
            break;

        // IN (C) (0xED70)
        // Undocumented. Reads input port pointed to by C and updates flags
        // only; does not retain the value read from port
        case 0x70:
            temp = in[C];
            update_S(temp);
            update_Z(temp);
            clearFlag(N_BIT);
            update_P(temp);
            clearFlag(H_BIT);
            break;

        // INI (0xEDA2)
        case 0xa2:
            memory[getHL()] = in[C];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0)
                setFlag(Z_BIT);
            else
                clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't
            // touch them here
            break;

        // INIR (0xEDB2)
        case 0xb2:
            memory[getHL()] = in[C];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0) {
                setFlag(Z_BIT);
            } else {
                clearFlag(Z_BIT);
                PC = PC - 2;
            }
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't
            // touch them here
            break;

        // IND (0xEDAA)
        case 0xaa:
            memory[getHL()] = in[C];
            B--;
            L--;
            // Decrement H if L wrapped around 0->255 (borrow)
            if (L == 0xFF) H--;
            if (B == 0)
                setFlag(Z_BIT);
            else
                clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't
            // touch them here
            break;

        // INDR (0xEDBA)
        case 0xba:
            memory[getHL()] = in[C];
            B--;
            L--;
            // Decrement H if L wrapped around 0->255 (borrow)
            if (L == 0xFF) H--;
            if (B == 0) {
                setFlag(Z_BIT);
            } else {
                clearFlag(Z_BIT);
                PC = PC - 2;
            }
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't
            // touch them here
            break;

        // OUT (C), r  (0xED41, 0xED49, 0xED51, 0xED59, 0xED61, 0xED69, 0xED79)
        // Note that while 0x70 is related, it is an undocumented opcdode which
        // behaves a little differently and is implemented separately
        case 0x41: case 0x49: case 0x51: case 0x59:
        case 0x61: case 0x69:            case 0x79:
            // Determine the register to use
            // Opcode: 0  1  r  r  r  0  0  1
            switch ((IR[1] & 0x38) >> 3) {
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
            if (B == 0)
                setFlag(Z_BIT);
            else
                clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't
            // touch them here
            break;

        // OTIR (0xEDB3)
        case 0xb3:
            out[C] = memory[getHL()];
            B--;
            L++;
            if (L == 0) H++;  // Increment H if L wrapped around 255->0 (carry)
            if (B == 0) {
                setFlag(Z_BIT);
            } else {
                clearFlag(Z_BIT);
                PC = PC - 2;
            }
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't
            // touch them here
            break;

        // OUTD (0xEDAB)
        case 0xab:
            out[C] = memory[getHL()];
            B--;
            L--;
            // Decrement H if L wrapped around 0->255 (borrow)
            if (L == 0xFF) H--;
            if (B == 0)
                setFlag(Z_BIT);
            else
                clearFlag(Z_BIT);
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't
            // touch them here
            break;

        // OTDR (0xEDBB)
        case 0xbb:
            out[C] = memory[getHL()];
            B--;
            L--;
            // Decrement H if L wrapped around 0->255 (borrow)
            if (L == 0xFF) H--;
            if (B == 0) {
                setFlag(Z_BIT);
            } else {
                clearFlag(Z_BIT);
                PC = PC - 2;
            }
            setFlag(N_BIT);
            // Per User Manual: S, H, P/V are "unknown", so we won't
            // touch them here
            break;

        // LD A, I (0xED57)
        case 0x57:
            A = I;
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            if (IFF2 == 1)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            clearFlag(N_BIT);
            break;

        // LD A, R (0xED5F)
        case 0x5f:
            A = R;
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            if (IFF2 == 1)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            clearFlag(N_BIT);
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
                case 0b00:
                    B = memory[(IR[3] << 8) + IR[2] + 1];
                    C = memory[(IR[3] << 8) + IR[2]];
                    break;
                case 0b01:
                    D = memory[(IR[3] << 8) + IR[2] + 1];
                    E = memory[(IR[3] << 8) + IR[2]];
                    break;
                case 0b10:
                    H = memory[(IR[3] << 8) + IR[2] + 1];
                    L = memory[(IR[3] << 8) + IR[2]];
                    break;
                case 0b11:
                    SP = ((memory[(IR[3] << 8) + IR[2] + 1]) << 8)
                       + memory[(IR[3] <<8) + IR[2]];
                    break;
                default: cout << "Invalid opcode: LD  dd, (nn)" << endl; break;
            }
            break;

        // LD (nn), dd (0xED43, 0xED53, 0xED63, 0xED73)
        case 0x43: case 0x53: case 0x63: case 0x73:
            // Determine which register we are working on:
            // Opcode 0  1  d  d  0  0  1  1
            switch ((IR[1] & 0x30) >> 4) {
                case 0b00:
                    memory[(IR[3] << 8) + IR[2] + 1] = B;
                    memory[(IR[3] << 8) + IR[2]] = C;
                    break;
                case 0b01:
                    memory[(IR[3] << 8) + IR[2] + 1] = D;
                    memory[(IR[3] << 8) + IR[2]] = E;
                    break;
                case 0b10:
                    memory[(IR[3] << 8) + IR[2] + 1] = H;
                    memory[(IR[3] << 8) + IR[2]] = L;
                    break;
                case 0b11:
                    memory[(IR[3] << 8) + IR[2] + 1] = (SP & 0xFF00)>>8;
                    memory[(IR[3] << 8) + IR[2]] = SP & 0xff;
                    break;
                default: cout << "Invalid opcode: LD  (nn), dd" << endl; break;
            }
            break;

        // LDI (0xEDA0)
        case 0xa0:
            memory[getDE()] = memory[getHL()];
            E++;
            if (E == 0) D++;
            L++;
            if (L == 0) H++;
            C--;
            if (C == 0xff) B--;
            if (B | C)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // LDIR (0xEDB0)
        case 0xb0:
            memory[getDE()] = memory[getHL()];
            E++;
            if (E == 0) D++;
            L++;
            if (L == 0) H++;
            C--;
            if (C == 0xff) B--;
            if (B | C) {
                setFlag(PV_BIT);
                PC -= 2;
            } else {
                clearFlag(PV_BIT);
            }
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // LDD (0xEDA8)
        case 0xa8:
            memory[getDE()] = memory[getHL()];
            E--;
            if (E == 0xff) D--;
            L--;
            if (L == 0xff) H--;
            C--;
            if (C == 0xff) B--;
            if (B | C)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // LDDR (0xEDB8)
        case 0xb8:
            memory[getDE()] = memory[getHL()];
            E--;
            if (E == 0xff) D--;
            L--;
            if (L == 0xff) H--;
            C--;
            if (C == 0xff) B--;
            if (B | C) {
                setFlag(PV_BIT);
                PC -= 2;
            } else {
                clearFlag(PV_BIT);
            }
            clearFlag(H_BIT);
            clearFlag(N_BIT);
            break;

        // CPI (0xEDA1)
        case 0xa1:
            update_S(A - memory[getHL()]);
            update_Z(A - memory[getHL()]);
            L++;
            if (L == 0) H++;
            C--;
            if (C == 0xff) B--;
            if (B | C)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            update_H(SUB, A, memory[getHL()]);
            setFlag(N_BIT);
            break;

        // CPIR (0xEDB1)
        case 0xb1:
            update_S(A - memory[getHL()]);
            update_Z(A - memory[getHL()]);
            L++;
            if (L == 0) H++;
            C--;
            if (C == 0xff) B--;
            if (B | C) {
                setFlag(PV_BIT);
            } else {
                clearFlag(PV_BIT);
            }
            update_H(SUB, A, memory[getHL()]);
            setFlag(N_BIT);
            // Continue looping until bc == 0 (PV clear) or match found (Z set)
            if (testFlag(PV_BIT) && !testFlag(Z_BIT)) {
                PC -= 2;
            }
            break;

        // CPD (0xEDA9)
        case 0xa9:
            update_S(A - memory[getHL()]);
            update_Z(A - memory[getHL()]);
            L--;
            if (L == 0xff) H--;
            C--;
            if (C == 0xff) B--;
            if (B | C)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            update_H(SUB, A, memory[getHL()]);
            setFlag(N_BIT);
            break;

        // CPDR (0xEDB9)
        case 0xb9:
            update_S(A - memory[getHL()]);
            update_Z(A - memory[getHL()]);
            L--;
            if (L == 0xff) H--;
            C--;
            if (C == 0xff) B--;
            if (B | C) {
                setFlag(PV_BIT);
            } else {
                clearFlag(PV_BIT);
            }
            setFlag(N_BIT);
            update_H(SUB, A, memory[getHL()]);
            // Continue looping until bc == 0 (PV clear) or match found (Z set)
            if (testFlag(PV_BIT) && !testFlag(Z_BIT)) {
                PC -= 2;
            }
            break;

        // NEG (0xED44)
        case 0x44:
            if (A == 0x80)
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            if (A)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            A = 0 - A;
            update_S(A);
            update_Z(A);
            update_H(SUB, 0, A);
            setFlag(N_BIT);
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
            // Determine which register we are working on:
            // Opcode 0  1  s  s  1  0  1  0
            temp = testFlag(C_BIT);
            switch ((IR[1] & 0x30) >> 4) {
                case 0b00:  // BC
                    if ((getHL() & 0x0fff) + (getBC() & 0x0fff) + temp > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    update_V16(ADC, getHL(), getBC());
                    if ((uint32_t) getHL() + (uint32_t) getBC() +
                        (uint32_t) temp >
                        (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() + getBC() + temp);
                    break;
                case 0b01:  // DE
                    if ((getHL() & 0x0fff) + (getDE() & 0x0fff) + temp > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    update_V16(ADC, getHL(), getDE());
                    if ((uint32_t) getHL() + (uint32_t) getDE() +
                        (uint32_t) temp >
                        (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() + getDE() + temp);
                    break;
                case 0b10:  // HL
                    if ((getHL() & 0x0fff) + (getHL() & 0x0fff) + temp > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    update_V16(ADC, getHL(), getHL());
                    if ((uint32_t) getHL() + (uint32_t) getHL() +
                        (uint32_t) temp >
                        (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() + getHL() + temp);
                    break;
                case 0b11:  // SP
                    if ((getHL() & 0x0fff) + (SP & 0x0fff) + temp > 0xfff)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    update_V16(ADC, getHL(), SP);
                    if ((uint32_t) getHL() + (uint32_t) SP + (uint32_t) temp >
                        (uint32_t) 0xffff)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() + SP      + temp);
                    break;
                default: cout << "Invalid opcode: ADD HL, ss" << endl; break;
            }
            if (getHL() & 0x8000)
                setFlag(S_BIT);
            else
                clearFlag(S_BIT);
            if (getHL() == 0)
                setFlag(Z_BIT);
            else
                clearFlag(Z_BIT);
            break;

        // SBC HL, ss  (0x42, 0x52, 0x62, 0x72)
        case 0x42: case 0x52: case 0x62: case 0x72:
            temp  = testFlag(C_BIT);
            // Determine which register we are working on:
            // Opcode 0  1  s  s  0  0  1  0
            switch ((IR[1] & 0x30) >> 4) {
                case 0b00:  // BC
                    if ((getHL() & 0x0fff) < (getBC() & 0x0fff) + temp)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    update_V16(SBC, getHL(), getBC());
                    if ((unsigned int) getHL() < (unsigned int) getBC() + temp)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() - getBC() - temp);
                    break;
                case 0b01:  // DE
                    if ((getHL() & 0x0fff) < (getDE() & 0x0fff) + temp)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    update_V16(SBC, getHL(), getDE());
                    if ((unsigned int) getHL() < (unsigned int) getDE() + temp)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() - getDE() - temp);
                    break;
                case 0b10:  // HL
                    if ((getHL() & 0x0fff) < (getHL() & 0x0fff) + temp)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    update_V16(SBC, getHL(), getHL());
                    if ((unsigned int) getHL() < (unsigned int) getHL() + temp)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() - getHL() - temp);
                    break;
                case 0b11:  // SP
                    if ((getHL() & 0x0fff) < (SP & 0x0fff) + temp)
                        setFlag(H_BIT);
                    else
                        clearFlag(H_BIT);
                    update_V16(SBC, getHL(), SP);
                    if ((unsigned int) getHL() < (unsigned int) SP + temp)
                        setFlag(C_BIT);
                    else
                        clearFlag(C_BIT);
                    setHL(getHL() - SP      - temp);
                    break;
                default: cout << "Invalid opcode: ADD HL, ss" << endl; break;
            }
            if (getHL() & 0x8000)
                setFlag(S_BIT);
            else
                clearFlag(S_BIT);
            if (getHL() == 0)
                setFlag(Z_BIT);
            else
                clearFlag(Z_BIT);
            break;

        // RLD (0xED6F)
        case 0x6f:
            temp = A;
            A = (A & 0xf0) | ((memory[getHL()] & 0xf0) >> 4);
            memory[getHL()] = ((memory[getHL()] & 0x0f) << 4) + (temp & 0x0f);
            if (A & 0x80)
                setFlag(S_BIT);
            else
                clearFlag(S_BIT);
            if (A == 0)
                setFlag(Z_BIT);
            else
                clearFlag(Z_BIT);
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
            break;

        // RRD (0xED67)
        case 0x67:
            temp = A;
            A = (A & 0xf0) | (memory[getHL()] & 0x0f);
            memory[getHL()] = ((memory[getHL()] & 0xf0) >> 4)
                            + ((temp & 0x0f) << 4);
            update_S(A);
            update_Z(A);
            clearFlag(H_BIT);
            update_P(A);
            clearFlag(N_BIT);
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
            SP += 2;
            break;

        default:
            // Invalid opcodes are disassembled with DEFB directives and
            // do not need special handling here. 
            break;
    }
}
