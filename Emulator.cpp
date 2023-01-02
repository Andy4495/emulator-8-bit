/* Z80 Emulator 
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
*/

#define VERSION 0.9

// 1. Read command line and parse arguments parseCommandLine()
// 2. Read memory file (hex, s-record) loadProgram()
//    - This can optionally pre-load RAM with values (i.e., can start-up the CPU with a known RAM state as opposed to a cold reset)
// 3. Initialize the processor: zero out ram, initialize registers, initialize internal state variables initializeCPU()
// 4. fetch_and_decode()
// 5. execute()
// 6. Print results status()
// 7. Repeat
// 8. softReset() clear registers, load reset vector, but don't zero RAM
// 9. hardReset() zero out everything, load reset vector
// 10. interrupt() TBD
// 11. halt() TBD; implement per user manual
// 12. quit() exit program, cleanup if required (maybe print register values before exit)
//
// Notes:
// 1. Currently using assert() for error handling for ease in debugging. May want to use a more graceful method in final version, which may be specific to the situation.
//

#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>

#include "Z80.h"

// Main() function: where the execution of program begins
int main(int argc, char** argv)
{
    // All CPU class functions use hex (hexadecimal) for cout
    // Anywhere dec is desired, temporarily switch to dec and then swtich back to hex when done writing to cout

    // prints hello world
    cout << "Z80 Emulator Version: " << VERSION << endl;

    /// Use default values for rom and ram definitions for now
    Z80 cpu;

    int choice = 0;
    unsigned short addr;
    bool disassemble_mode = false;

    if (argc == 2) { // Use pathname passed on command line
        cpu.load_memory(argv[1]);
    }
    else { // Use default filename
        cpu.load_memory("data.bin");
    }
    cout << "DONE." << endl;

    while (choice == 0) {
        cout << "Select an option:" << endl;
        cout << "1. Cold reset and run from $0000." << endl;
        cout << "2. Warm reset and run from $0000." << endl;
        cout << "3. Run from specific address." << endl;
        cout << "4. Set breakpoint." << endl;
        cout << "5. Disassemble (do not run code)." << endl;

        cin >> choice;

        switch (choice) {
            case 1: // Cold restart
                cpu.cold_reset();
                break;

            case 2: // Warm restart
                cpu.warm_reset();
                break;

            case 3: // Run from address
                cout << "Enter starting address in hex: 0x";
                cin >> hex >> addr >> dec;
                cpu.run_from_address(addr);
                break;

            case 4: // Set breakpoint
                // Add code to input address 
                // Add code to set a breakpoint
                choice = 0;
                break;

            case 5: // Disassemble (do not execute)
                disassemble_mode = true;
                cout << "Enter starting address in hex: 0x";
                cin >> hex >> addr >> dec;
                cpu.run_from_address(addr);
                break;

            default: // Choice outside of range
                cout << "Invalid selection. Try again." << endl << endl;
                choice = 0;
                break;

        }
    }

    int state = 1;
    while ((state < 12) && (!cpu.Halt)) { // Limit the number if times we fetch and decode
    ///    cout << "State: " << state << " PC: " << hex << cpu.PC << endl; /// debug
        cpu.fetch_and_decode();
        if (disassemble_mode == false) cpu.execute();
        cpu.print_fetched_instruction();
        state++;
    }

    if (cpu.Halt) cout << "CPU Halted." << endl;

    cout << hex << "A: " << (unsigned int) cpu.A 
    <<  " B: " << (unsigned int) cpu.B 
    <<  " C: " << (unsigned int) cpu.C
    <<  " D: " << (unsigned int) cpu.D
    <<  " E: " << (unsigned int) cpu.E
    <<  " H: " << (unsigned int) cpu.H
    <<  " L: " << (unsigned int) cpu.L 
    << " IX: " << (unsigned short) cpu.getIX()
    << " IY: " << (unsigned short) cpu.getIY()
    << " SP: " << (unsigned short) cpu.SP
    << "  I: " << (unsigned int)   cpu.I
    << "  R: " << (unsigned int)   cpu.R
    << endl;

    cout << "SZXH XPNC: "
    << cpu.testFlag(Z80::S_BIT)
    << cpu.testFlag(Z80::Z_BIT)
    << cpu.testFlag(Z80::X1_BIT)
    << cpu.testFlag(Z80::H_BIT)
    << " "
    << cpu.testFlag(Z80::X2_BIT)
    << cpu.testFlag(Z80::PV_BIT)
    << cpu.testFlag(Z80::N_BIT)
    << cpu.testFlag(Z80::C_BIT)
    << endl;

    return 0;
}
