/* Z80 Emulator 
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
*/

// Z80 core definitions
#include "Z80.h"
#include "Z80_opcodes.h"

#include <iostream>
#include <fstream>
#include <cstring>
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
    cout << "Cold Reset: clearing RAM, clearing registers, PC set to $0000" << endl;
    for (unsigned long i = _ramstart; i <= _ramend; i++) memory[i] = 0;
    clear_registers();
    Halt = 0;
}

void Z80::warm_reset() {
    // Keep RAM intact, clear registers, start from $0000
    cout << "Warm Reset: clearing registers, PC set to $0000" << endl;
    clear_registers();
    Halt = 0;
}

void Z80::clear_registers() {
    A    = 0;
    F    = 0;
    B    = 0; C = 0; D = 0; E = 0; H = 0; L = 0;
    Aprime  = 0;
    Fprime  = 0;   
    Bprime  = 0; Cprime = 0; Dprime = 0; Eprime = 0; Hprime = 0; Lprime = 0;
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

void Z80::setFlag(FLAG_BITS f) {
    F |= f;
}

void Z80::clearFlag(FLAG_BITS f) {
    F = F & ~f;
}

int Z80::testFlag(FLAG_BITS f) {
    return (F & f) ? 1 : 0;
}

void Z80::run_from_address(unsigned short addr) {
    // Keep RAM and registers intact, start from addr passed into function
    cout << "Starting execution from address: 0x" << hex << addr << endl;
    PC = addr;
}

void Z80::fetch_and_decode() {
    ///cout << "Fetching instruction at address: 0x" << hex << PC << endl; /// Debug
    PC_of_Fetch = PC; // Save the current PC for printing later
    IR[0] = memory[PC++];
    // Figure out the opcode type (main or extended) and decode from the appropriate opcode array
    switch (IR[0]) {
        case 0xcb:     // Bit Instructions
            IR[1] = memory[PC++];
            decode_bit_instruction();
            break;
        case 0xdd:     // IX Register Instructions
            IR[1] = memory[PC++];
            decode_ix_instruction();
            break;
        case 0xed:     // Miscellaneous Instructions
            IR[1] = memory[PC++];
            decode_misc_instruction();       
            break;
        case 0xfd:    // IY Register Instructions
            IR[1] = memory[PC++];
            decode_iy_instruction();
            break;
        default:       // Main Instructions
            decode_main_instruction();
            break;
    }
}

unsigned char Z80::get_next_byte() {
    ///cout << "Loading a byte from memory." << endl; /// Debug
    return memory[PC++];
}

void Z80::print_fetched_instruction() {
    snprintf(instr_string, MAX_TEXT_LENGTH, "%04x  %-s  %s", PC_of_Fetch, fetched, mnemonic);  // Same format for every opcode
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
        case ADC:
          break;
        case SUB:
          break;
        case SBC:
          break;
        case COMP:
          break;
        case TEST:
          break;
        case BIT:
          break;
        default:  // NONE - Flag not affected
          break;
    }
}

void Z80::update_N(INST_TYPE t) {
    switch (t) {
        case ADD:
        case ADC:
        case BIT:
          clearFlag(N_BIT);
          break;
        case SUB:
        case SBC:
          setFlag(N_BIT);
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
          if ((val1 & 0x80) != (val2 & 0x80)) clearFlag(PV_BIT);   // operands are diffent signs, no overflow
          else if (((int) val1 + (int) val2 > 127) || ((int) val1 + (int) val2 < -128)) setFlag(PV_BIT);
          else clearFlag(PV_BIT);
          break;
        case ADC:
          break;
        case SUB:
          if ((val1 & 0x80) == (val2 & 0x80)) clearFlag(PV_BIT);   // operands are same signs, no overflow
          else if (((int) val1 + (int) val2 > 127) || ((int) val1 + (int) val2 < -128)) setFlag(PV_BIT);
          else clearFlag(PV_BIT);
          break;
        case SBC:
          break;
        case COMP:
          break;
        case TEST:
          break;
        case BIT:
          /// Implement even/odd parity check
          break;
        default:  // NONE - Flag not affected
          break;
    }
}

void Z80::update_H(INST_TYPE t, unsigned char val1, unsigned char val2) {
    // It is important that the calling function masks val so that it is only a 4-bit value
    switch (t) {
        case ADD:
          if (((val1 & 0x0f) + (val2 &0x0f)) > 0x0f) setFlag(H_BIT); else clearFlag(H_BIT);
          break;
        case ADC:
          break;
        case SUB:
          if ((val1 & 0xf) < (val2 & 0x0f)) setFlag(H_BIT); else clearFlag(H_BIT);
          break;
        case SBC:
          break;
        case COMP:
          break;
        case TEST:
          break;
        case BIT: 
          clearFlag(H_BIT); 
          break;
        default:  // NONE - Flag not affected
          break;
    }
}

