/* 8-bit Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  02/23/2023  Andy4495  Initial Creation
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
   0.3.0 04/16/2023 Andy4495  Tag v0.3.0

*/

// This is an abstract base class for an 8-bit CPU
// All CPU class definitions should publicly derive from this class

#ifndef SRC_ABSTRACT_CPU_H_
#define SRC_ABSTRACT_CPU_H_
#include <cstdint>

class abstract_CPU {
    virtual void load_memory(const char* fname) = 0;
    virtual void dump_memory_to_file(const char* fname) = 0;
    virtual void cold_reset() = 0;
    virtual void warm_reset() = 0;
    virtual void run_from_address(uint16_t addr) = 0;
    virtual void fetch_and_decode() = 0;
    virtual void execute() = 0;
    virtual void print_fetched_instruction() = 0;
    virtual void print_registers() = 0;
    virtual void print_flags() = 0;
    virtual void print_memory(uint16_t start, uint16_t end) = 0;
    virtual bool halted() = 0;  // If true, the CPU is halted
    // Wrapper program may need to know the current program counter value
    virtual uint16_t getPC() = 0;
    // Print disassembly so that it can be directly input to an assembler
    virtual void print_assembly() = 0;
};

#endif  //  SRC_ABSTRACT_CPU_H_
