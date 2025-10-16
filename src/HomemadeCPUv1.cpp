/* Homemade CPU Emulator, Version 1
   Copyright 2025 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   See https://github.com/Andy4495/Homemade-CPU for the CPU this emulates

   This code is based off version 1 of the CPU design. It has been superseded by Version 2.

   0.1  08-Oct-2025  Andy4495  Initial Creation


*/

#include "HomemadeCPUv1.h"
#include "HomemadeCPUv1_opcodes.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cassert>
#include <cstdint>
using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::dec;
using std::ifstream;
using std::ofstream;
using std::iostream;
using std::ios;
using std::setw;


// public: 
HomemadeCPUv1::HomemadeCPUv1(uint16_t ramstart, uint16_t ramend) {
    _ramstart = ramstart;
    _ramend   = ramend;
    delay_slot = false;
}

void HomemadeCPUv1::load_memory(const char* fname) {
    // Load memory from filename fname
    unsigned int i = 0;
    char data;
    uint8_t msb, lsb;

    ifstream memfile(fname, iostream::in|ios::binary);
    assert(memfile);  // Check if open successful

    while (memfile.read(&data, 1)) {
        memory[i++] = data;
        if (i > CPU_MAX_MEMORY - 1) {
            cout << "Loading registers . . . ";
            // Add registers to end of file.
            memfile >> AC;
            memfile >> FL;
            memfile >> msb;
            setSH(msb);
            memfile >> lsb;
            setSL(lsb);
            memfile >> MH;
            memfile >> JH;
            memfile >> JL;
            memfile >> msb;
            memfile >> lsb;
            setPC(msb, lsb);
            memfile >> IR;
            memfile >> OR;
            break;    // Make sure we don't try to load beyond memory space
        }
    }
}

void HomemadeCPUv1::dump_memory_to_file(const char* fname) {
    ofstream memfile(fname, iostream::out|ios::binary);
    assert(memfile);  // Check if open successful
    for (uint32_t i = 0; i <= 0xffff; i++) {
        memfile << memory[i];
    }
    // Add registers to end of file.
    memfile << AC;
    memfile << FL;
    memfile << getSH();
    memfile << getSL();
    memfile << MH;
    memfile << JH;
    memfile << JL;
    memfile << (uint8_t) ((PC & 0xff00) >> 8); 
    memfile << (uint8_t)  (PC & 0xff);
    memfile << IR;
    memfile << OR;

    memfile.close();
}

void HomemadeCPUv1::cold_reset() {
    // Clear RAM and registers, start from $0000
    // CPU registers set to all zeroes
    // RAM is left as-is
    cout << "Cold Reset: PC and other registers set to all zeroes. "
         << endl;
    init_registers();
    Halt = false;
    delay_slot = false;
}

void HomemadeCPUv1::warm_reset() {
    // Keep RAM intact, clear registers
    cout << "Warm Reset: PC set to $0000, other registers unchanged." << endl;
    PC = 0x0000;
    Halt = false;
    delay_slot = false;
}

void HomemadeCPUv1::init_registers() {
    AC  = 0x00;
    FL  = 0x00;
    setSH(0x00);
    setSL(0x00);
    MH = 0x00;
    JH = 0x00;
    JL = 0x00;
    PC = 0x0000;
    IR = 0x00;
    OR = 0x00;
}

void HomemadeCPUv1::setFlag(FLAG_BITS f) {
    FL |= f;
}

void HomemadeCPUv1::clearFlag(FLAG_BITS f) {
    FL = FL & ~f;
}

uint8_t HomemadeCPUv1::testFlag(FLAG_BITS f) {
    return (FL & f) ? 1 : 0;
}

void HomemadeCPUv1::run_from_address(uint16_t addr) {
    // Keep RAM and registers intact, start from addr passed into function
    cout << ";Starting from address: 0x" << hex << addr << endl;
    PC = addr;
    Halt = false;
}

uint8_t HomemadeCPUv1::get_next_byte() {
    return memory[PC++];
}

void HomemadeCPUv1::print_fetched_instruction() {
    // Same format for every opcode
    snprintf(instr_string, CPU_MAX_TEXT_LENGTH, "%04x  %-s  %s",
             PC_of_Fetch, fetched, mnemonic);
    cout << instr_string << endl;
}

// This is the same as print_fetched_instruction, except it doesn't
// print the address, so that it can be input into an assembler
void HomemadeCPUv1::print_assembly() {
    // Same format for every opcode
    snprintf(instr_string, CPU_MAX_TEXT_LENGTH, "      %s", mnemonic);
    cout << instr_string << endl;
}

void HomemadeCPUv1::print_registers() {
    cout << hex << "AC:  " << setw(2) << (uint16_t) AC
    <<  " FL:  "  << setw(2) << (uint16_t) FL
    <<  " SH:  "  << setw(2) << (uint16_t) getSH()
    <<  " SL:  "  << setw(2) << (uint16_t) getSL()
    <<  " MH:  "  << setw(2) << (uint16_t) MH
    <<  " JH:  "  << setw(2) << (uint16_t) JH
    <<  " JL:  "  << setw(2) << (uint16_t) JL
    <<  " PC:  "  << setw(4) << (uint16_t) PC
    <<  " IR: "  << setw(2) << (uint16_t) IR
    <<  " OR: "  << setw(2) << (uint16_t) OR

    << endl;
}

