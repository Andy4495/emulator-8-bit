/* Z80 Emulator 
   Copyright 2023 Andreas Taylor
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   v0.1.0    02/11/23  Andy4495 Read for first "release"
   0.2.0 03/09/2023 Andy4495  Tag v0.2.0
*/

#ifndef SRC_Z80_H_
#define SRC_Z80_H_

#include "./abstract_cpu.h"

/* Z80 Core Definitions */
#define MAX_MEMORY 65536
#define MAX_IO 256
#define MAX_INSTR_SIZE 4

#define MAX_FETCHED_LENGTH 9
#define MAX_MNEMONIC_LENGTH 49
#define MAX_TEXT_LENGTH 72

class Z80 : public abstract_CPU {
 public:
    Z80(uint16_t ramstart = 0x8000, uint16_t ramend = 0xffff,
        uint16_t romstart = 0x0000, uint16_t romend = 0x7fff);
    void load_memory(const char* fname);
    void dump_memory_to_file(const char* fname);
    void cold_reset();
    void warm_reset();
    void run_from_address(uint16_t addr);
    void fetch_and_decode();
    void execute();
    void print_fetched_instruction();
    void print_assembly();
    void print_registers();
    void print_flags();
    void print_memory(uint16_t start, uint16_t end);
    bool halted();  // If true, the CPU is halted
    uint16_t getPC();

 private:
    uint8_t memory[MAX_MEMORY] = {0};
    uint8_t in[MAX_IO] = {0};
    uint8_t out[MAX_IO] = {0};
    unsigned int instr_length;
    char mnemonic[MAX_MNEMONIC_LENGTH + 1];
    char instr_string[MAX_TEXT_LENGTH + 1];
    uint16_t _ramstart;
    uint16_t _ramend;
    uint16_t _romstart;
    uint16_t _romend;

    enum INST_TYPE {ADD, ADC, SUB, SBC};
    enum FLAG_BITS { S_BIT = 0x80,  Z_BIT = 0x40, X1_BIT = 0x20,
                        H_BIT = 0x10, X2_BIT = 0x08, PV_BIT = 0x04,
                        N_BIT = 0x02,  C_BIT = 0x01};
    enum INDEX_REG {IX_REGISTER, IY_REGISTER};

    // State variables
    bool Halt;

    // Main register set
    // Accumulator
    uint8_t A;
    // Flags
    uint8_t F;
    // General purpose registers
    uint8_t B, C, D, E, H, L;

    // Alernate register set
    // Accumulator
    uint8_t Aprime;
    // Flags
    uint8_t Fprime;
    // General purpose registers
    uint8_t Bprime, Cprime, Dprime, Eprime, Hprime, Lprime;

    // Special purpose registers
    // Interrupt vector
    uint8_t I;
    // Memory refresh register
    uint8_t R;
    // Index registers, defined as 8-bit to simplifiy implementation
    // Use get/set methods to manipulate as 16-bit
    uint8_t IXH, IXL, IYH, IYL;
    // Stack pointer
    uint16_t SP;

    // Other Registers and States, not directly accessible to the programmer:
    // Program counter
    uint16_t PC;
    // Temporary storage for Program Counter when instruction was fetched
    // This is needed because Jumps will alter PC value before it is printed
    uint16_t PC_of_Fetch;
    // Instruction Register
    // This is a one-byte register in the physical Z80,
    // but define it here as 4-bytes (max instruction size)
    // for ease of implementation
    uint8_t IR[MAX_INSTR_SIZE];
    // Interrupt enable flip-flops
    uint8_t IFF1;
    uint8_t IFF2;
    // Interrupt Mode
    uint8_t INT_MODE;
    char fetched[MAX_FETCHED_LENGTH + 1];
    bool disp_pos; // Indicates if index displacement is positive or negative

    uint8_t  testFlag(FLAG_BITS f);
    uint8_t get_next_byte();
    void update_C(INST_TYPE t,  uint16_t val1, uint16_t val2);
    void update_P(uint8_t v);
    void update_V(INST_TYPE t, uint8_t val1, uint8_t val2);
    void update_V16(INST_TYPE t, uint16_t val1, uint16_t val2);
    void update_H(INST_TYPE t, uint8_t val1, uint8_t val2);
    void update_Z(uint8_t val);
    void update_S(uint8_t val);
    uint16_t getIX();
    uint16_t getIY();
    uint16_t getBC();
    uint16_t getDE();
    uint16_t getHL();
    void setIX(uint16_t v);
    void setIY(uint16_t v);
    void setBC(uint16_t v);
    void setDE(uint16_t v);
    void setHL(uint16_t v);
    void setSP(uint8_t msb, uint8_t lsb);
    void setPC(uint8_t msb, uint8_t lsb);
    void init_registers();
    void setFlag(FLAG_BITS f);
    void clearFlag(FLAG_BITS f);
    uint16_t getIndexReg(INDEX_REG r);
    void setIndexReg(INDEX_REG r, uint16_t v);
    void decode_main_instruction();
    void decode_misc_instruction();
    void decode_bit_instruction();
    void decode_index_instruction();
    void decode_index_bit_instruction();
    void execute_main_opcode();
    void execute_misc_opcode();
    void execute_bit_opcode();
    void execute_index_opcode();
    void execute_ix_iy_bit_opcode();
};
#endif  // SRC_Z80_H_
