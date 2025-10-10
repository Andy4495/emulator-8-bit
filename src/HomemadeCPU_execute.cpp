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

    switch (IR) {
        case 0x00: 
            AC = OR;
            break;

        case 0x08: 
            AC = memory[getMA()];
            break;

        case 0x10: 
//            Commenting this out to clear a compiler warning.
//            TBD whether the hardware actually implements the move
//            Either way is the net effect of this is a NOOP
//            AC = AC;
            break;

        case 0x11:
            AC = FL;
            break;

        case 0x12: 
            AC = getSH(); 
            break;

        case 0x13: 
            AC = getSL();
            break;

        case 0x14: 
            AC = MH; 
            break;

        case 0x15: 
            AC = JH;
            break;

        case 0x16: 
            AC = JL;
            break;

        case 0x18: 
            memory[getMA()] = AC;
            break;

        case 0x20: 
//            Commenting this out to clear a compiler warning.
//            TBD whether the hardware actually implements the move
//            Either way is the net effect of this is a NOOP            
//            AC = AC; 
            break;

        case 0x21: 
            FL = AC;
            break;

        case 0x22: 
            setSH(AC); 
            break;

        case 0x23:
            setSL(AC); 
            break;
        
        case 0x24: 
            MH = AC;
            break;

        case 0x25: 
            JH = AC; 
            break;

        case 0x26: 
            JL = AC;
            break;

        case 0x28:
            memory[SP--] = AC;
            break;

        case 0x29:
            memory[SP--] = FL;
            break;

        case 0x2a: 
            memory[SP--] = getSH();
            break;

        case 0x2b: 
            memory[SP--] = getSL();
            break;

        case 0x2c: 
            memory[SP--] = MH;
            break;

        case 0x2d: 
            memory[SP--] = JH;
            break;

        case 0x2e: 
            memory[SP--] = JL;
            break;

        case 0x30: 
            memory[SP--] = OR;
            break;

        case 0x38: 
            AC = memory[SP++];
            break;

        case 0x39: 
            FL = memory[SP++]; 
            break;

        case 0x3a:
            setSH(memory[SP++]);

        case 0x3b:
            setSL(memory[SP++]);
            break;

        case 0x3c:
            MH = memory[SP++];
            break;

        case 0x3d: 
            JH = memory[SP++];
            break;

        case 0x3e:
            JL = memory[SP++];
            break;

        case 0x40:                  // COMP - result not saved
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

        case 0x42:
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

        case 0x44:
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

        case 0x50:
            result = AC & OR;
            update_Z(result);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            AC = result;
            break;  
            
        case 0x52:
            result = AC | OR;
            update_Z(result);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            AC = result;
            break;

        case 0x54: 
            result = AC ^ OR;
            update_Z(result);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            AC = result;
            break;

        case 0x56: 
            result = ~(AC & OR);
            update_Z(result);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            AC = result;
            break;

        case 0x58: 
            result = ~(AC | OR);
            update_Z(result);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            AC = result;
            break;

        case 0x60:
            result = ~AC;
            update_Z(result);
            clearFlag(C_BIT);
            clearFlag(S_BIT);
            clearFlag(V_BIT);
            AC = result;
            break;
        
        default:
            cout << "Unsupported opcode: 0x" << hex << setw(2) << (unsigned int) IR << endl;
    }

}