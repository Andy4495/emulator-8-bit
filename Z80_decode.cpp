// Z80 core definitions
#include "Z80.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>

#define MAX_TEXT_LENGTH 64
#define MAX_INSTR_LENGTH 16
#define MAX_FETCHED_LENGTH 9

void Z80::decode() {
    char s[MAX_TEXT_LENGTH];
    char instr[MAX_INSTR_LENGTH];
    char fetched[MAX_FETCHED_LENGTH];
    unsigned int instr_PC;

    switch (IR) {
        case 0x00:  // NOP
            instr_PC = PC - 1;
            snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ", IR);
            snprintf(instr, MAX_INSTR_LENGTH, "NOP");
            snprintf(s, MAX_TEXT_LENGTH, "%04x  %-s  %s", instr_PC, fetched, instr);  // Same format for every opcode
            cout << s << endl;
            break;
        
        case 0x01:  // LD BC, nn
            instr_PC = PC - 1;
            C = get_next_byte();
            B = get_next_byte();
            snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ", IR, C, B);
            snprintf(instr, MAX_INSTR_LENGTH, "LD BC, $%02x%02x", B, C);
            snprintf(s, MAX_TEXT_LENGTH, "%04x  %-s  %s", instr_PC, fetched, instr);  // Same format for every opcode
            cout << s << endl;
            break;
        
        case 0x02:  // LD (BC), A
            instr_PC = PC - 1;
            memory[(B<<8) + C] = A;
            snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ", IR);
            snprintf(instr, MAX_INSTR_LENGTH, "LD (BC), A");
            snprintf(s, MAX_TEXT_LENGTH, "%04x  %-s  %s", instr_PC, fetched, instr);  // Same format for every opcode
            cout << s << endl;
            break;
        
        case 0x3e:  // LD A, n
            instr_PC = PC - 1;
            A = get_next_byte();
            snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ", IR, A);
            snprintf(instr, MAX_INSTR_LENGTH, "LD A, $%02x", A);
            snprintf(s, MAX_TEXT_LENGTH, "%04x  %-s  %s", instr_PC, fetched, instr);  // Same format for every opcode
            cout << s << endl;
            break;
        
        default: 
            cout << "Unimplemented instruction: 0x" << hex << (unsigned int) IR << "!" << endl;
            ///cout << hex << "B: " << (unsigned int) B << " C: " << (unsigned int) C << " memory[BC]: " << (unsigned int) memory[B*256+C] << endl; ///
            break;
    }
}