void Z80::update_Z(INST_TYPE t, unsigned char val) {
    switch (t) {
        case ADD:
        case ADC:
        case SUB:
        case SBC:
        case BIT:
          if (val == 0) setFlag(Z_BIT); else clearFlag(Z_BIT);
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
        case ADC:
        case SUB:
        case SBC:
        case BIT:
          if (val & 0x80) setFlag(S_BIT); else clearFlag(S_BIT);
          break;
        case COMP:
          break;
        case TEST:
          break;
        default:  // NONE - Flag not affected
          break;
    }
}

void Z80::decode_main_instruction() {
    unsigned int i;
    instr_length = opcodes[IR[0]].length;
    for (i = 1; i < instr_length; i++) IR[i] = get_next_byte();
    // Create the memory string based on size of instruction: 
    if (instr_length == 1) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ", IR[0]);
    if (instr_length == 2) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ", IR[0], IR[1]);
    if (instr_length == 3) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ", IR[0], IR[1], IR[2]);
    if (instr_length == 4) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x", IR[0], IR[1], IR[2], IR[3]);
    // Depending on the instruction, there may be data that needs to be displayed as part of the instruction string
    // The ".s" field in the opcodes array defines the instruction layout of opcodes and data
    // Relative jumps ("JN") require slightly different handling in order to generate the 16-bit address from 
    // the 8-bit relative jump value
    switch (opcodes[IR[0]].s) {
        case O:
        case OO:
            strncpy(mnemonic, opcodes[IR[0]].mn, MAX_MNEMONIC_LENGTH);
            break; 
        case ON:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[1]);
            break;
        case ONN:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2], IR[1]);
            break;
        case OONN:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[3], IR[2]);
            break;
        case JN:
            // The following uses a clumsy way to deal with the special case where the displacement value is actually 
            // signed where we normally treat the data as unsigned
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, (IR[1] & 0x80) ?  PC - (unsigned char) ~IR[1] - 1 : PC + IR[1]);
            break;
        case OON:
        case OONO:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes[IR[0]].mn, IR[2]);
            break;

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}

void Z80::decode_bit_instruction() {
    // These are all 2 byte instructions of the form OO
    instr_length = opcodes_bit_instructions[IR[1]].length;
    // Create the memory string based on size of instruction: 
    snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ", IR[0], IR[1]);
    // Depending on the instruction, there may be data that needs to be displayed as part of the instruction string
    // The ".s" field in the opcodes array defines the instruction layout of opcodes and data
    switch (opcodes_bit_instructions[IR[1]].s) {
        case OO:
            strncpy(mnemonic, opcodes_bit_instructions[IR[1]].mn, MAX_MNEMONIC_LENGTH);
            break; 

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}

void Z80::decode_misc_instruction() {
    unsigned int i;
    instr_length = opcodes_misc_instructions[IR[1]].length;
    for (i = 2; i < instr_length; i++) IR[i] = get_next_byte();
    // Create the memory string based on size of instruction: 
    if (instr_length == 1) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ", IR[0]);
    if (instr_length == 2) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ", IR[0], IR[1]);
    if (instr_length == 3) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ", IR[0], IR[1], IR[2]);
    if (instr_length == 4) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x", IR[0], IR[1], IR[2], IR[3]);
    // Depending on the instruction, there may be data that needs to be displayed as part of the instruction string
    // The ".s" field in the opcodes array defines the instruction layout of opcodes and data
    // Relative jumps ("JN") require slightly different handling in order to generate the 16-bit address from 
    // the 8-bit relative jump value
    switch (opcodes_misc_instructions[IR[1]].s) {
        case O:
        case OO:
            strncpy(mnemonic, opcodes_misc_instructions[IR[1]].mn, MAX_MNEMONIC_LENGTH);
            break; 
        case ON:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_misc_instructions[IR[1]].mn, IR[1]);
            break;
        case ONN:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_misc_instructions[IR[1]].mn, IR[2], IR[1]);
            break;
        case OONN:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_misc_instructions[IR[1]].mn, IR[3], IR[2]);
            break;
        case JN:
            // The following uses a clumsy way to deal with the special case where the displacement value is actually 
            // signed where we normally treat the data as unsigned
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_misc_instructions[IR[1]].mn, (IR[1] < 0x80) ? IR[1]+PC : PC - (0xff - IR[1] + 1));
            break;
        case OON:
        case OONO:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_misc_instructions[IR[1]].mn, IR[2]);
            break;

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}

