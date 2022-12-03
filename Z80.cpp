// Z80 core definitions
#include "Z80.h"
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
	unsigned char data;

    cout << "Loading memory from file: " << fname << endl;

    ifstream memfile(fname,iostream::in|ios::binary);
	assert(memfile); // Check if open successful

	while (memfile >> data) {
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
    A = 0;
    F.S = 0;
    F.Z = 0;
    F.H = 0;
    F.PV = 0;
    F.N = 0;
    F.C = 0;
    B = 0; C = 0; D = 0; E = 0; H = 0; L = 0;
    Aprime = 0;
    Fprime = 0;
    Bprime = 0; Cprime = 0; Dprime = 0; Eprime = 0; Hprime = 0; Lprime = 0;
    I = 0;
    R = 0;
    IX = 0; IY = 0;
    SP = 0;
    PC = 0;
    IR = 0; 
    IFF1 = 0;
    IFF2 = 0;
    INT_MODE = 0;
}

void Z80::run_from_address(unsigned short addr) {
    // Keep RAM and registers intact, start from addr passed into function
    cout << "Starting execution from address: 0x" << hex << addr << endl;
    PC = addr;
}

void Z80::fetch() {
    ///cout << "Fetching instruction at address: 0x" << hex << PC << endl;
    IR = memory[PC++];
}

unsigned char Z80::get_next_byte() {
    ///cout << "Loading a byte from memory." << endl;
    return memory[PC++];
}
