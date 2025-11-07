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
    uint8_t  temp_reg;

    switch (IR) {
        case 0x00:                                       // NOOP
            break;

        case 0x04:                                       // BCFL    4 ; BCFL V
            FL = FL & 0xEF;
            break;
        
        case 0x05:                                       // BCFL    5 ; BCFL V
            FL = FL & 0xDF;
            break;
        
        case 0x06:                                       // BCFL    6 ; BCFL C
            FL = FL & 0xBF;
            break;
        
        case 0x07:                                       // BCFL    7 ; BCFL Z
            FL = FL & 0x7F;
            break;

        case 0x0C:                                       // BSFL    4 ; BSFL S
            FL = FL | 0x10;
            break;
        
        case 0x0D:                                       // BSFL    5 ; BSFL C
            FL = FL | 0x20;
            break;
        
        case 0x0E:                                       // BSFL    6 ; BSFL Z
            FL = FL | 0x40;
            break;
        
        case 0x0F:                                       // BSFL    7 ; BSFL S
            FL = FL | 0x80;
            break;

        case 0x10:                                       // BCAA    0
            AA = AA & 0xFE;
            break;
        
        case 0x11:                                       // BCAA    1
            AA = AA & 0xFD;
            break;
        
        case 0x12:                                       // BCAA    2
            AA = AA & 0xFB;
            break;
        
        case 0x13:                                       // BCAA    3
            AA = AA & 0xF7;
            break;
        
        case 0x14:                                       // BCAA    4
            AA = AA & 0xEF;
            break;
        
        case 0x15:                                       // BCAA    5
            AA = AA & 0xDF;
            break;
        
        case 0x16:                                       // BCAA    6
            AA = AA & 0xBF;
            break;
        
        case 0x17:                                       // BCAA    7
            AA = AA & 0x7F;
            break;
        
        case 0x18:                                       // BSAA    0
            AA = AA | 0x01;
            break;
        
        case 0x19:                                       // BSAA    1
            AA = AA | 0x02;
            break;
        
        case 0x1A:                                       // BSAA    2
            AA = AA | 0x04;
            break;
        
        case 0x1B:                                       // BSAA    3
            AA = AA | 0x08;
            break;
        
        case 0x1C:                                       // BSAA    4
            AA = AA | 0x10;
            break;
        
        case 0x1D:                                       // BSAA    5
            AA = AA | 0x20;
            break;
        
        case 0x1E:                                       // BSAA    6
            AA = AA | 0x40;
            break;
        
        case 0x1F:                                       // BSAA    7
            AA = AA | 0x80;
            break;
        
        case 0x20:                                       // LOAD (MR)
            AA = memory[getMR()];
            break;

        case 0x21:                                       // STOR (MR)
            memory[getMR()] = AA;
            break;

        // Note that for all jumps, there will need to be a branch delay slot
        // that will be executed after all jumps
        case 0x24:                                       // JUMP
            setPC(JH, JL);
            delay_slot = true; 
            break;

        case 0x28:                                       // JPVC
            if (!(FL & V_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0x29:                                       // JPVS
            if ((FL & V_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0x2A:                                       // JPCC
            if (!(FL & C_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0x2B:                                       // JPCS
            if ((FL & C_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0x2C:                                       // JPZC
            if (!(FL & Z_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0x2D:                                       // JPZS
            if (FL & Z_BIT) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0x2E:                                       // JPSC
            if (!(FL & S_BIT)) {
                setPC(JH, JL);
                delay_slot = true; 
            }
            break;

        case 0x2F:                                       // JPSS
            if (FL & S_BIT) {
                setPC(JH, AB);
                delay_slot = true; 
            }
            break;

        case 0x30:                                       // PUSH AA
            memory[SP--] = AA;
            break;

        case 0x32:                                       // PUSH FL
            memory[SP--] = FL & 0xF0;
            break;

        case 0x38:                                       // POP AA
            AA = memory[++SP];
            break;

        case 0x3A:                                       // POP FL
            FL = memory[++SP] & 0xF0;
            break;

        case 0x40:                                       // EXCH
            temp_reg = AB;
            AB = AA;
            AA = temp_reg;
            break;

        case 0x41:                                       // MOVE AB->AA
            AA = AB;
            break;

        case 0x42:                                       // MOVE SL->AA
            AA = getSL();
            break;

        case 0x43:                                       // MOVE SH->AA
            AA = getSH();
            break;

        case 0x44:                                       // MOVE ML->AA
            AA = ML;
            break;

        case 0x45:                                       // MOVE MH->AA
            AA = MH;
            break;

        case 0x46:                                       // MOVE JL->AA
            AA = JL;
            break;

        case 0x47:                                       // MOVE JH->AA
            AA = JH;
            break;

        case 0x49:                                       // MOVE AA->AB
            AB = AA;
            break;

        case 0x4A:                                       // MOVE AA->SL
            setSL(AA);
            break;

        case 0x4B:                                       // MOVE AA->SH
            setSH(AA);
            break;

        case 0x4C:                                       // MOVE AA->ML
            ML = AA;
            break;

        case 0x4D:                                       // MOVE AA->MH
            MH = AA;
            break;

        case 0x4E:                                       // MOVE AA->JL
            JL = AA;
            break;

        case 0x4F:                                       // MOVE AA->JH
            JH = AA;
            break;
    
        case 0x50:                                       // COMP
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

        case 0x51:                                       // SUBB
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

        case 0x52:                                       // ADDD
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
            AA = result;
            break;

        case 0x53:                                       // ANDD
            AA = AA & AB;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;  
            
        case 0x54:                                       // ORRR
            AA = AA | AB;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x55:                                       // XORR
            AA = AA ^ AB;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x56:                                       // NAND
            AA = ~(AA & AB);
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x57:                                       // NORR
            AA = ~(AA | AB);
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x58:                                       // NOTT
            AA = ~AA;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;
            
        case 0x59:                                       // NEGA
            AA = 0 - AA;
            update_Z(AA);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x5A:                                       // SHRL
            if (AA & 0x01) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AA = AA >> 1;
            update_Z(AA);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x5B:                                       // SHLL
            if (AA & 0x80) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AA = AA << 1;
            update_Z(AA);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x5C:                                       // SHRA
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
            
        case 0x5D:                                       // SRLC
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
            
        case 0x5E:                                       // SLLC
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

        case 0x60:                                       // INCA
            AA++;
            break;

        case 0x61:                                       // DECA
            AA--;
            break;

        case 0x62:                                       // INCB
            AB++;
            break;

        case 0x63:                                       // DECB
            AB--;
            break;

        case 0x64:                                       // INCM
            result = getMR() + 1;
            MH = (result & 0xff00) >> 8;
            ML = result & 0x00ff;
            break;

        case 0x68:                                       // LDA0
            AA = 0x00;
            break;

        case 0x69:                                       // LDA1
            AA = 0x01;
            break;

        case 0x6B:                                       // LDAF
            AA = 0xFF;
            break;

        case 0x6C:                                       // LDB0
            AB = 0x00;
            break;

        case 0x6D:                                       // LDB1
            AB = 0x01;
            break;

        case 0x6F:                                       // LDBF
            AB = 0xFF;
            break;

        case 0x7F:                                       // HALT
            Halt = true;
            break;


        case 0x80: case 0x81: case 0x82: case 0x83:      // LDAL #d
        case 0x84: case 0x85: case 0x86: case 0x87:
        case 0x88: case 0x89: case 0x8a: case 0x8b:
        case 0x8c: case 0x8d: case 0x8e: case 0x8f:
            AA = (AA & 0xF0) | (IR & 0x0F);
            break;

        case 0x90: case 0x91: case 0x92: case 0x93:      // LDAH #d
        case 0x94: case 0x95: case 0x96: case 0x97:
        case 0x98: case 0x99: case 0x9a: case 0x9b:
        case 0x9c: case 0x9d: case 0x9e: case 0x9f: 
            AA = (AA & 0x0F) | ((IR & 0x0F) << 4);
            break;

        case 0xA0: case 0xA1: case 0xA2: case 0xA3:      // LDBL #d
        case 0xA4: case 0xA5: case 0xA6: case 0xA7:
        case 0xA8: case 0xA9: case 0xAa: case 0xAb:
        case 0xAc: case 0xAd: case 0xAe: case 0xAf:
            AB = (AB & 0xF0) | (IR & 0x0F);
            break;

        case 0xB0: case 0xB1: case 0xB2: case 0xB3:      // LDBH #d
        case 0xB4: case 0xB5: case 0xB6: case 0xB7:
        case 0xB8: case 0xB9: case 0xBa: case 0xBb:
        case 0xBc: case 0xBd: case 0xBe: case 0xBf:
            AB = (AB & 0x0F) | ((IR & 0x0F) << 4);
            break;

        case 0xC0: case 0xC1: case 0xC2: case 0xC3:      // LJLL #d
        case 0xC4: case 0xC5: case 0xC6: case 0xC7:
        case 0xC8: case 0xC9: case 0xCa: case 0xCb:
        case 0xCc: case 0xCd: case 0xCe: case 0xCf:
            JL = (JL & 0xF0) | (IR & 0x0F);
            break;

        case 0xD0: case 0xD1: case 0xD2: case 0xD3:      // LJLH #d
        case 0xD4: case 0xD5: case 0xD6: case 0xD7:
        case 0xD8: case 0xD9: case 0xDa: case 0xDb:
        case 0xDc: case 0xDd: case 0xDe: case 0xDf:            
            JL = (JL & 0x0F) | ((IR & 0x0F) << 4);
            break;

        case 0xE0: case 0xE1: case 0xE2: case 0xE3:      // LJHL #d
        case 0xE4: case 0xE5: case 0xE6: case 0xE7:
        case 0xE8: case 0xE9: case 0xEa: case 0xEb:
        case 0xEc: case 0xEd: case 0xEe: case 0xEf:
            JH = (JH & 0xF0) | (IR & 0x0F);
            break;

        case 0xF0: case 0xF1: case 0xF2: case 0xF3:      // LLHH #d
        case 0xF4: case 0xF5: case 0xF6: case 0xF7:
        case 0xF8: case 0xF9: case 0xFa: case 0xFb:
        case 0xFc: case 0xFd: case 0xFe: case 0xFf:
            JH = (JH & 0x0F) | ((IR & 0x0F) << 4);
            break;

        default:
        cout << "Unsupported opcode: 0x" << hex << setw(2) << (unsigned int) IR << endl;
    }

}