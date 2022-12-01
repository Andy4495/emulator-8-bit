// Z80 core definitions
#include "Z80.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>

void Z80::load_memory(const char* fname) {
    // Load memory from filename fname
    unsigned int i = 0;
	unsigned char data;

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
    // Clear RAM and registers, start from reset vector

}

void Z80::warm_reset() {
    // Keep RAM intact, clear registers, start from reset vector

}

void Z80::run_from_address(unsigned short addr) {
    // Keep RAM and registers intact, start from addr passed into function

}

void Z80::fetch() {

}

void Z80::decode() {

}

