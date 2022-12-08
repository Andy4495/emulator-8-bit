// Z80 core definitions
#include "Z80.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>
#include <cstring>

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

    switch (IR[0]) {
        case 0x00:  // NOP -- no flags affected
            break;
        
        case 0x01:  // LD BC, nn -- no flags affected
            C = IR[1];
            B = IR[2];
            break;
        
        case 0x02:  // LD (BC), A -- no flags affected
            memory[(B<<8) + C] = A;
            break;
        
        case 0x03:  // INC BC -- no flags affected
            C++;
            if (C == 0) B++;
            break;
        
        case 0x04:  // INC B -- S, Z, H, P/V, N
            B++;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, ADD, B, 1);
            break;
        
        case 0x05:  // DEC B -- S, Z, H, P/V, N
            B--;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, SUB, B, 1);
            break;
        
        case 0x06:  // LD B, n -- no flags affected
            B = IR[1];
            break;
        
        case 0x3e:  // LD A, n -- no flags affected
            A = IR[1];
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
