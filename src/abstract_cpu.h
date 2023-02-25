/* 8-bit Emulator 
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  02/23/2023  Andy4495  Initial Creation

*/

// This is an abstract base class for an 8-bit CPU
// All CPU class definitions should publicly derive from this class

#ifndef ABSTRACT_CPU
#define ABSTRACT_CPU

class abstract_CPU {
        virtual void load_memory(const char* fname) = 0;
        virtual void cold_reset() = 0;
        virtual void warm_reset() = 0;
        virtual void run_from_address(unsigned short addr) = 0;
        virtual void fetch_and_decode() = 0;
        virtual void execute() = 0;
        virtual void print_fetched_instruction() = 0;
        virtual void print_registers() = 0;
        virtual void print_flags() = 0;
        virtual void print_memory(unsigned short start, unsigned short end) = 0;
        virtual bool halted() = 0;  // If true, the CPU is halted
        virtual unsigned short getPC() = 0; // The wrapper program may need to know the current program counter value
};

#endif