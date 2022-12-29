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

void Z80::execute() {
    R++;
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
