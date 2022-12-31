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

void Z80::execute_ix_bit_opcode() {  // IR[0,1] = 0xDDCB

    switch (IR[3]) {
        case 0x00:  // RLC (IX+d), B
            if (memory[IX+IR[2]] & 0x80) setFlag(C_BIT); else clearFlag(C_BIT);
            memory[IX+IR[2]] = memory[IX+IR[2]] << 1;
            memory[IX+IR[2]] = (memory[IX+IR[2]] & 0xFE) | testFlag(C_BIT);
            B = memory[IX+IR[2]];
            update_flags(S_BIT|Z_BIT|H_BIT|PV_BIT|N_BIT, BIT, memory[IX+IR[2]], 0);
            break;

        default: 
            cout << "Execution not defined: 0xddcbnn" << hex << setw(2) << (unsigned int) IR[3] << endl;
            break;
    }
}