void Z80::decode_ix_instruction() {
    unsigned int i;
    if (IR[1] == 0xcb) 
        decode_ix_bit_instruction();
    else {
        instr_length = opcodes_IX_instructions[IR[1]].length;
        for (i = 2; i < instr_length; i++) IR[i] = get_next_byte();
        // Create the memory string based on size of instruction: 
        if (instr_length == 1) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ", IR[0]);
        if (instr_length == 2) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ", IR[0], IR[1]);
        if (instr_length == 3) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ", IR[0], IR[1], IR[2]);
        if (instr_length == 4) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x", IR[0], IR[1], IR[2], IR[3]);
        // Depending on the instruction, there may be data that needs to be displayed as part of the instruction string
        // The ".s" field in the opcodes array defines the instruction layout of opcodes and data
        // Relative jumps ("JN") require slightly different handling in order to generate the 16-bit address from 
        // the 8-bit relative jump value
        switch (opcodes_IX_instructions[IR[1]].s) {
            case O:
            case OO:
                strncpy(mnemonic, opcodes_IX_instructions[IR[1]].mn, MAX_MNEMONIC_LENGTH);
                break; 
            case ON:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IX_instructions[IR[1]].mn, IR[1]);
                break;
            case ONN:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IX_instructions[IR[1]].mn, IR[2], IR[1]);
                break;
            case OONN:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IX_instructions[IR[1]].mn, IR[3], IR[2]);
                break;
            case JN:
                // The following uses a clumsy way to deal with the special case where the displacement value is actually 
                // signed where we normally treat the data as unsigned
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IX_instructions[IR[1]].mn, (IR[1] < 0x80) ? IR[1]+PC : PC - (0xff - IR[1] + 1));
                break;
            case OON:
            case OONO:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IX_instructions[IR[1]].mn, IR[2]);
                break;

            default:  // Invalid instruction layout
                strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
                break;
        }
    }
}

void Z80::decode_iy_instruction() {
    unsigned int i;
    if (IR[1] == 0xcb) 
        decode_iy_bit_instruction();
    else {
        instr_length = opcodes_IY_instructions[IR[1]].length;
        for (i = 2; i < instr_length; i++) IR[i] = get_next_byte();
        // Create the memory string based on size of instruction: 
        if (instr_length == 1) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x      ", IR[0]);
        if (instr_length == 2) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x    ", IR[0], IR[1]);
        if (instr_length == 3) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x  ", IR[0], IR[1], IR[2]);
        if (instr_length == 4) snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x", IR[0], IR[1], IR[2], IR[3]);
        // Depending on the instruction, there may be data that needs to be displayed as part of the instruction string
        // The ".s" field in the opcodes array defines the instruction layout of opcodes and data
        // Relative jumps ("JN") require slightly different handling in order to generate the 16-bit address from 
        // the 8-bit relative jump value
        switch (opcodes_IY_instructions[IR[1]].s) {
            case O:
            case OO:
                strncpy(mnemonic, opcodes_IY_instructions[IR[1]].mn, MAX_MNEMONIC_LENGTH);
                break; 
            case ON:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IY_instructions[IR[1]].mn, IR[1]);
                break;
            case ONN:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IY_instructions[IR[1]].mn, IR[2], IR[1]);
                break;
            case OONN:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IY_instructions[IR[1]].mn, IR[3], IR[2]);
                break;
            case JN:
                // The following uses a clumsy way to deal with the special case where the displacement value is actually 
                // signed where we normally treat the data as unsigned
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IY_instructions[IR[1]].mn, (IR[1] < 0x80) ? IR[1]+PC : PC - (0xff - IR[1] + 1));
                break;
            case OON:
            case OONO:
                snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IY_instructions[IR[1]].mn, IR[2]);
                break;

            default:  // Invalid instruction layout
                strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
                break;
        }
    }
}

void Z80::decode_ix_bit_instruction() {
    // All of these are 4 byte instructions
    IR[2] = get_next_byte();
    IR[3] = get_next_byte();
    instr_length = opcodes_IX_bit_instructions[IR[3]].length;
    // Create the memory string based on size of instruction (all of these are 4 bytes)
    snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x", IR[0], IR[1], IR[2], IR[3]);
    // Depending on the instruction, there may be data that needs to be displayed as part of the instruction string
    // The ".s" field in the opcodes array defines the instruction layout of opcodes and data
    // All of these are of the form OONO
    switch (opcodes_IX_bit_instructions[IR[3]].s) {
        case OONO:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IX_bit_instructions[IR[3]].mn, IR[2]);
            break;

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}

void Z80::decode_iy_bit_instruction() {
    // All of these are 4 byte instructions
    IR[2] = get_next_byte();
    IR[3] = get_next_byte();
    instr_length = opcodes_IY_bit_instructions[IR[3]].length;
    // Create the memory string based on size of instruction (all of these are 4 bytes)
    snprintf(fetched, MAX_FETCHED_LENGTH, "%02x%02x%02x%02x", IR[0], IR[1], IR[2], IR[3]);
    // Depending on the instruction, there may be data that needs to be displayed as part of the instruction string
    // The ".s" field in the opcodes array defines the instruction layout of opcodes and data
    // All of these are of the form OONO
    switch (opcodes_IY_bit_instructions[IR[3]].s) {
        case OONO:
            snprintf(mnemonic, MAX_MNEMONIC_LENGTH, opcodes_IY_bit_instructions[IR[3]].mn, IR[2]);
            break;

        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", MAX_MNEMONIC_LENGTH);
            break;
    }
}