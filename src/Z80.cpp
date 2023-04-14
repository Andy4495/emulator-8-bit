/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   v0.1.0    02/11/23  Andy4495 Read for first "release"
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
*/

#include "Z80.h"
#include "Z80_opcodes.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cassert>
#include <cstdint>
using std::cout;
using std::endl;
using std::hex;
using std::ifstream;
using std::ofstream;
using std::iostream;
using std::ios;
using std::setw;

Z80::Z80(uint16_t ramstart, uint16_t ramend,
         uint16_t romstart, uint16_t romend) {
    _ramstart = ramstart;
    _ramend   = ramend;
    _romstart = romstart;
    _romend   = romend;
    // Load the input port with some dummy data to make it easier to test
        in[0x96] = 'i';  // 0x69 - even parity
        in[0x97] = 'h';  // 0x68 - odd parity
        in[0x98] = 0x80;
        in[0x99] = 0x00;
}

void Z80::load_memory(const char* fname) {
    // Load memory from filename fname
    unsigned int i = 0;
    char data;
    uint8_t msb, lsb;

    ifstream memfile(fname, iostream::in|ios::binary);
    assert(memfile);  // Check if open successful

    while (memfile.read(&data, 1)) {
        memory[i++] = data;
        if (i > MAX_MEMORY - 1) {
            cout << "Loading registers . . . ";
            // Add registers to end of file.
            memfile >> A;
            memfile >> F;
            memfile >> B;
            memfile >> C;
            memfile >> D;
            memfile >> E;
            memfile >> H;
            memfile >> L;
            memfile >> Aprime;
            memfile >> Fprime;
            memfile >> Bprime;
            memfile >> Cprime;
            memfile >> Dprime;
            memfile >> Eprime;
            memfile >> Hprime;
            memfile >> Lprime;
            memfile >> I;
            memfile >> R;
            memfile >> IXH;
            memfile >> IXL;
            memfile >> IYH;
            memfile >> IYL;
            memfile >> msb;
            memfile >> lsb;
            setSP(msb, lsb);
            memfile >> msb;
            memfile >> lsb;
            setPC(msb, lsb);
            memfile >> IFF1;
            memfile >> IFF2;
            memfile >> INT_MODE;
            break;    // Make sure we don't try to load beyond memory space
        }
    }
}

void Z80::dump_memory_to_file(const char* fname) {
    
    ofstream memfile(fname, iostream::out|ios::binary);
    assert(memfile);  // Check if open successful
    for (uint32_t i = 0; i <= 0xffff; i++) {
        memfile << memory[i];
    }
    // Add registers to end of file.
    memfile << A;
    memfile << F;
    memfile << B;
    memfile << C;
    memfile << D;
    memfile << E;
    memfile << H;
    memfile << L;
    memfile << Aprime;
    memfile << Fprime;
    memfile << Bprime;
    memfile << Cprime;
    memfile << Dprime;
    memfile << Eprime;
    memfile << Hprime;
    memfile << Lprime;
    memfile << I;
    memfile << R;
    memfile << IXH;
    memfile << IXL;
    memfile << IYH;
    memfile << IYL;
    memfile << (uint8_t) ((SP & 0xff00) >> 8);
    memfile << (uint8_t)  (SP & 0xff);
    memfile << (uint8_t) ((PC & 0xff00) >> 8); 
    memfile << (uint8_t)  (PC & 0xff);
    memfile << IFF1;
    memfile << IFF2;
    memfile << INT_MODE;

    memfile.close();
}

void Z80::cold_reset() {
    // Clear RAM and registers, start from $0000
    // CPU is initialized as follows:
    // PC, IFF1, IFF2 set to 0
    // Other registers set to $FF/$FFFF
    // Interrupt Mode set to 0
    // RAM is left as-is
    cout << "Cold Reset: PC, I, R, IFF1/2 set to zero, other registers set to $FF, Interrupt Mode 0. "
         << endl;
    init_registers();
    Halt = false;
}

void Z80::warm_reset() {
    // Keep RAM intact, clear registers
    cout << "Warm Reset (aka Special Reset): PC set to $0000, other registers unchanged." << endl;
    PC = 0x0000;
    Halt = false;
}

