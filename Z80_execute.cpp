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
            cout << "Unimplemented instruction: 0x" << hex << (unsigned int) IR[0] << "!" << endl;
            break;
    }
}

