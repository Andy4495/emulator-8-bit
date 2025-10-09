/* Homemade CPU Emulator 
   Copyright 2025 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   See https://github.com/Andy4495/Homemade-CPU for the CPU this emulates

   0.1  08-Oct-2025  Andy4495  Initial Creation


*/

#ifndef _HOMEMADECPU_H_
#define _HOMEMADECPU_H_

#include "./abstract_cpu.h"

/* CPU Core Definitions */
#define CPU_MAX_MEMORY 65536
#define CPU_MAX_INSTR_SIZE 2

#define CPU_MAX_FETCHED_LENGTH 9
#define CPU_MAX_MNEMONIC_LENGTH 49
#define CPU_MAX_TEXT_LENGTH 72

class HomemadeCPU : public abstract_CPU {
  public:
   HomemadeCPU(uint16_t ramstart = 0x8000, uint16_t ramend = 0xffff);
   void load_memory(const char* fname);
   void dump_memory_to_file(const char* fname);
   void cold_reset();
   void warm_reset();
   void run_from_address(uint16_t addr);
   void fetch_and_decode();
   void execute();
   void print_fetched_instruction();
   void print_registers();
   void print_flags();
   void print_memory(uint16_t start, uint16_t end);
   uint8_t get_memory(uint16_t loc);
   void set_memory(uint16_t loc, uint8_t val);
   uint8_t get_input_port(uint8_t loc);
   void set_input_port(uint8_t loc, uint8_t val);
   uint8_t get_output_port(uint8_t loc);   
   void set_register();
   uint8_t halt_opcode();
   bool halted();  // If true, the CPU is halted
   uint16_t getPC();
   void print_assembly();

  private:
   uint8_t memory[CPU_MAX_MEMORY] = {0};
   unsigned int instr_length;
   char mnemonic[CPU_MAX_MNEMONIC_LENGTH + 1];
   char instr_string[CPU_MAX_TEXT_LENGTH + 1];
   uint16_t _ramstart;
   uint16_t _ramend;

   enum FLAG_BITS { Z_BIT = 0x08, C_BIT = 0x04, S_BIT = 0x02, V_BIT = 0x01};

   // State variables
   bool Halt;

   // Main register set
   // Accumulator
   uint8_t AC;
   // Flags
   uint8_t FL;

   // Special purpose registers
   // Stack pointer
   uint8_t SH;
   uint8_t SL;
   // Memory pointer MSB
   uint8_t MH;
   // Jump Address
   uint8_t JH;
   uint8_t JL;

   // Other Registers and States, not directly accessible to the programmer:
   // Program counter
   uint16_t PC;
   // Temporary storage for Program Counter when instruction was fetched
   // This is needed because Jumps will alter PC value before it is printed
   uint16_t PC_of_Fetch;
   // Instruction Register
   uint8_t IR;
   // Operand Register
   uint8_t OR;

   uint8_t  testFlag(FLAG_BITS f);
   uint8_t get_next_byte();
   void update_C(uint8_t val1, uint8_t val2);
   void update_V(uint8_t val1, uint8_t val2);
   void update_Z(uint8_t val);
   void update_S(uint8_t val);
   uint16_t getSP();
   uint16_t getJA();  // Jump Address: 16-bit equivalent of JH, JL
   void setSP(uint16_t v);
   void setPC(uint8_t msb, uint8_t lsb);
   void setPCofFetch(uint8_t msb, uint8_t lsb);
   void init_registers();
   void setFlag(FLAG_BITS f);
   void clearFlag(FLAG_BITS f);
   void decode_instruction();
   void execute_instruction();
};
#endif  // _HOMEMADECPU_H_
