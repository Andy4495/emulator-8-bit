// Z80 core definitions
#include "Z80.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>

void Z80::decode() {
    cout << "Decoding and executing." << endl;

    switch (IR) {
        case 0x00:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << endl;
            cout << "NOP" << endl;
            break;
        
        case 0x01:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << endl;
            C = get_next_byte();
            B = get_next_byte();
            cout << "LD BC, $" << hex << (unsigned int) B << (unsigned int) C << endl;
            break;
        
        case 0x31:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << endl;
            break;
        
        case 0x32:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << endl;
            break;
        
        case 0x33:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << endl;
            break;
        
        case 0x34:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << endl;
            break;
        
        case 0x35:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << endl;
            break;
        
        default: 
            cout << "Unimplemented instruction: 0x" << hex << (unsigned int) IR << "!" << endl;
            break;
    }
}
