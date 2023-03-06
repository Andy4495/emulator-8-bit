/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   v0.1.0    02/11/23  Andy4495 Read for first "release"
*/

#include "Z80.h"
#include "Z80_opcodes.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <cstdint>
using std::cout;
using std::endl;
using std::hex;
using std::ifstream;
using std::iostream;
using std::ios;

Z80::Z80(uint16_t ramstart, uint16_t ramend,
         uint16_t romstart, uint16_t romend) {
    _ramstart = ramstart;
    _ramend   = ramend;
    _romstart = romstart;
    _romend   = romend;
}

void Z80::load_memory(const char* fname) {
    // Load memory from filename fname
    unsigned int i = 0;
    char data;

    ifstream memfile(fname, iostream::in|ios::binary);
    assert(memfile);  // Check if open successful

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
    cout << "Cold Reset: clearing RAM, clearing registers, PC set to $0000"
         << endl;
    for (uint32_t i = _ramstart; i <= _ramend; i++) memory[i] = 0;
    clear_registers();
    Halt = false;
}

void Z80::warm_reset() {
    // Keep RAM intact, clear registers, start from $0000
    cout << "Warm Reset: clearing registers, PC set to $0000" << endl;
    clear_registers();
    Halt = false;
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
    IXH = 0; IXL = 0; IYH = 0; IYL = 0;
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

uint8_t Z80::testFlag(FLAG_BITS f) {
    return (F & f) ? 1 : 0;
}

void Z80::run_from_address(uint16_t addr) {
    // Keep RAM and registers intact, start from addr passed into function
    cout << ";Starting from address: 0x" << hex << addr << endl;
    PC = addr;
    Halt = false;
}

uint8_t Z80::get_next_byte() {
    return memory[PC++];
}

void Z80::print_fetched_instruction() {
    // Same format for ever opcode
    snprintf(instr_string, MAX_TEXT_LENGTH, "%04x  %-s  %s",
             PC_of_Fetch, fetched, mnemonic);
    cout << instr_string << endl;
}

// This is the same as print_fetched_instruction, except it doesn't
// print the address, so that it can be input into an assembler
void Z80::print_assembly() {
    // Same format for every opcode
    snprintf(instr_string, MAX_TEXT_LENGTH, "      %s", mnemonic);
    cout << instr_string << endl;
}

void Z80::print_registers() {
    cout << hex << "A: " << (uint16_t) A
    <<  " B: " << (uint16_t) B
    <<  " C: " << (uint16_t) C
    <<  " D: " << (uint16_t) D
    <<  " E: " << (uint16_t) E
    <<  " H: " << (uint16_t) H
    <<  " L: " << (uint16_t) L
    << " IX: " << (uint16_t) getIX()
    << " IY: " << (uint16_t) getIY()
    << " SP: " << (uint16_t) SP
    << "  I: " << (uint16_t) I
    << "  R: " << (uint16_t) R
    << endl;
}

void Z80::print_flags() {
    cout << "SZXH XPNC: "
    << (unsigned int) testFlag(Z80::S_BIT)
    << (unsigned int) testFlag(Z80::Z_BIT)
    << (unsigned int) testFlag(Z80::X1_BIT)
    << (unsigned int) testFlag(Z80::H_BIT)
    << " "
    << (unsigned int) testFlag(Z80::X2_BIT)
    << (unsigned int) testFlag(Z80::PV_BIT)
    << (unsigned int) testFlag(Z80::N_BIT)
    << (unsigned int) testFlag(Z80::C_BIT)
    << endl;
}

void Z80::print_memory(uint16_t start, uint16_t end) {
}

bool Z80::halted() {
    return Halt;
}

uint16_t Z80::getPC() {
    return PC;
}

// Methods for updating the various bits in the Flags register
void Z80::update_C(INST_TYPE t, const uint16_t val1, const uint16_t val2) {
    switch (t) {
        case ADD:
            if ((val1 + val2) & 0x0100)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
        case ADC:
            if ((val1 + val2 + testFlag(C_BIT)) & 0x0100)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            break;
        case SUB:
            if (val2 > val1)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            break;
        case SBC:
            if ((val2 + testFlag(C_BIT)) > val1)
                setFlag(C_BIT);
            else
                clearFlag(C_BIT);
            break;
        default:  // Instruction type not supported
            break;
    }
}

void Z80::update_V(INST_TYPE t, const uint8_t val1, const uint8_t val2) {
    switch (t) {
        case ADD:
            // operands with different signs --> no overflow
            if ((val1 & 0x80) != (val2 & 0x80)) {
                clearFlag(PV_BIT);
            } else {
                // both operands negative, result positive --> overflow
                if ((val1 & 0x80)) {
                    if ((val1 + val2) & 0x80)
                        clearFlag(PV_BIT);
                    else
                        setFlag(PV_BIT);
                } else {
                    // both operands positive, result negative --> overflow
                    if ((val1 + val2) & 0x80)
                        setFlag(PV_BIT);
                    else
                        clearFlag(PV_BIT);
                }
            }
          break;
        case ADC:
            // operands with different signs --> no overflow
            if ((val1 & 0x80) != (val2 & 0x80)) {
                clearFlag(PV_BIT);
            } else {
                // both operands negative, result positive --> overflow
                if ((val1 & 0x80)) {
                    if ((val1 + val2 + testFlag(C_BIT)) & 0x80)
                        clearFlag(PV_BIT);
                    else
                        setFlag(PV_BIT);
                } else {
                    // both operands positive, result negative --> overflow
                    if ((val1 + val2 + testFlag(C_BIT)) & 0x80)
                        setFlag(PV_BIT);
                    else
                        clearFlag(PV_BIT);
                }
            }
          break;
        case SUB:
            // operands are same signs, no overflow
            if ((val1 & 0x80) == (val2 & 0x80)) {
                clearFlag(PV_BIT);
            // with different signs, need to check result
            } else {
                if (((uint32_t) val1 - (uint32_t) val2 > (uint32_t) 127) ||
                    ((uint32_t) val1 - (uint32_t) val2 < (uint32_t) -128))
                    setFlag(PV_BIT);
                else
                    clearFlag(PV_BIT);
            }
            break;
        case SBC:
            // operands are same signs, no overflow
            if ((val1 & 0x80) == (val2 & 0x80)) {
                clearFlag(PV_BIT);
            // with different signs, need to check result
            } else {
                if (((uint32_t) val1 - (uint32_t) val2 -
                     (uint32_t) testFlag(C_BIT) >
                     (uint32_t) 127)   ||
                    ((uint32_t) val1 - (uint32_t) val2 -
                     (uint32_t) testFlag(C_BIT) <
                     (uint32_t) -128))
                        setFlag(PV_BIT);
                else
                        clearFlag(PV_BIT);
            }
            break;
        default:  // NONE - Flag not affected
          break;
    }
}

void Z80::update_V16(INST_TYPE t, uint16_t val1, uint16_t val2) {
    switch (t) {
        case ADD:
            // operands with different signs --> no overflow
            if ((val1 & 0x8000) != (val2 & 0x8000)) {
                clearFlag(PV_BIT);
            } else {
                // both operands negative, result positive --> overflow
                if ((val1 & 0x8000)) {
                    if ((val1 + val2) & 0x8000)
                        clearFlag(PV_BIT);
                    else
                        setFlag(PV_BIT);
                } else {
                    // both operands positive, result negative --> overflow
                    if ((val1 + val2) & 0x8000)
                        setFlag(PV_BIT);
                    else
                        clearFlag(PV_BIT);
                }
            }
          break;
        case ADC:
            // operands with different signs --> no overflow
            if ((val1 & 0x8000) != (val2 & 0x8000)) {
                clearFlag(PV_BIT);
            } else {
                // both operands negative, result positive --> overflow
                if ((val1 & 0x8000)) {
                    if ((val1 + val2 + testFlag(C_BIT)) & 0x8000)
                        clearFlag(PV_BIT);
                    else
                        setFlag(PV_BIT);
                } else {
                    // both operands positive, result negative --> overflow
                    if ((val1 + val2 + testFlag(C_BIT)) & 0x8000)
                        setFlag(PV_BIT);
                    else
                        clearFlag(PV_BIT);
                }
            }
          break;
        case SUB:
            // operands are same signs, no overflow
            if ((val1 & 0x8000) == (val2 & 0x8000)) clearFlag(PV_BIT);
            // with different signs, need to check result
            else if (((uint32_t) val1 - (uint32_t) val2 > (uint32_t) 0x7fff) ||
                     ((uint32_t) val1 - (uint32_t) val2 < (uint32_t) -32768))
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            break;
        case SBC:
            // operands are same signs, no overflow
            if ((val1 & 0x8000) == (val2 & 0x8000)) {
                clearFlag(PV_BIT);
            // with different signs, need to check result
            } else {
                if (((int32_t) val1 - (int32_t) val2 -
                     (int32_t) testFlag(C_BIT) > (int32_t) 0x7fff)   ||
                    ((int32_t) val1 - (int32_t) val2 -
                     (int32_t) testFlag(C_BIT) < (int32_t) -32768))
                        setFlag(PV_BIT);
                else
                        clearFlag(PV_BIT);
            }
            break;
        default:  // NONE - Flag not affected
          break;
    }
}


void Z80::update_H(INST_TYPE t, const uint8_t val1, const uint8_t val2) {
    switch (t) {
        case ADD:
            if ((val1 & 0x0f) + (val2 & 0x0f) > 0x0f)
                setFlag(H_BIT);
            else
                clearFlag(H_BIT);
            break;
        case ADC:
            if ((val1 & 0x0f) + (val2 & 0x0f) + testFlag(C_BIT) > 0x0f)
                setFlag(H_BIT);
            else
                clearFlag(H_BIT);
            break;
        case SUB:
            if ((val1 & 0x0f) < (val2 & 0x0f))
                setFlag(H_BIT);
            else
                clearFlag(H_BIT);
            break;
        case SBC:
            if ((val1 & 0x0f) < ((val2 & 0x0f) + testFlag(C_BIT)))
                setFlag(H_BIT);
            else
                clearFlag(H_BIT);
            break;
        default:
            break;
    }
}

void Z80::update_P(const uint8_t v) {
    int count = 0;

    if (v & 0x01) count++;
    if (v & 0x02) count++;
    if (v & 0x04) count++;
    if (v & 0x08) count++;
    if (v & 0x10) count++;
    if (v & 0x20) count++;
    if (v & 0x40) count++;
    if (v & 0x80) count++;

    if (count % 2) clearFlag(PV_BIT);  // odd
    else           setFlag(PV_BIT);    // even
}

void Z80::update_Z(const uint8_t val) {
    if (val == 0)
        setFlag(Z_BIT);
    else
        clearFlag(Z_BIT);
}

void Z80::update_S(const uint8_t val) {
    if (val & 0x80)
        setFlag(S_BIT);
    else
        clearFlag(S_BIT);
}

uint16_t Z80::getIX() {
    return (IXH << 8) + IXL;
}

uint16_t Z80::getIY() {
    return (IYH << 8) + IYL;
}

uint16_t Z80::getBC() {
    return (B << 8) + C;
}

uint16_t Z80::getDE() {
    return (D << 8) + E;
}

uint16_t Z80::getHL() {
    return (H << 8) + L;
}

void Z80::setIX(uint16_t v) {
    IXH = ((v & 0xff00)>>8);
    IXL = v & 0x00ff;
}

void Z80::setIY(uint16_t v) {
    IYH = ((v & 0xff00)>>8);
    IYL = v & 0x00ff;
}

void Z80::setBC(uint16_t v) {
    B = ((v & 0xff00)>>8);
    C = v & 0x00ff;
}

void Z80::setDE(uint16_t v) {
    D = ((v & 0xff00)>>8);
    E = v & 0x00ff;
}

void Z80::setHL(uint16_t v) {
    H = ((v & 0xff00)>>8);
    L = v & 0x00ff;
}

void Z80::setSP(uint8_t msb, uint8_t lsb) {
    SP = (msb << 8) + lsb;
}

void Z80::setPC(uint8_t msb, uint8_t lsb) {
    PC = (msb << 8) + lsb;
}

uint16_t Z80::getIndexReg(INDEX_REG r) {
  if (r == IX_REGISTER)
    return getIX();
  else
    return getIY();
}

void Z80::setIndexReg(INDEX_REG r, uint16_t v) {
  if (r == IX_REGISTER)
    setIX(v);
  else
    setIY(v);
}
