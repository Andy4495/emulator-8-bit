/* Homemade CPU Emulator 
   Copyright 2025 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   See https://github.com/Andy4495/Homemade-CPU for the CPU this emulates

   This code is based off version 2 of the CPU design.

   0.1  15-Oct-2025  Andy4495  Initial Creation


*/

#include "HomemadeCPU.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
using std::cout;
using std::endl;
using std::hex;
using std::setw;

void HomemadeCPU::execute() {
    uint16_t result;
    uint8_t  temp_bit;

    switch (IR) {
        case 0x00: case 0x01: case 0x02: case 0x03:     // LDAL #d
        case 0x04: case 0x05: case 0x06: case 0x07:
        case 0x08: case 0x09: case 0x0a: case 0x0b:
        case 0x0c: case 0x0d: case 0x0e: case 0x0f:
            AA = (AA & 0xF0) | (IR & 0x0F);
            break;

        case 0x10: case 0x11: case 0x12: case 0x13:     // LDAH #d
        case 0x14: case 0x15: case 0x16: case 0x17:
        case 0x18: case 0x19: case 0x1a: case 0x1b:
        case 0x1c: case 0x1d: case 0x1e: case 0x1f: 
            AA = (AA & 0x0F) | ((IR & 0x0F) << 4);
            break;

        case 0x20: case 0x21: case 0x22: case 0x23:     // LDBL #d
        case 0x24: case 0x25: case 0x26: case 0x27:
        case 0x28: case 0x29: case 0x2a: case 0x2b:
        case 0x2c: case 0x2d: case 0x2e: case 0x2f:
            AB = (AB & 0xF0) | (IR & 0x0F);
            break;

        case 0x30: case 0x31: case 0x32: case 0x33:     // LDBH #d
        case 0x34: case 0x35: case 0x36: case 0x37:
        case 0x38: case 0x39: case 0x3a: case 0x3b:
        case 0x3c: case 0x3d: case 0x3e: case 0x3f:
            AB = (AB & 0x0F) | ((IR & 0x0F) << 4);
            break;

        case 0x40: case 0x41: case 0x42: case 0x43:     // LJLL #d
        case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x48: case 0x49: case 0x4a: case 0x4b:
        case 0x4c: case 0x4d: case 0x4e: case 0x4f:
            JL = (JL & 0xF0) | (IR & 0x0F);
            break;

        case 0x50: case 0x51: case 0x52: case 0x53:     // LJLH #d
        case 0x54: case 0x55: case 0x56: case 0x57:
        case 0x58: case 0x59: case 0x5a: case 0x5b:
        case 0x5c: case 0x5d: case 0x5e: case 0x5f:            
            JL = (JL & 0x0F) | ((IR & 0x0F) << 4);
            break;

        case 0x60: case 0x61: case 0x62: case 0x63:     // LJHL #d
        case 0x64: case 0x65: case 0x66: case 0x67:
        case 0x68: case 0x69: case 0x6a: case 0x6b:
        case 0x6c: case 0x6d: case 0x6e: case 0x6f:
            JH = (JH & 0xF0) | (IR & 0x0F);
            break;

        case 0x70: case 0x71: case 0x72: case 0x73:     // LLHH #d
        case 0x74: case 0x75: case 0x76: case 0x77:
        case 0x78: case 0x79: case 0x7a: case 0x7b:
        case 0x7c: case 0x7d: case 0x7e: case 0x7f:
            JH = (JH & 0x0F) | ((IR & 0x0F) << 4);
            break;

        case 0x80:                                       // BCAA    0
            AA = AA & 0xFE;
            break;
        
        case 0x81:                                       // BCAA    1
            AA = AA & 0xFD;
            break;
        
        case 0x82:                                       // BCAA    2
            AA = AA & 0xFB;
            break;
        
        case 0x83:                                       // BCAA    3
            AA = AA & 0xF7;
            break;
        
        case 0x84:                                       // BCAA    4
            AA = AA & 0xEF;
            break;
        
        case 0x85:                                       // BCAA    5
            AA = AA & 0xDF;
            break;
        
        case 0x86:                                       // BCAA    6
            AA = AA & 0xBF;
            break;
        
        case 0x87:                                       // BCAA    7
            AA = AA & 0x7F;
            break;
        
        case 0x88:                                       // BSAA    0
            AA = AA | 0x01;
            break;
        
        case 0x89:                                       // BSAA    1
            AA = AA | 0x02;
            break;
        
        case 0x8A:                                       // BSAA    2
            AA = AA | 0x04;
            break;
        
        case 0x8B:                                       // BSAA    3
            AA = AA | 0x08;
            break;
        
        case 0x8C:                                       // BSAA    4
            AA = AA | 0x10;
            break;
        
        case 0x8D:                                       // BSAA    5
            AA = AA | 0x20;
            break;
        
        case 0x8E:                                       // BSAA    6
            AA = AA | 0x40;
            break;
        
        case 0x8F:                                       // BSAA    7
            AA = AA | 0x80;
            break;
        
        case 0x94:                                       // BCFL    4
            FL = FL & 0xEF;
            break;
        
        case 0x95:                                       // BCFL    5
            FL = FL & 0xDF;
            break;
        
        case 0x96:                                       // BCFL    6
            FL = FL & 0xBF;
            break;
        
        case 0x97:                                       // BCFL    7
            FL = FL & 0x7F;
            break;

        case 0x9C:                                       // BSFL    4
            FL = FL | 0x10;
            break;
        
        case 0x9D:                                       // BSFL    5
            FL = FL | 0x20;
            break;
        
        case 0x9E:                                       // BSFL    6
            FL = FL | 0x40;
            break;
        
        case 0x9F:                                       // BSFL    7
            FL = FL | 0x80;
            break;

        case 0xA0:                                       // LDA0
            AA = 0x00;
            break;

        case 0xA1:                                       // LDA1
            AA = 0x01;
            break;

        case 0xA3:                                       // LDAF
            AA = 0xFF;
            break;

        case 0xA4:                                       // LDB0
            AB = 0x00;
            break;

        case 0xA5:                                       // LDB1
            AB = 0x01;
            break;

        case 0xA7:                                       // LDBF
            AB = 0xFF;
            break;

        case 0xA8:                                       // LDSF
            SP = 0xFFFF;
            break;

        case 0xC0:                                       // LOAD (MR)
            AA = memory[getMR()];
            break;

        case 0xC1:                                       // LOAD AB
            AA = AB;
            break;

        case 0xC2:                                       // LOAD SL
            AA = getSL();
            break;

        case 0xC3:                                       // LOAD SH
            AA = getSH();
            break;

        case 0xC4:                                       // LOAD ML
            AA = ML;
            break;

        case 0xC5:                                       // LOAD MH
            AA = MH;
            break;

        case 0xC6:                                       // LOAD JL
            AA = JL;
            break;

        case 0xC7:                                       // LOAD JH
            AA = JH;
            break;

        case 0xC8:                                       // STOR (MR)
            memory[getMR()] = AA;
            break;

        case 0xC9:                                       // STOR AB
            AB = AA;
            break;

        case 0xCA:                                       // STOR SL
            setSL(AA);
            break;

        case 0xCB:                                       // STOR SH
            setSH(AA);
            break;

        case 0xCC:                                       // STOR ML
            ML = AA;
            break;

        case 0xCD:                                       // STOR MH
            MH = AA;
            break;

        case 0xCE:                                       // STOR JL
            JL = AA;
            break;

        case 0xCF:                                       // STOR JH
            JH = AA;
            break;
    
        case 0xD0:                                       // PUSH AA
            memory[SP--] = AA;
            break;

        case 0xD1:                                       // PUSH AB
            memory[SP--] = AB;
            break;

        case 0xD2:                                       // PUSH SL
            memory[SP--] = getSL();
            break;

        case 0xD3:                                       // PUSH SH
            memory[SP--] = getSH();
            break;

        case 0xD4:                                       // PUSH ML
            memory[SP--] = ML;
            break;

        case 0xD5:                                       // PUSH MH
            memory[SP--] = MH;
            break;

        case 0xD6:                                       // PUSH JL
            memory[SP--] = JL;
            break;

        case 0xD7:                                       // PUSH JH
            memory[SP--] = JH;
            break;

        case 0xD8:                                       // POP AA
            AA = memory[++SP];
            break;

        case 0xD9:                                       // POP AB
            AB = memory[++SP];
            break;

        case 0xDA:                                       // POP SL
            setSL(memory[++SP]);
            break;

        case 0xDB:                                       // POP SH
            setSH(memory[++SP]);
            break;

        case 0xDC:                                       // POP ML
            ML = memory[++SP];
            break;

        case 0xDD:                                       // POP MH
            MH = memory[++SP];
            break;

        case 0xDE:                                       // POP JL
            JL = memory[++SP];
            break;

        case 0xDF:                                       // POP JH
            JH = memory[++SP];
            break;

        case 0xE0:                                       // COMP #dd - result not saved
            result = AA - AB - testFlag(C_BIT);
            update_Z(result);
            if ((AB + testFlag(C_BIT)) > AA) setFlag(C_BIT);
            else                             clearFlag(C_BIT);
            update_S(result);
            // Overflow algorithm:
            // Subtraction: minuend - subtrahend = difference
            //   - If the minuend and subtrahend are the same sign, then there is no overlow
            //   - If the minuend and subtrahend have different signs, then:
            //     - If difference is the same sign as the subtrahend, then overflow
            //     - If difference is different sign than subtrahend, then no overflow
            //
            // operands are same signs, no overflow
            if ((AA & 0x80) == (AB & 0x80)) {
                clearFlag(V_BIT);
            // different signs, compare subtrahend and difference signs
            // same sign -> overflow
            } else {
                if ( ((AA - AB - testFlag(C_BIT)) & 0x80) == (AB & 0x80) )
                        setFlag(V_BIT);
                else
                        clearFlag(V_BIT);
            }
            break;

        case 0xE1:                                       // SUBB #dd
            result = AA - AB - testFlag(C_BIT);;
            update_Z(result);
            if ((AB + testFlag(C_BIT)) > AA) setFlag(C_BIT);
            else                             clearFlag(C_BIT);
            update_S(result);
            // Overflow algorithm:
            // Subtraction: minuend - subtrahend = difference
            //   - If the minuend and subtrahend are the same sign, then there is no overlow
            //   - If the minuend and subtrahend have different signs, then:
            //     - If difference is the same sign as the subtrahend, then overflow
            //     - If difference is different sign than subtrahend, then no overflow
            //
            // operands are same signs, no overflow
            if ((AA & 0x80) == (AB & 0x80)) {
                clearFlag(V_BIT);
            // different signs, compare subtrahend and difference signs
            // same sign -> overflow
            } else {
                if ( ((AA - AB - testFlag(C_BIT)) & 0x80) == (AB & 0x80) )
                        setFlag(V_BIT);
                else
                        clearFlag(V_BIT);
            }
            AA = result;
            break;

        case 0xE2:                                       // ADDD #dd
            result = AA + AB + testFlag(C_BIT);
            update_Z(result);
            if ((AA + AB + testFlag(C_BIT)) & 0x0100) setFlag(C_BIT);
            else                                      clearFlag(C_BIT);
            update_S(result);
            // Overflow algorithm:
            // Addition: addend + addend = sum
            //   - If the addends have different signs, then there is no overflow
            //   - If the addends have the same signes, then:
            //     - If sum is different sign than the addends, then overflow
            //     - If sum is same sign as addends, then no overflow
            //
            // operands with different signs --> no overflow
            if ((AA & 0x80) != (AB & 0x80)) {
                clearFlag(V_BIT);
            } else {
                // both operands negative, result positive --> overflow
                if ((AA & 0x80)) {
                    if ((AA + AB + testFlag(C_BIT)) & 0x80)
                        clearFlag(V_BIT);
                    else
                        setFlag(V_BIT);
                } else {
                    // both operands positive, result negative --> overflow
                    if ((AA + AB + testFlag(C_BIT)) & 0x80)
                        setFlag(V_BIT);
                    else
                        clearFlag(V_BIT);
                }
            }
            AB = result;
            break;

        case 0xE3:                                       // ANDD #dd
            AA = AA & AB;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;  
            
        case 0xE4:                                       // ORRR #dd
            AA = AA | AB;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0xE5:                                       // XORR #dd
            AA = AA ^ AB;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0xE6:                                       // NAND #dd
            AA = ~(AA & AB);
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0xE7:                                       // NORR #dd
            AA = ~(AA | AB);
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0xE8:                                       // NOTT
            AA = ~AA;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;
            
        case 0xE9:                                       // NEGG
            AA = 0 - AA;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0xEA:                                       // SHRL
            if (AA & 0x01) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AA = AA >> 1;
            update_Z(AA);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0xEB:                                      // SHLL
            if (AA & 0x80) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AA = AA << 1;
            update_Z(AA);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0xEC:                                      // SHRA
            if (AA & 0x80) temp_bit = 0x80;
            else           temp_bit = 0x00;
            if (AA & 0x01) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AA = AA >> 1;
            AA |= temp_bit;
            update_S(AA);
            update_Z(AA);
            clearFlag(V_BIT);
            break;
            
        case 0xED:                                      // SRLC
            if (testFlag(C_BIT)) temp_bit = 0x80;
            else                 temp_bit = 0x00;
            if (AA & 0x01) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AA = AA >> 1;
            AA = AA | temp_bit;
            update_Z(AA);
            update_S(AA);
            clearFlag(V_BIT);
            break; 
            
        case 0xEE:                                      // SLLC
            if (testFlag(C_BIT)) temp_bit = 0x01;
            else                 temp_bit = 0x00;
            if (AA & 0x80) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AA = AA << 1;
            AA = AA | temp_bit;
            update_Z(AA);
            update_S(AA);
            clearFlag(V_BIT);
            break;


        // Note that for all jumps, there will need to be a branch delay slot
        // that will be executed after all jumps
        case 0xF0:                                      // JPVC
            if (!(FL & V_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0xF1:                                      // JPVS
            if ((FL & V_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0xF2:                                      // JPCC
            if (!(FL & C_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0xF3:                                      // JPCS
            if ((FL & C_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0xF4:                                      // JPZC
            if (!(FL & Z_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0xF5:                                      // JPZS
            if (FL & Z_BIT) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0xF6:                                      // JPSC
            if (!(FL & S_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0xF7:                                      // JPSS
            if (FL & S_BIT) {
                setPC(JH, AB);
                delay_slot = true; 
            }
            break;

        case 0xF8:                                      // INCA
            AA++;
            break;

        case 0xF9:                                      // DECA
            AA--;
            break;

        case 0xFA:                                      // INCB
            AB++;
            break;

        case 0xFB:                                      // DECB
            AB--;
            break;

        case 0xFC:                                      // INCM
            result = getMR() + 1;
            MH = (result & 0xff00) >> 8;
            ML = result & 0x00ff;
            break;

        case 0xFD:                                      // JUMP
            setPC(JH, JL);
            delay_slot = true; 
            break;

        case 0xFE:                                      // NOOP
            break;

        case 0xFF:                                      // HALT
            Halt = true;
            break;

        default:
            cout << "Unsupported opcode: 0x" << hex << setw(2) << (unsigned int) IR << endl;
    }

}