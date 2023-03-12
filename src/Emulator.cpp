/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
*/

const char* VERSION = "v0.2.0";

// 1. Read command line and parse arguments parseCommandLine()
// 2. Read memory file (hex, s-record) loadProgram()
//    - This can optionally pre-load RAM with values (i.e., can start-up
//      the CPU with a known RAM state as opposed to a cold reset)
// 3. Initialize the processor: zero out ram, initialize registers,
//    initialize internal state variables initializeCPU()
// 4. fetch_and_decode()
// 5. execute()
// 6. Print results status()
// 7. Repeat
// 8. softReset() clear registers, load reset vector, but don't zero RAM
// 9. hardReset() zero out everything, load reset vector
// 10. interrupt() TBD
// 11. halt() TBD; implement per user manual
// 12. quit() exit program, cleanup if required
//     (maybe print register values before exit)
//
// Notes:
// 1. Currently using assert() for error handling for ease in debugging.
//    May want to use a more graceful method in final version, which may
//    be specific to the situation.
//

#include "Z80.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdint>
using std::ifstream;
using std::iostream;
using std::ios;
#include <iomanip>
using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::setw;

int display_start_menu();
int display_breakpoint_menu();

/// Use default values for rom and ram definitions for now
Z80 cpu;

// Main() function: where the execution of program begins
int main(int argc, char** argv) {
    // All CPU class functions use hex (hexadecimal) for cout
    // Anywhere dec is desired, temporarily switch to dec and
    // then swtich back to hex when done writing to cout

    cout.fill('0');

    // prints hello world
    cout << "Z80 Emulator Version: " << VERSION << endl;

    const int MAXWIDTH=128;
    char fname[MAXWIDTH]; 
    uint16_t start_addr, end_addr;
    uint16_t prevPC;
    enum DISASM_MODE {OFF, FULL, ASSEMBLY} disassemble_mode = OFF;
    enum EMU_STATE {START, RUN, DISASSEMBLE, PRINT_STATE, BREAK, EXIT}
                    state = START;

    if (argc == 2) {  // Use pathname passed on command line
        cout << "Loading memory from file: " << argv[1] << " . . . ";
        cpu.load_memory(argv[1]);
    } else {  // Use default filename
        cpu.load_memory("data.bin");
        cout << "Loading memory from file: " << "data.bin" << " . . . ";
    }
    cout << "DONE." << endl;

    while (state != EXIT) {
        switch(state) {
            case START:
                switch (display_start_menu()) {
                    case 1:  // Cold restart
                        cpu.cold_reset();
                        state = RUN;
                        break;

                    case 2:  // Warm restart
                        cpu.warm_reset();
                        state = START;
                        break;

                    case 3:  // Run from address
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cpu.run_from_address(start_addr);
                        state = RUN;
                        break;

                    case 4:  // Set breakpoint
                        // Add code to input address
                        // Add code to set a breakpoint
                        cout << "Not supported yet." << endl;
                        break;

                    case 5:  // Disassemble (do not execute)
                        disassemble_mode = FULL;
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Enter ending address in hex: 0x";
                        cin >> hex >> end_addr;
                        cpu.run_from_address(start_addr);
                        state = DISASSEMBLE;
                        break;

                    case 6:  // Disassemble into compilable assembly
                            // (same as above, but don't print address or fetched data)
                        disassemble_mode = ASSEMBLY;
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Enter ending address in hex: 0x";
                        cin >> hex >> end_addr;
                        prevPC = start_addr;
                        cout << endl << ";----- Program starts here ------" << endl;
                        cout << "      org    $" << hex << start_addr << endl;
                        cpu.run_from_address(start_addr);
                        state = DISASSEMBLE;
                        break;

                    default:  // Choice outside of range
                        cout << "Invalid selection. Try again." << endl << endl;
                        break;
                }                
                break;

            case RUN:
                cpu.fetch_and_decode();
                cpu.execute();
                cpu.print_fetched_instruction();
                if (cpu.halted()) {
                    cout << "CPU Halted." << endl;
                    state = PRINT_STATE;
                }
                break;

            case DISASSEMBLE:
                cpu.fetch_and_decode();
                if (disassemble_mode == ASSEMBLY)
                    cpu.print_assembly();
                else
                    cpu.print_fetched_instruction();
                // Check for some corner cases so that we don't wrap around memory
                if ((cpu.getPC() > end_addr) || (cpu.getPC() == 0) || (cpu.getPC() < prevPC)) {
                    state = EXIT;
                }
                prevPC = cpu.getPC();
                break;

            case PRINT_STATE:
                cpu.print_registers();
                cpu.print_flags();
                state = BREAK;
                break;

            case BREAK:
                switch (display_breakpoint_menu()) {
                    case 1:  // Continue
                        cpu.run_from_address(cpu.getPC());
                        state = RUN;
                        break;

                    case 2:  // Print memory
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Enter ending address in hex: 0x";
                        cin >> hex >> end_addr;                        
                        cpu.print_memory(start_addr, end_addr);
                        break;

                    case 3:  // Dump memory to file
                        cout << "Enter filename: "; 
                        cin >> setw(MAXWIDTH) >> fname;
                        cout << endl; 
                        cpu.dump_memory_to_file(fname);
                        break;

                    case 4:  // Exit
                        state = EXIT;
                        break;

                    default: 
                        cout << "Invalid program state. Exiting." << endl;
                        state = EXIT;
                        break;
                }
                break;

            default:
                cout << "Invalid program state. Exiting." << endl;
                state = EXIT;
        }
    }

    return 0;
}

int display_start_menu() {
    int choice;
    cout << "Select an option:" << endl;
    cout << "1. Cold reset and run from $0000. "
            << "Program will continue running until HALT." << endl;
    cout << "2. Warm reset (clear all registers). " << endl;
    cout << "3. Run from specific address. "
            << "Program will continue running until HALT." << endl;
    cout << "4. Set breakpoint." << endl;
    cout << "5. Disassemble (do not run code)." << endl;
    cout << "6. Disassemble into compilable assembly." << endl;
    cin >> choice;
    return choice;
}

int display_breakpoint_menu() {
    int choice;
    cout << "Select an option:" << endl;
    cout << "1. Continue." << endl;
    cout << "2. Print memory." << endl;
    cout << "3. Dump memory to file" << endl; 
    cout << "4. Exit." << endl; 
    cin >> choice;
    return choice;
}

