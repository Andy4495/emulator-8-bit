// Z80 core definitions
#include "Z80.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>
#include <cstring>

#define MAX_BUFFER 8

void Z80::execute() {
    char buffer[8];

    switch (IR[0]) {
        case 0x00:  // NOP -- no flags affected
            break;
        
        case 0x01:  // LD BC, nn -- no flags affected
            C = IR[1];
            B = IR[2];
            snprintf(buffer, MAX_BUFFER, "%02x%02x", B, C);
            strncat(mnemonic, buffer, 4);
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
            update_S(ADD,  B);
            update_Z(ADD,  B);
            update_H(ADD,  (B & 0xF) + 1);
            update_PV(ADD, B, 1);
            update_N(ADD);
            update_C(NONE, B); // C not affected
            break;
        
        case 0x05:  // DEC B -- S, Z, H, P/V, N
            B--;
            update_S(SUB,  B);
            update_Z(SUB,  B);
            update_H(SUB,  (B & 0xF) - 1);
            update_PV(SUB, B, 1);
            update_N(SUB);
            update_C(NONE, B); // C not affected
            break;
        
        case 0x06:  // LD B, n -- no flags affected
            B = IR[1];
            snprintf(buffer, MAX_BUFFER, "%02x", B);
            strncat(mnemonic, buffer, 2);
            break;
        
        case 0x3e:  // LD A, n
            A = IR[1];
            snprintf(buffer, MAX_BUFFER, "%02x", A);
            strncat(mnemonic, buffer, 4);
            break;
        
        default: 
            cout << "Unimplemented instruction: 0x" << hex << (unsigned int) IR[0] << "!" << endl;
            break;
    }
}

