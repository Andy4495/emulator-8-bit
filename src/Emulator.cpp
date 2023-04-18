/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
   0.3.0 04/16/2023 Andy4495  Tag v0.3.0
*/

const char* VERSION = "v0.3.0";

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
using std::dec;
using std::setw;

int display_main_menu();
int display_disassemble_menu();
int display_execute_menu();
int display_debug_menu();

/// Use default values for rom and ram definitions for now
Z80 cpu;

// Breakpoints -- save current instruction and replace with HALT
// Only single breakpoint supported for now
// Eventually need to use class function. For now, hardcode Z80 HALT opcode.
bool breakpoint_set = false;
uint16_t breakpoint_address;
uint8_t breakpoint_prev_val;
int answer;

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
    uint16_t value;
    uint16_t prevPC;
    enum DISASM_MODE {OFF, FULL, ASSEMBLY} disassemble_mode = OFF;
    enum EMU_STATE {MAIN_MENU, DISASSEMBLE_MENU, EXECUTE_MENU, DEBUG_MENU,
                    DISASSEMBLE, RUN_EXECUTE, RUN_DEBUG,
                    SINGLE_STEP_EXECUTE, SINGLE_STEP_DEBUG,
                    EXIT}
                    state = MAIN_MENU;

    if (argc == 2) {  // Use pathname passed on command line
        cout << "Loading memory from file: " << argv[1] << " . . . ";
        cpu.load_memory(argv[1]);
    } else {  // Use default filename
        cout << "Loading memory from file: " << "data.bin" << " . . . ";
        cpu.load_memory("data.bin");
    }
    cout << "DONE." << endl;

    while (state != EXIT) {
        switch(state) {
            case MAIN_MENU:
                switch (display_main_menu()) {
                    case 1:
                        state = DISASSEMBLE_MENU;
                        break;
                    case 2:
                        state = EXECUTE_MENU;
                        break;
                    case 3: 
                        state = DEBUG_MENU;
                        break;
                    case 9: 
                        state = EXIT;
                        break;

                    default:  // Choice outside of range
                        cout << "Invalid selection. Try again." << endl << endl;
                        break;
                }
                break;

            case DISASSEMBLE_MENU:
                switch (display_disassemble_menu()) {
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
                    case 8: 
                        state = MAIN_MENU;
                        break;
                    case 9: 
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
                        state = RUN_EXECUTE;
                        break;
                    case 2:  // Warm Reset; reset PC to zero
                        cpu.warm_reset();
                        state = RUN_EXECUTE;
                        break;
                    case 3:  // Run from address
                        cout << "Enter address to run in hex: 0x";
                        cin >> hex >> start_addr;
                        cpu.run_from_address(start_addr);
                        state = RUN_EXECUTE;
                        break;
                    case 4:  // Single step program from current PC
                        cpu.run_from_address(cpu.getPC());
                        state = SINGLE_STEP_EXECUTE;
                        cout << endl << "Press <SPC> to step; <x> to end." << endl;
                        break;
                    case 5:  // Continue
                        cout << "Continuing execution from 0x" << hex << cpu.getPC() << endl;
                        cpu.run_from_address(cpu.getPC());
                        state = RUN_EXECUTE;
                        break;                        
                    case 6:  // Print memory
                        cout << "Print memory..." << endl;
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Enter ending address in hex: 0x";
                        cin >> hex >> end_addr;                        
                        cpu.print_memory(start_addr, end_addr);
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

            case DEBUG_MENU:
                switch (display_debug_menu()) {
                    case 1:  // Cold reset
                        cpu.cold_reset();
                        state = RUN_DEBUG;
                        break;
                    case 2:  // Breakpoints
                        cout << endl;
                        if (breakpoint_set) {
                            cout << "Breakpoint set at 0x" << hex << setw(4) << breakpoint_address
                                 << " replacing value 0x" << setw(2) << (int) breakpoint_prev_val << endl;
                            cout << "Enter 1 to replace, 2 to delete, anything else to leave as-is: ";
                            cin >> answer;
                            if (answer == 1) {
                                cpu.set_memory(breakpoint_address, breakpoint_prev_val);
                                cout << "  Enter breakpoint address in hex: 0x";
                                cin >> breakpoint_address;
                                breakpoint_prev_val = cpu.get_memory(breakpoint_address);
                                cpu.set_memory(breakpoint_address, cpu.halt_opcode());
                                cout << "New breakpoint set at 0x" << setw(4) << breakpoint_address << endl;
                            } else if (answer == 2) {
                                cpu.set_memory(breakpoint_address, breakpoint_prev_val);
                                breakpoint_set = false;
                                cout << "Breakpoint cleared." << endl;
                            }
                        } else {
                            cout << "No breakpoint currently set." << endl;
                            cout << "Add breakpoint? (Y/N) ";
                            cin  >> c; 
                            if (c == 'y' or c == 'Y') {
                                cout << "  Enter breakpoint address in hex: 0x";
                                cin >> breakpoint_address;
                                breakpoint_prev_val = cpu.get_memory(breakpoint_address);
                                cpu.set_memory(breakpoint_address, cpu.halt_opcode());
                                breakpoint_set = true;
                                cout << "New breakpoint set at 0x" << setw(4) << breakpoint_address << endl;
                            }
                        }
                        cout << endl;
                        state = DEBUG_MENU;
                        break;
                    case 3:  // Run from address
                        cout << "Enter address to run in hex: 0x";
                        cin >> hex >> start_addr;
                        cpu.run_from_address(start_addr);
                        state = RUN_DEBUG;
                        break;
                    case 4:  // Single step program from current PC
                        cpu.run_from_address(cpu.getPC());
                        state = SINGLE_STEP_DEBUG;
                        cout << endl << "Press <SPC> to step; <x> to end." << endl;
                        break;
                    case 5:  // Continue
                        cout << "Continuing execution from 0x" << hex << cpu.getPC() << endl;
                        cpu.run_from_address(cpu.getPC());
                        state = RUN_DEBUG;
                        break;                        
                    case 6:  // Print memory
                        cout << "Print memory..." << endl;
                        cout << "Enter starting address in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Enter ending address in hex: 0x";
                        cin >> hex >> end_addr;                        
                        cpu.print_memory(start_addr, end_addr);
                        break;
                    case 7:  //  Edit memory
                        cout << "Enter address in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Current value at 0x" << setw(4) << hex << start_addr << ": 0x" 
                             << setw(2) << hex << (int) cpu.get_memory(start_addr) << endl;
                        cout << "Enter new value in hex: 0x";
                        cin >> hex >> value;
                        cpu.set_memory(start_addr, value);
                        cout << endl;
                        state = DEBUG_MENU;
                        break;
                    case 8:  //  Print registers
                        cout << endl << "Register values:" << endl;
                        cpu.print_registers();
                        cout << endl;
                        state = DEBUG_MENU;
                        break;
                    case 9:  //  Edit register
                        cpu.set_register();
                        state = DEBUG_MENU;
                        break;
                    case 10: //  Print Input Port values
                        cout << endl << "Input Ports:" << endl;
                        cout << "      0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f" << endl;
                        cout << "     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --" << endl;
                        for (uint32_t i = 0; i < 256; i += 16) {
                            cout << hex << setw(1) << i/16 << "x" << ":  ";
                            for (int j = 0; j < 16; j ++) {
                                cout << hex << setw(2) << (uint16_t) cpu.get_input_port(i + j) << " ";
                            }
                            cout << endl;
                        }
                        cout << endl;
                        state = DEBUG_MENU;
                        break;
                    case 11: //  Edit Input Port value
                        cout << "Enter port number in hex: 0x";
                        cin >> hex >> start_addr;
                        cout << "Current value at inport port 0x" << setw(2) << hex << start_addr << ": 0x" 
                             << setw(2) << hex << (int) cpu.get_input_port(start_addr) << endl;
                        cout << "Enter new value in hex: 0x";
                        cin >> hex >> value;
                        cpu.set_input_port(start_addr, value);
                        cout << endl;
                        state = DEBUG_MENU;
                        break;
                    case 12: //  Print Output Port values
                        cout << endl << "Output Ports:" << endl; 
                        cout << "      0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f" << endl;
                        cout << "     -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --" << endl;
                        for (uint32_t i = 0; i < 256; i += 16) {
                            cout << hex << setw(1) << i/16 << "x" << ":  ";
                            for (int j = 0; j < 16; j ++) {
                                cout << hex << setw(2) << (uint16_t) cpu.get_output_port(i + j) << " ";
                            }
                            cout << endl;
                        }
                        cout << endl;
                        state = DEBUG_MENU;
                        break;
                    case 98:  // Return to main menu
                        state = MAIN_MENU;
                        break;
                    case 99:  // Exit
                        state = EXIT;
                        break;

                    default:  // Choice outside of range
                        cout << "Invalid selection. Try again." << endl << endl;
                        break;                    
                }
                break;

            case RUN_EXECUTE:
            case RUN_DEBUG:
                cpu.fetch_and_decode();
                cpu.execute();
                cpu.print_fetched_instruction();
                if (cpu.halted()) {
                    cout << "CPU Halted." << endl;
                    cpu.print_registers();
                    cpu.print_flags();
                    if (state == RUN_EXECUTE) state = EXECUTE_MENU;
                    else state = DEBUG_MENU;
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
                        state = DISASSEMBLE_MENU;
                    }
                }
                prevPC = cpu.getPC();
                break;

            case SINGLE_STEP_EXECUTE: 
            case SINGLE_STEP_DEBUG:
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
                if (c != ' ') {
                    if (state == SINGLE_STEP_EXECUTE) state = EXECUTE_MENU;
                    else state = DEBUG_MENU;
                }
                break;

            default:
                cout << "Invalid program state. Exiting." << endl;
                state = EXIT;
                break;
        }
    }

    return 0;
}