void Z80::init_registers() {
    A    = 0xFF;
    F    = 0xD7;    // Leave bits 5 and 3 clear (XF and YF)
    B    = 0xFF; C = 0xFF; D = 0xFF; E = 0xFF; H = 0xFF; L = 0xFF;
    Aprime  = 0xFF;
    Fprime  = 0xD7;    // Leave bits 5 and 3 clear (XF and YF)
    Bprime  = 0xFF; Cprime = 0xFF; Dprime = 0xFF; Eprime = 0xFF; Hprime = 0xFF; Lprime = 0xFF;
    I = 0;
    R = 0;
    IXH = 0xFF; IXL = 0xFF; IYH = 0xFF; IYL = 0xFF;
    SP = 0xFF;
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
    cout << hex << "A:  " << setw(2) << (uint16_t) A
    <<  " F:  "  << setw(2) << (uint16_t) F
    <<  " B:  "  << setw(2) << (uint16_t) B
    <<  " C:  "  << setw(2) << (uint16_t) C
    <<  " D:  "  << setw(2) << (uint16_t) D
    <<  " E:  "  << setw(2) << (uint16_t) E
    <<  " H:  "  << setw(2) << (uint16_t) H
    <<  " L:  "  << setw(2) << (uint16_t) L << endl
    <<  "A': "   << setw(2) << (uint16_t) Aprime
    <<  " F': "  << setw(2) << (uint16_t) Fprime
    <<  " B': "  << setw(2) << (uint16_t) Bprime
    <<  " C': "  << setw(2) << (uint16_t) Cprime
    <<  " D': "  << setw(2) << (uint16_t) Dprime
    <<  " E': "  << setw(2) << (uint16_t) Eprime
    <<  " H': "  << setw(2) << (uint16_t) Hprime
    <<  " L': "  << setw(2) << (uint16_t) Lprime << endl
    <<  "I:  "   << setw(2) << (uint16_t) I
    << " R:  "   << setw(2) << (uint16_t) R
    << " IX: "   << setw(4) << (uint16_t) getIX()
    << " IY: "   << setw(4) << (uint16_t) getIY()
    << " SP: "   << setw(4) << (uint16_t) SP
    << " PC: "   << setw(4) << (uint16_t) PC
    << " IFF1: " << setw(2) << (uint16_t) IFF1
    << " IFF2: " << setw(2) << (uint16_t) IFF2
    << " INT_MODE: " << setw(2) << (uint16_t) INT_MODE
    << endl;
}

void Z80::print_flags() {
    cout << "SZ5H 3PNC: "
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
    // Overflow algorithm:
    // Addition: addend + addend = sum
    //   - If the addends have different signs, then there is no overflow
    //   - If the addends have the same signes, then:
    //     - If sum is different sign than the addends, then overflow
    //     - If sum is same sign as addends, then no overflow
    // Subtraction: minuend - subtrahend = difference
    //   - If the minuend and subtrahend are the same sign, then there is no overlow
    //   - If the minuend and subtrahend have different signs, then:
    //     - If difference is the same sign as the subtrahend, then overflow
    //     - If difference is different sign than subtrahend, then no overflow
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
            // different signs, compare subtrahend and difference signs
            // same sign -> overflow
            } else {
                if ( ((val1 - val2) & 0x80) == (val2 & 0x80) )
                    setFlag(PV_BIT);
                else
                    clearFlag(PV_BIT);
            }
            break;
        case SBC:
            // operands are same signs, no overflow
            if ((val1 & 0x80) == (val2 & 0x80)) {
                clearFlag(PV_BIT);
            // different signs, compare subtrahend and difference signs
            // same sign -> overflow
            } else {
                if ( ((val1 - val2 - testFlag(C_BIT)) & 0x80) == (val2 & 0x80) )
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
    // Overflow algorithm:
    // Addition: addend + addend = sum
    //   - If the addends have different signs, then there is no overflow
    //   - If the addends have the same signes, then:
    //     - If sum is different sign than the addends, then overflow
    //     - If sum is same sign as addends, then no overflow
    // Subtraction: minuend - subtrahend = difference
    //   - If the minuend and subtrahend are the same sign, then there is no overlow
    //   - If the minuend and subtrahend have different signs, then:
    //     - If difference is the same sign as the subtrahend, then overflow
    //     - If difference is different sign than subtrahend, then no overflow    switch (t) {
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
            // different signs, compare subtrahend and difference signs
            // same sign -> overflow
            else if ( ((val1 - val2) & 0x8000) == (val2 & 0x8000) )
                setFlag(PV_BIT);
            else
                clearFlag(PV_BIT);
            break;
        case SBC:
            // operands are same signs, no overflow
            if ((val1 & 0x8000) == (val2 & 0x8000)) {
                clearFlag(PV_BIT);
            // different signs, compare subtrahend and difference signs
            // same sign -> overflow
            } else {
                if ( ((val1 - val2 - testFlag(C_BIT)) & 0x8000) == (val2 & 0x8000) )
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