void HomemadeCPUv1::print_flags() {
    cout << "ZCSV: "
    << (unsigned int) testFlag(HomemadeCPUv1::Z_BIT)
    << (unsigned int) testFlag(HomemadeCPUv1::C_BIT)
    << (unsigned int) testFlag(HomemadeCPUv1::S_BIT)
    << (unsigned int) testFlag(HomemadeCPUv1::V_BIT)
    << endl;
}

void HomemadeCPUv1::print_memory(uint16_t start, uint16_t end) {
    cout << endl << "        0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f" << endl;
    cout         << "       -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --" << endl;
    for (uint32_t i = (start & 0xfff0); i <= (uint32_t) end; i += 16) {
        cout << hex << setw(3) << i/16 << "x" << ":  ";
        for (int j = 0; j < 16; j ++) {
            cout << hex << setw(2) << (uint16_t) memory[i + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

uint8_t HomemadeCPUv1::get_memory(uint16_t loc) {
    return(memory[loc]);
}

void HomemadeCPUv1::set_memory(uint16_t loc, uint8_t val) {
    memory[loc] = val;
}

uint8_t HomemadeCPUv1::get_input_port(uint8_t loc) {
    cout << "Not supported by Homemade CPU";
    return 0;
}

void HomemadeCPUv1::set_input_port(uint8_t loc, uint8_t val) {
    cout << "Not supported by Homemade CPU";
}

uint8_t HomemadeCPUv1::get_output_port(uint8_t loc) {
    cout << "Not supported by Homemade CPU";
    return 0;
}

void HomemadeCPUv1::set_register() {
    uint16_t regno, val;

    cout << "Current register values:" << endl;
    print_registers();
    cout << endl;

    cout << "Enter the register number to update: " << endl;
    cout << " 1: AC     2: FL     3: SH     4: SL     5: MH     6: JH     7: JL "  << endl;
    cout << "11: PC    12: IR    13: OR"                                           << endl;
    cin >> regno; 
    cout << "Enter new value in hex: 0x";
    cin >> hex >> val;
    cout << endl;
    switch(regno) {
        case 1:
            AC = val;
            break;
        case 2:
            FL = val;
            break;
        case 3:
            setSH(val);
            break;
        case 4:
            setSL(val);
            break;
        case 5:
            MH = val;
            break;
        case 6:
            JH = val;
            break;
        case 7:
            JL = val;
            break;
        case 11:
            PC = val;
            break;
        case 12:
            IR = val;
            break;
        case 13:
            OR = val;
            break;
        default:
            cout << "Invalid register number: " << regno << endl;
            break;
    }
}

uint8_t HomemadeCPUv1::halt_opcode() {
    return 0xFF;
}

// If true, the CPU is halted
bool HomemadeCPUv1::halted() {  
    return Halt;
}

uint16_t HomemadeCPUv1::getPC() {
    return PC;
}

void HomemadeCPUv1::fetch_and_decode() {
    // All instructions are 2 bytes long

    // Check if this is a branch delay slot instruction
    // If there is a jump, then the next instruction is already in the pipeline,
    // so it needs to be executed without changing the PC (since PC already has 
    // new jump-to location)
    if (delay_slot) {
        // Need to use PC_of_Fetch, since that contains address of last executed instruction
        // and need to increment it to point to the delay slot instruction
        cout << ">>> Branch delay slot <<<" << endl;
        PC_of_Fetch++;
        IR = memory[PC_of_Fetch++];
        OR = memory[PC_of_Fetch];
        delay_slot = false; 
    }
    else { // No delay slot, so fetch normally
        PC_of_Fetch = PC;  // Save the current PC for printing later
        IR = memory[PC++];
        OR = memory[PC++];
    }

    // Decode 
    instr_length = 2;
    snprintf(fetched, CPU_MAX_FETCHED_LENGTH, "%02x%02x    ",
                IR, OR);
    switch (opcodes[IR].s) {
        case ON:
            snprintf(mnemonic, CPU_MAX_MNEMONIC_LENGTH, opcodes[IR].mn, OR);
            break;
        case OU:
            snprintf(mnemonic, CPU_MAX_MNEMONIC_LENGTH, "%s", opcodes[IR].mn);
            break;
        case NN:
            snprintf(mnemonic, CPU_MAX_MNEMONIC_LENGTH, opcodes[IR].mn, IR, OR);
            break;
        default:  // Invalid instruction layout
            strncpy(mnemonic, "Invalid layout", CPU_MAX_MNEMONIC_LENGTH);
            break;
    }
}

// private:

void HomemadeCPUv1::update_Z(uint8_t val) {
    if (val == 0) setFlag(Z_BIT);
    else          clearFlag(Z_BIT);
}

void HomemadeCPUv1::update_S(uint8_t val) {
    if (val & 0x80) setFlag(S_BIT);
    else            clearFlag(S_BIT);
}

uint8_t HomemadeCPUv1::getSH() {
    return (SP >> 8);
}

uint8_t HomemadeCPUv1::getSL() {
    return SP & 0x00ff;
}

// Jump Address: 16-bit equivalent of JH, JL
uint16_t HomemadeCPUv1::getJA() {  
    return (JH << 8) + JL;
}

// Jump Address: 16-bit equivalent of MH and OR (which contains ML)
uint16_t HomemadeCPUv1::getMA() {
    return (MH << 8) + OR;
}

void HomemadeCPUv1::setSH(uint8_t v) {
    SP = (v << 8) + (SP & 0x00ff);
}

void HomemadeCPUv1::setSL(uint8_t v) {
    SP = (SP & 0xff00) + v;
}

void HomemadeCPUv1::setPC(uint8_t msb, uint8_t lsb) {
    PC = (msb << 8) + lsb;
}