int display_main_menu() {
    int choice;
    cout << "Select an option:" << endl;
    cout << "1. Disassemble mode." << endl;
    cout << "2. Execute mode." << endl;
    cout << "3. Debug mode." << endl;
    cout << "9. Exit." << endl;
    cin >> dec >> choice;
    return choice;
}

int display_disassemble_menu() {
    int choice;

    cout << "1. Disassemble (do not run code)." << endl;
    cout << "2. Disassemble and exit." << endl;
    cout << "3. Disassemble into compilable assembly and exit." << endl;
    cout << "8. Return to main menu." << endl;
    cout << "9. Exit." << endl;
    cin >> dec >> choice;
    return choice;
}

int display_execute_menu() {
    int choice;

    cout << "1. Cold reset: initialize registers and run from $0000. "
            << "Program will continue running until HALT." << endl;
    cout << "2. Warm reset: run from $0000 (no other registers updated). " << endl;
    cout << "3. Run from address. " << endl;
    cout << "4. Single step from current PC (press <SPC> to continue stepping)." << endl;
    cout << "5. Continue (run from current PC)." << endl;
    cout << "6. Print memory." << endl;
    cout << "7. Dump memory to file." << endl;
    cout << "8. Return to main menu." << endl;
    cout << "9. Exit." << endl;
    cin >> dec >> choice;
    return choice;
}

int display_debug_menu() {
    int choice;

    cout << "1. Cold reset - run from $0000. " << endl;
    cout << "2. List/Set/Clear breakpoint. " << endl;
    cout << "3. Run from address. " << endl;
    cout << "4. Single step from current PC (press <SPC> to continue stepping)." << endl;
    cout << "5. Continue (run from current PC)." << endl;
    cout << "6. Print memory." << endl;
    cout << "7. Edit memory." << endl;
    cout << "8. Print registers." << endl;
    cout << "9. Edit register." << endl;
    cout << "10. Print input port values." << endl;
    cout << "11. Edit input port value." << endl;
    cout << "12. Print output port values." << endl;
    cout << "98. Return to main menu." << endl;
    cout << "99. Exit." << endl;
    cin >> dec >> choice;
    return choice;
}
