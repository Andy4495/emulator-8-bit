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
#include <string>
using std::ifstream;
using std::iostream;
using std::ios;
#include <iomanip>
using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::setw;

int display_main_menu();
int display_disassembly_menu();
int display_execute_menu();

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
    char c;
    bool exit_on_disassemble = false;
    uint16_t start_addr, end_addr;
    uint16_t prevPC;
    enum DISASM_MODE {OFF, FULL, ASSEMBLY} disassemble_mode = OFF;
    enum EMU_STATE {MAIN_MENU, DISASSEMBLY_MENU, EXECUTE_MENU, 
                    DISASSEMBLE, RUN, SINGLE_STEP, EXIT}
                    state = MAIN_MENU;

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
            case MAIN_MENU:
                switch (display_main_menu()) {
                    case 1:
                        state = DISASSEMBLY_MENU;
                        break;
                    case 2:
                        state = EXECUTE_MENU;
                        break;
                    case 3: 
                        state = EXIT;
                        break;

                    default:  // Choice outside of range
                        cout << "Invalid selection. Try again." << endl << endl;
                        break;
                }
                break;

            case DISASSEMBLY_MENU:
                switch (display_disassembly_menu()) {
                    case 2: 
                        exit_on_disassemble = true;
                        // Intentionally fall through to case 1 without break;
                    case 1: 
                        disassemble_mode = FULL;
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Enter ending address in hex: 0x";
                        cin >> hex >> end_addr;
                        prevPC = start_addr;
                        cpu.run_from_address(start_addr);
                        state = DISASSEMBLE;
                        break;                    
                    case 3:  // Disassemble into compilable assembly
                             // Same as above, but print mnemomic only;
                             // don't print address or fetched data
                        disassemble_mode = ASSEMBLY;
                        exit_on_disassemble = true;
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
                    case 4: 
                        state = MAIN_MENU;
                        break;
                    case 5: 
                        state = EXIT;
                        break;

                    default:  // Choice outside of range
                        cout << "Invalid selection. Try again." << endl << endl;
                        break;
                }
                break;
            
            case EXECUTE_MENU:
                switch (display_execute_menu()) {
                    case 1:  // Cold reset
                        cpu.cold_reset();
                        state = RUN;
                        break;
                    case 2:  // Clear all registers
                        cpu.warm_reset();
                        break;
                    case 3:  // Run from address
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cpu.run_from_address(start_addr);
                        state = RUN;
                        break;
                    case 4:  // Single step program
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cpu.run_from_address(start_addr);
                        state = SINGLE_STEP;
                        cout << endl << "Press <SPC> to step; <x> to end." << endl;
                        break;
                    case 5:  // Print memory
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Enter ending address in hex: 0x";
                        cin >> hex >> end_addr;                        
                        cpu.print_memory(start_addr, end_addr);
                        break;
                    case 6:  // Continue
                        cpu.run_from_address(cpu.getPC());
                        state = RUN;
                        break;                        
                    case 7:  // Dump memory to file
                        cout << "Enter filename: "; 
                        cin >> setw(MAXWIDTH) >> fname;
                        cout << endl; 
                        cpu.dump_memory_to_file(fname);
                        break;
                    case 8:  // Return to main menu
                        state = MAIN_MENU;
                        break;
                    case 9:  // Exit
                        state = EXIT;
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
                    cpu.print_registers();
                    cpu.print_flags();
                    state = EXECUTE_MENU;
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
                    if (exit_on_disassemble == true) {
                        state = EXIT;
                    } else {
                        state = DISASSEMBLY_MENU;
                    }
                }
                prevPC = cpu.getPC();
                break;

            case SINGLE_STEP: 
                cpu.fetch_and_decode();
                cpu.execute();
                cpu.print_fetched_instruction();
                cpu.print_registers();
                cpu.print_flags();
                c = 0;
                system("stty raw");  // Set terminal to raw mode 
                while ((c != ' ') && (c != 'x')) {
                    c = getchar();  // Wait for single character 
                }
                // Reset terminal to normal "cooked" mode 
                system("stty cooked"); 
                if (c != ' ') state = EXECUTE_MENU;
                break;

            default:
                cout << "Invalid program state. Exiting." << endl;
                state = EXIT;
        }
    }

    return 0;
}

int display_main_menu() {
    int choice;
    cout << "Select an option:" << endl;
    cout << "1. Disassembly mode." << endl;
    cout << "2. Execute mode." << endl;
    cout << "3. Exit." << endl;
    cin >> choice;
    return choice;
}

int display_disassembly_menu() {
    int choice;

    cout << "1. Disassemble (do not run code)." << endl;
    cout << "2. Disassemble and exit." << endl;
    cout << "3. Disassemble into compilable assembly and exit." << endl;
    cout << "4. Return to main menu." << endl;
    cout << "5. Exit." << endl;
    cin >> choice;
    return choice;
}

int display_execute_menu() {
    int choice;

    cout << "1. Cold reset - run from $0000. "
            << "Program will continue running until HALT." << endl;
    cout << "2. Clear all registers. " << endl;
    cout << "3. Run from address. " << endl;
    cout << "4. Single step program (press <SPC> to continue stepping)." << endl;
    cout << "5. Print memory." << endl;
    cout << "6. Continue (run from current PC)." << endl;
    cout << "7. Dump memory to file." << endl;
    cout << "8. Return to main menu." << endl;
    cout << "9. Exit." << endl;
    cin >> choice;
    return choice;
}


