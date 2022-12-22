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

void Z80::execute_iy_opcode() { // IR[0] = 0xFD

    switch (IR[1]) {
        case 0x00:  // NOP -- no flags affected
            break;

        case 0x04:  // INC B
            B++;
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, ADD, B, 1);
            break;            

        default: 
            cout << "Execution not defined: 0xfd" << hex << setw(2) << (unsigned int) IR[1] << endl;
            break;
    }
}

void Z80::execute_iy_bit_opcode() {  // IR[0,1] = 0xFDCB

    switch (IR[3]) {
        case 0x00:  // RLC (IY+d), B
            if (memory[IY+IR[2]] & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            memory[IY+IR[2]] = memory[IY+IR[2]] << 1;
            memory[IY+IR[2]] = (memory[IY+IR[2]] & 0xFE) | testFlag(C_BIT);
            B = memory[IY+IR[2]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, BIT, memory[IY+IR[2]], 0);
            break;

        default: 
            cout << "Execution not defined: 0xfdcbnn" << hex << setw(2) << (unsigned int) IR[3] << endl;
            break;
    }
}
