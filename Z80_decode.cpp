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
            cout << "Instruction: 0x" << hex << (unsigned int) IR << dec << endl;
            break;
        
        case 0x31:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << dec << endl;
            break;
        
        case 0x32:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << dec << endl;
            break;
        
        case 0x33:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << dec << endl;
            break;
        
        case 0x34:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << dec << endl;
            break;
        
        case 0x35:
            cout << "Instruction: 0x" << hex << (unsigned int) IR << dec << endl;
            break;
        
        default: 
            cout << "Unimplemented instruction: 0x" << hex << (unsigned int) IR << dec << "!" << endl;
            break;
    }
}
