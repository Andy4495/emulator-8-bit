/* Z80 Emulator 
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
*/

const char* VERSION = "v0.1.1";

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
    unsigned short start_addr, end_addr;
    unsigned short prevPC; 
    enum DISASM_MODE {OFF, FULL, ASSEMBLY};
    DISASM_MODE disassemble_mode = OFF;

    if (argc == 2) { // Use pathname passed on command line
        cout << "Loading memory from file: " << argv[1] << " . . . ";
        cpu.load_memory(argv[1]);
    }
    else { // Use default filename
        cpu.load_memory("data.bin");
        cout << "Loading memory from file: " << "data.bin" << " . . . ";
    }
    cout << "DONE." << endl;

    while (choice == 0) {
        cout << "Select an option:" << endl;
        cout << "1. Cold reset and run from $0000. Program will continue running until HALT." << endl;
        cout << "2. Warm reset and run from $0000. Program will continue running until HALT." << endl;
        cout << "3. Run from specific address. Program will continue running until HALT." << endl;
        cout << "4. Set breakpoint." << endl;
        cout << "5. Disassemble (do not run code)." << endl;
        cout << "6. Disassemble into compilable assembly." << endl;

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
                cin >> hex >> start_addr >> dec;
                cpu.run_from_address(start_addr);
                break;

            case 4: // Set breakpoint
                // Add code to input address 
                // Add code to set a breakpoint
                cout << "Not supported yet." << endl;
                choice = 0;
                break;

            case 5: // Disassemble (do not execute)
                disassemble_mode = FULL;
                cout << "Enter starting address in hex: 0x";
                cin >> hex >> start_addr >> dec;
                cout << "Enter ending address in hex: 0x";
                cin >> hex >> end_addr >> dec;
                cpu.run_from_address(start_addr);
                break;

            case 6: // Disassemble into compilable assembly (same as above, but don't print address or fetched data)
                disassemble_mode = ASSEMBLY;
                cout << "Enter starting address in hex: 0x";
                cin >> hex >> start_addr >> dec;
                cout << "Enter ending address in hex: 0x";
                cin >> hex >> end_addr >> dec;
                prevPC = start_addr;
                cout << endl << ";----- Program starts here ------" << endl;
                cout << "      org    $" << hex << start_addr << endl;
                cpu.run_from_address(start_addr);
                break;

            default: // Choice outside of range
                cout << "Invalid selection. Try again." << endl << endl;
                choice = 0;
                break;

        }
    }

    while (!cpu.halted()) {
        cpu.fetch_and_decode();
        if (disassemble_mode == OFF) cpu.execute();
        if (disassemble_mode == ASSEMBLY) cpu.print_assembly(); else cpu.print_fetched_instruction();
        // Check for some corner cases so that we don't wrap around memory
        if ((disassemble_mode != OFF) && ((cpu.getPC() > end_addr) || (cpu.getPC() == 0) || (cpu.getPC() < prevPC))) break;
        prevPC = cpu.getPC();
    }

    if (cpu.halted()) cout << "CPU Halted." << endl;

    if (disassemble_mode == OFF) {
        cpu.print_registers();
        cpu.print_flags();
    }

    return 0;
}
