/* Homemade CPU Emulator 
   Copyright 2025 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   See https://github.com/Andy4495/Homemade-CPU for the CPU this emulates

   0.1  09-Oct-2025  Andy4495  Initial Creation


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
        case 0x00:      // LOAD #dd
            AC = OR;
            break;

        case 0x08:      // LOAD (mm)
            AC = memory[getMA()];
            break;

        case 0x10:      // LOAD AC
//            Commenting this out to clear a compiler warning.
//            TBD whether the hardware actually implements the move
//            Either way is the net effect of this is a NOOP
//            AC = AC;
            break;

        case 0x11:      // LOAD FL
            AC = FL;
            break;

        case 0x12:      // LOAD SH
            AC = getSH(); 
            break;

        case 0x13:      // LOAD SL
            AC = getSL();
            break;

        case 0x14:      // LOAD MH
            AC = MH; 
            break;

        case 0x15:      // LOAD JH
            AC = JH;
            break;

        case 0x16:      // LOAD JL
            AC = JL;
            break;

        case 0x18:      // STOR (mm)
            memory[getMA()] = AC;
            break;

        case 0x20:      // STOR AC
//            Commenting this out to clear a compiler warning.
//            TBD whether the hardware actually implements the move
//            Either way is the net effect of this is a NOOP            
//            AC = AC; 
            break;

        case 0x21:      // STOR FL
            FL = AC;
            break;

        case 0x22:      // STOR SH
            setSH(AC); 
            break;

        case 0x23:      // STOR SL
            setSL(AC); 
            break;
        
        case 0x24:      // STOR MH
            MH = AC;
            break;

        case 0x25:      // STOR JH
            JH = AC; 
            break;

        case 0x26:      // STOR JL
            JL = AC;
            break;

        case 0x28:      // PUSH AC
            memory[SP--] = AC;
            break;

        case 0x29:      // PUSH FL
            memory[SP--] = FL;
            break;

        case 0x2a:      // PUSH SH
            memory[SP--] = getSH();
            break;

        case 0x2b:      // PUSH SL
            memory[SP--] = getSL();
            break;

        case 0x2c:      // PUSH MH
            memory[SP--] = MH;
            break;

        case 0x2d:      // PUSH JH
            memory[SP--] = JH;
            break;

        case 0x2e:      // PUSH JL
            memory[SP--] = JL;
            break;

        case 0x30:      // PUSH #dd
            memory[SP--] = OR;
            break;

        case 0x38:      // POPP AC
            AC = memory[SP++];
            break;

        case 0x39:      // POPP FL
            FL = memory[SP++]; 
            break;

        case 0x3a:      // POPP SH
            setSH(memory[SP++]);

        case 0x3b:      // POPP SL
            setSL(memory[SP++]);
            break;

        case 0x3c:      // POPP MH
            MH = memory[SP++];
            break;

        case 0x3d:      // POPP JH
            JH = memory[SP++];
            break;

        case 0x3e:      // POPP JL
            JL = memory[SP++];
            break;

        case 0x40:      // COMP #dd - result not saved
            result = AC - OR - testFlag(C_BIT);
            update_Z(result);
            if ((OR + testFlag(C_BIT)) > AC) setFlag(C_BIT);
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
            if ((AC & 0x80) == (OR & 0x80)) {
                clearFlag(V_BIT);
            // different signs, compare subtrahend and difference signs
            // same sign -> overflow
            } else {
                if ( ((AC - OR - testFlag(C_BIT)) & 0x80) == (OR & 0x80) )
                        setFlag(V_BIT);
                else
                        clearFlag(V_BIT);
            }
            break;

        case 0x42:      // SUBB #dd
            result = AC - OR - testFlag(C_BIT);;
            update_Z(result);
            if ((OR + testFlag(C_BIT)) > AC) setFlag(C_BIT);
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
            if ((AC & 0x80) == (OR & 0x80)) {
                clearFlag(V_BIT);
            // different signs, compare subtrahend and difference signs
            // same sign -> overflow
            } else {
                if ( ((AC - OR - testFlag(C_BIT)) & 0x80) == (OR & 0x80) )
                        setFlag(V_BIT);
                else
                        clearFlag(V_BIT);
            }
            AC = result;
            break;

        case 0x44:      // ADDD #dd
            result = AC + OR + testFlag(C_BIT);
            update_Z(result);
            if ((AC + OR + testFlag(C_BIT)) & 0x0100) setFlag(C_BIT);
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
            if ((AC & 0x80) != (OR & 0x80)) {
                clearFlag(V_BIT);
            } else {
                // both operands negative, result positive --> overflow
                if ((AC & 0x80)) {
                    if ((AC + OR + testFlag(C_BIT)) & 0x80)
                        clearFlag(V_BIT);
                    else
                        setFlag(V_BIT);
                } else {
                    // both operands positive, result negative --> overflow
                    if ((AC + OR + testFlag(C_BIT)) & 0x80)
                        setFlag(V_BIT);
                    else
                        clearFlag(V_BIT);
                }
            }
            AC = result;
            break;

        case 0x50:      // ANDD #dd
            AC = AC & OR;
            update_Z(AC);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;  
            
        case 0x52:      // ORRR #dd
            AC = AC | OR;
            update_Z(AC);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x54:      // XORR #dd
            AC = AC ^ OR;
            update_Z(AC);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x56:      // NAND #dd
            AC = ~(AC & OR);
            update_Z(AC);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x58:      // NORR #dd
            AC = ~(AC | OR);
            update_Z(AC);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x60:      // NOTT
            AC = ~AC;
            update_Z(AC);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x70:      // SHRL
            if (AC & 0x01) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AC = AC >> 1;
            update_Z(AC);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x72:      // SHLL
            if (AC & 0x80) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AC = AC << 1;
            update_Z(AC);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            break;

        case 0x74:      // SHRA
            if (AC & 0x80) temp_bit = 0x80;
            else           temp_bit = 0x00;
            if (AC & 0x01) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AC = AC >> 1;
            AC |= temp_bit;
            update_S(AC);
            update_Z(AC);
            clearFlag(V_BIT);
            break;
            
        case 0x78:      // ROTR
            if (AC & 0x01) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AC = AC >> 1;
            if (FL & C_BIT) AC |= 0x80;
            update_Z(AC);
            update_S(AC);
            clearFlag(V_BIT);
            break;

        case 0x7a:      // RRTC
            if (FL & C_BIT) temp_bit = 0x80;
            else            temp_bit = 0x00;
            if (AC & 0x01) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AC = AC >> 1;
            if (temp_bit) AC |= 0x80;
            update_Z(AC);
            update_S(AC);
            clearFlag(V_BIT);
            break;        

        case 0x7c:      // ROTL
            if (AC & 0x80) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AC = AC << 1;
            if (FL & C_BIT) AC |= 0x01;
            update_Z(AC);
            update_S(AC);
            clearFlag(V_BIT);
            break;            

        case 0x7e:      // RLTC
            if (FL & C_BIT) temp_bit = 0x01;
            else            temp_bit = 0x00;
            if (AC & 0x80) setFlag(C_BIT);
            else           clearFlag(C_BIT);
            AC = AC << 1;
            if (temp_bit) AC |= 0x01;
            update_Z(AC);
            update_S(AC);
            clearFlag(V_BIT);
            break; 

        // Note that the BITC/BITS commands may be alternatively be implemented 
        // with all the same opcode (one for BITC, one for BITS), and the operand
        // gives the mask for what is set/clear.
        case 0x80:      // BITC 0
            AC = AC & 0xFE;
            break;

        case 0x82:      // BITC 1
            AC = AC & 0xFD;
            break;

        case 0x84:      // BITC 2
            AC = AC & 0xFB;
            break;

        case 0x86:      // BITC 3
            AC = AC & 0xF7;
            break;

        case 0x88:      // BITC 4
            AC = AC & 0xEF;
            break;

        case 0x8a:      // BITC 5
            AC = AC & 0xDF;
            break;

        case 0x8c:      // BITC 6
            AC = AC & 0xBF;
            break;

        case 0x8e:      // BITC 7
            AC = AC & 0x7F;
            break;

        case 0x90:      // BITS 0
            AC = AC | 0x01;
            break;

        case 0x92:      // BITS 1
            AC = AC | 0x02;
            break;

        case 0x94:      // BITS 2
            AC = AC | 0x04;
            break;

        case 0x96:      // BITS 3
            AC = AC | 0x08;
            break;

        case 0x98:      // BITS 4
            AC = AC | 0x10;
            break;

        case 0x9a:      // BITS 5
            AC = AC | 0x20;
            break;

        case 0x9c:      // BITS 6
            AC = AC | 0x40;
            break;

        case 0x9e:      // BITS 7
            AC = AC | 0x80;
            break;

        case 0xa0:      // CLRV
            FL = FL & ~(V_BIT);
            break;

        case 0xa2:      // CLRS
            FL = FL & ~(S_BIT);
            break;

        case 0xa4:      // CLRC
            FL = FL & ~(C_BIT);
            break;

        case 0xa6:      // CRLZ
            FL = FL & ~(Z_BIT);
            break;

        case 0xb0:      // SETV
            FL = FL | V_BIT;
            break;

        case 0xb2:      // SETS
            FL = FL | S_BIT;
            break;

        case 0xb4:      // SETC
            FL = FL | C_BIT;
            break;

        case 0xb6:      // SETZ
            FL = FL | Z_BIT;
            break;

        // Note that for all jumps, there will need to be a branch delay slot
        // that will be executed after all jumps
        case 0xc0:      // JPVC #aa
            if (!(FL & V_BIT)) {
                setPC(JH, OR);
                delay_slot = true; 
            }
            break;

        case 0xc2:      // JPSC #aa
            if (!(FL & S_BIT)) {
                setPC(JH, OR);
                delay_slot = true; 
            }
            break;

        case 0xc4:      // JPCC #aa
            if (!(FL & C_BIT)) {
                setPC(JH, OR);
                delay_slot = true; 
            }
            break;

        case 0xc6:      // JPZC #aa
            if (!(FL & Z_BIT)) {
                setPC(JH, OR);
                delay_slot = true; 
            }
            break;

        case 0xd0:      // JPVS #aa
            if (FL & V_BIT) {
                setPC(JH, OR);
                delay_slot = true; 
            }
            break;

        case 0xd2:      // JPSS #aa
            if (FL & S_BIT) {
                setPC(JH, OR);
                delay_slot = true; 
            }
            break;

        case 0xd4:      // JPCS #aa
            if (FL & C_BIT) {
                setPC(JH, OR);
                delay_slot = true; 
            }
            break;

        case 0xd6:      // JPZS #aa
            if (FL & Z_BIT) {
                setPC(JH, OR);
                delay_slot = true; 
            }
            break;

        case 0xe0:      // JUMP #aa
            setPC(JH, OR);
            delay_slot = true; 
            break;

        case 0xe6:      // JUMP JL
            setPC(JH, JL);
            delay_slot = true; 
            break;

        case 0xfc:      // NOOP
            break;

        case 0xff:      // HALT
            Halt = true;
            break;

        default:
            cout << "Unsupported opcode: 0x" << hex << setw(2) << (unsigned int) IR << endl;
    }

}