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
        // Now figure out what sort of instruction data we loaded to construct the correct mnemonic string
        // Most opcodes either load no data, or the additional bytes are extensions to the opcode: these are covered by the "default:" case
        // Otherwise, we need special handlers for a subset of the opcodes
        switch (IR[0]) {
            // Relative Jumps
            case 0x10:
            case 0x20:
            case 0x30:
            case 0x18:
            case 0x28:
            case 0x38:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[1]+PC-2);
                break;

            // IX Instructions (0xdd)
            // For 3-byte instructions, 3rd byte is "n" 
            // For 4-byte instructions, 3rd and 4th byte are "nn"
            case 0xdd: 
                if (instr_length == 3){
                    snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2]);
                }
                else if (instr_length ==4) {
                    snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[3], IR[2]);
                }
                else snprintf(mnemonic, MAX_MNEMONIC_LENGTH, "%s", opcodes[IR[0]].mn);
                break;
            
            // Miscellaneous Instructions (0xed)
            // For 3-byte intructions, byte 3 is "n"
            // For 4-byte instructions, bytes 3 and 4 are "nn"
            // Note: The only 3-byte instructions are Z180 only, which isn't implemented in this emulator
            // NOTE: This implementation is the same as 0xed. After verifying the instructions, merge these two cases together.
            case 0xed: 
                if (instr_length == 3){
                    snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2]);
                }
                else if (instr_length ==4) {
                    snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[3], IR[2]);
               }
                else snprintf(mnemonic, MAX_MNEMONIC_LENGTH, "%s", opcodes[IR[0]].mn);
                break;

            // IY Instructions (0xfd)
            // For 3-byte intructions, byte 3 is "n"
            // For 4-byte instructions, bytes 3 and 4 are "nn" 
            // EXCEPT: 0xfdcb (IY Bit Instructions) work differently
            case 0xfd:
                if (IR[2] == 0xCB) { // These are all 4-byte instructions, where 3rd byte is "n"
                    snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2]);
                }
                else {
                    if (instr_length == 3){
                        snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2]);
                    }
                    else if (instr_length ==4) {
                        snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[3], IR[2]);
                    }
                    else snprintf(mnemonic, MAX_MNEMONIC_LENGTH, "%s", opcodes[IR[0]].mn);
                    break;
                }

            default: // General case, depends on length of instruction
                if (instr_length == 1) strncpy(mnemonic, opcodes[IR[0]].mn, MAX_MNEMONIC_LENGTH);
                if (instr_length == 2) snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[1]);
                if (instr_length == 3) snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2], IR[1]);
                if (instr_length == 4) snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2], IR[1]);
                break;
        }
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

void Z80::update_flags(unsigned char f_list, INST_TYPE t, unsigned char val1, unsigned char val2) {
    if (f_list & S_BIT) {
        update_S(t, val1);
    }
    if (f_list & Z_BIT) {
        update_Z(t, val1);
    }
    if (f_list & H_BIT) {
        update_H(t, val1, val2);
    }
    if (f_list & PV_BIT) {
        update_PV(t, val1, val2);
    }
    if (f_list & N_BIT) {
        update_N(t);
    }
    if (f_list & C_BIT) {
        update_C(t, val1);
    }
}

// Methods for updating the various bits in the Flags register
    void Z80::update_C(INST_TYPE t, unsigned char val) {
        switch (t) {
            case ADD:
              break;
            case SUB:
              break;
            case COMP:
              break;
            case TEST:
              break;
            default:  // NONE - Flag not affected
              break;
        }
    }

    void Z80::update_N(INST_TYPE t) {
        switch (t) {
            case ADD:
              F.N = 0;
              break;
            case SUB:
              F.N = 1;
              break;
            case COMP:
            case TEST:
            default:  // NONE - Flag not affected
              break;
        }
    }
    
    void Z80::update_PV(INST_TYPE t, unsigned char val1, unsigned char val2) {
        switch (t) {
            case ADD:
              if ((val1 & 0x80) != (val2 & 0x80)) F.PV = 0;   // operands are diffent signs, no overflow
              else if (((int) val1 + (int) val2 > 127) || ((int) val1 + (int) val2 < -128)) F.PV = 1;
              else F.PV = 0;
              break;
            case SUB:
              if ((val1 & 0x80) == (val2 & 0x80)) F.PV = 0;   // operands are same signs, no overflow
              else if (((int) val1 + (int) val2 > 127) || ((int) val1 + (int) val2 < -128)) F.PV = 1;
              else F.PV = 0;
              break;
            case COMP:
              break;
            case TEST:
              break;
            default:  // NONE - Flag not affected
              break;
        }
    }
    
    void Z80::update_H(INST_TYPE t, unsigned char val1, unsigned char val2) {
        // It is important that the calling function masks val so that it is only a 4-bit value
        switch (t) {
            case ADD:
              if (((val1 & 0x0f) + (val2 &0x0f)) > 0x0f) H = 1; else H = 0;
              break;
            case SUB:
              if ((val1 & 0xf) < (val2 & 0x0f)) H = 1; else H = 0;
              break;
            case COMP:
              break;
            case TEST:
              break;
            default:  // NONE - Flag not affected
              break;
        }
    }
    
    void Z80::update_Z(INST_TYPE t, unsigned char val) {
        switch (t) {
            case ADD:
            case SUB:
              if (val == 0) F.Z = 1; else F.Z = 0;
              break;
            case COMP:
              break;
            case TEST:
              break;
            default:  // NONE - Flag not affected
              break;
        }
    }
    
    void Z80::update_S(INST_TYPE t, unsigned char val) {
        switch (t) {
            case ADD:
            case SUB:
              if (val & 0x80) F.S = 1; else F.S = 0;
              break;
            case COMP:
              break;
            case TEST:
              break;
            default:  // NONE - Flag not affected
              break;
        }
    }
    