// Z80 core definitions
#include "Z80.h"
#include "Z80_opcodes.h"

#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>

Z80::Z80(unsigned short ramstart, unsigned short ramend, unsigned short romstart, unsigned short romend) {
    _ramstart = ramstart;
    _ramend   = ramend;
    _romstart = romstart;
    _romend   = romend;
}

void Z80::load_memory(const char* fname) {
    // Load memory from filename fname
    unsigned int i = 0;
	char data;

    cout << "Loading memory from file: " << fname << endl;

    ifstream memfile(fname,iostream::in|ios::binary);
	assert(memfile); // Check if open successful

    while (memfile.read(&data, 1)) {
		memory[i++] = data;
		if (i > MAX_MEMORY - 1) {
			cout << "Filled memory before reaching end of file." << endl;
			break;    // Make sure we don't try to load beyond memory space
		}
	}
}

void Z80::cold_reset() {
    // Clear RAM and registers, start from $0000
    cout << "Cold Reset: clearing RAM, clearing registers and PC set to $0000" << endl;
    for (unsigned long i = _ramstart; i <= _ramend; i++) memory[i] = 0;
    clear_registers();
}

void Z80::warm_reset() {
    // Keep RAM intact, clear registers, start from $0000
    cout << "Warm Reset: clearing registers and PC set to $0000" << endl;
    clear_registers();
}

void Z80::clear_registers() {
    A    = 0;
    F.S  = 0;
    F.Z  = 0;
    F.H  = 0;
    F.PV = 0;
    F.N  = 0;
    F.C  = 0;
    B    = 0; C = 0; D = 0; E = 0; H = 0; L = 0;
    Aprime    = 0;
    Fprime.S  = 0;
    Fprime.Z  = 0;
    Fprime.H  = 0;
    Fprime.PV = 0;
    Fprime.N  = 0;
    Fprime.C  = 0;    
    Bprime    = 0; Cprime = 0; Dprime = 0; Eprime = 0; Hprime = 0; Lprime = 0;
    I = 0;
    R = 0;
    IX = 0; IY = 0;
    SP = 0;
    PC = 0;
    for (int i = 0; i < MAX_INSTR_SIZE; i++) IR[i] = 0;
    IFF1 = 0;
    IFF2 = 0;
    INT_MODE = 0;
}

void Z80::run_from_address(unsigned short addr) {
    // Keep RAM and registers intact, start from addr passed into function
    cout << "Starting execution from address: 0x" << hex << addr << endl;
    PC = addr;
}

void Z80::fetch_and_decode() {
    unsigned int i;
    ///cout << "Fetching instruction at address: 0x" << hex << PC << endl; /// Debug
    IR[0] = memory[PC++];
    if (IR[0] < MAX_OPCODE) {
        instr_length = opcodes[IR[0]].length;
        for (i = 0; i < instr_length - 1; i++) IR[i+1] = get_next_byte();
        // Create the memory string based on size of instruction: 
        if (instr_length == 1) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ", IR[0]);
        if (instr_length == 2) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ", IR[0], IR[1]);
        if (instr_length == 3) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ", IR[0], IR[1], IR[2]);
        if (instr_length == 4) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x", IR[0], IR[1], IR[2], IR[3]);
        snprintf(mnemonic, MAX_MNEMONIC_LENGTH, "%s", opcodes[IR[0]].mn);
    }
    else {
        instr_length = 1;
        snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ", IR[0]);
        snprintf(mnemonic, MAX_MNEMONIC_LENGTH, "<UNIMP>");
    }
}

unsigned char Z80::get_next_byte() {
    ///cout << "Loading a byte from memory." << endl; /// Debug
    return memory[PC++];
}

void Z80::print_fetched_instruction() {
    snprintf(instr_string, MAX_TEXT_LENGTH, "%04x  %-s  %s", PC - instr_length, fetched, mnemonic);  // Same format for every opcode
    cout << instr_string << endl;
}