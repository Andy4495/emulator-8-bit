/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   v0.1.0    02/11/23  Andy4495 Read for first "release"
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
   0.3.0 04/16/2023 Andy4495  Tag v0.3.0
*/

#include "Z80.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
using std::cout;
using std::endl;

void Z80::execute() {
    switch (IR[0]) {
        case 0xCB:
            execute_bit_opcode();
            break;

        case 0xDD:
            if (IR[1] == 0xCB)
                execute_ix_iy_bit_opcode();
            else
                execute_index_opcode();
            break;

        case 0xED:
            execute_misc_opcode();
            break;

        case 0xFD:
            if (IR[1] == 0xCB)
                execute_ix_iy_bit_opcode();
            else
                execute_index_opcode();
            break;

        default:
            execute_main_opcode();
            break;
    }
}
