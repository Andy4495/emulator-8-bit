/* Z80 Emulator 
   https://github.com/Andy4495/emulator-8-bit
   MIT License

   0.1  11/29/22  Andy4495  Initial Creation
   0.2  12/22/22  Andy4495  Additional implementation
   v0.1.0    02/11/23  Andy4495 Read for first "release"
*/

#ifndef Z80_H
#define Z80_H

/* Z80 Core Definitions */
#define MAX_MEMORY 65536
#define MAX_IO 256
#define MAX_INSTR_SIZE 4

#define MAX_FETCHED_LENGTH 9
#define MAX_MNEMONIC_LENGTH 24
#define MAX_TEXT_LENGTH 64

class Z80 {
    public:
        Z80(unsigned short ramstart=0x8000, unsigned short ramend=0xffff, unsigned short romstart=0x0000, unsigned short romend=0x7fff);
        void load_memory(const char* fname);
        void cold_reset();
        void warm_reset();
        void run_from_address(unsigned short addr);
        void fetch_and_decode();
        void execute();
        void print_fetched_instruction();
        void print_registers();
        void print_flags();
        void print_memory(unsigned short start, unsigned short end);
        bool halted();  // If true, the CPU is halted

    private:
        unsigned char memory[MAX_MEMORY] = {0};
        unsigned char in[MAX_IO] = {0};
        unsigned char out[MAX_IO] = {0};
        unsigned int instr_length;
        char mnemonic[MAX_MNEMONIC_LENGTH + 1];
        char instr_string[MAX_TEXT_LENGTH + 1];
        unsigned short _ramstart;
        unsigned short _ramend;
        unsigned short _romstart;
        unsigned short _romend;

        enum INST_TYPE {ADD, ADC, SUB, SBC};
        enum FLAG_BITS { S_BIT = 0x80, Z_BIT = 0x40, X1_BIT = 0x20, H_BIT = 0x10, X2_BIT = 0x08, PV_BIT = 0x04, N_BIT = 0x02, C_BIT = 0x01};
        enum INDEX_REG {IX_REGISTER, IY_REGISTER};

        // State variables
        bool Halt;

        // Main register set
        // Accumulator 
        unsigned char A;
        // Flags
        unsigned char F;
        // General purpose registers
        unsigned char B, C, D, E, H, L;

        // Alernate register set
        // Accumulator 
        unsigned char Aprime;
        // Flags
        unsigned char Fprime;
        // General purpose registers
        unsigned char Bprime, Cprime, Dprime, Eprime, Hprime, Lprime;

        // Special purpose registers
        // Interrupt vector
        unsigned char I;
        // Memory refresh register
        unsigned char R;
        // Index registers, defined as 8-bit to simplifiy implementation
        // Use get/set methods to manipulate as 16-bit 
        unsigned char IXH, IXL, IYH, IYL;
        // Stack pointer
        unsigned short SP;

        // *** Other Registers and States, not directly accessible to the programmer ***
        // Program counter
        unsigned short PC;
        // Temporary storage for Program Counter when instruction was fetched
        // This is needed because Jumps will alter PC value before it is printed
        unsigned short PC_of_Fetch; 
        // Instruction Register
        // This is a one-byte register in the physical Z80, 
        // but define it here as 4-bytes (max instruction size) 
        // for ease of implementation
        unsigned char IR[MAX_INSTR_SIZE]; 
        // Interrupt enable flip-flops
        unsigned char IFF1;
        unsigned char IFF2;
        // Interrupt Mode
        unsigned char INT_MODE;
        char fetched[MAX_FETCHED_LENGTH + 1];

        unsigned char  testFlag(FLAG_BITS f);
        unsigned char get_next_byte();
        void update_C(INST_TYPE t,  unsigned short val1, unsigned short val2);
        void update_P(unsigned char v);
        void update_V(INST_TYPE t, unsigned char val1, unsigned char val2);
        void update_V16(INST_TYPE t, unsigned short val1, unsigned short val2);
        void update_H(INST_TYPE t, unsigned char val1, unsigned char val2);
        void update_Z(unsigned char val);
        void update_S(unsigned char val);
        unsigned short getIX();
        unsigned short getIY();
        unsigned short getBC();
        unsigned short getDE();
        unsigned short getHL();
        void setIX(unsigned short v);
        void setIY(unsigned short v);
        void setBC(unsigned short v);
        void setDE(unsigned short v);
        void setHL(unsigned short v);
        void setSP(unsigned char msb, unsigned char lsb);
        void setPC(unsigned char msb, unsigned char lsb);
        void clear_registers();
        void setFlag(FLAG_BITS f);
        void clearFlag(FLAG_BITS f);
        unsigned short getIndexReg(INDEX_REG r);
        void setIndexReg(INDEX_REG r, unsigned short v);
        void decode_main_instruction();
        void decode_misc_instruction();
        void decode_bit_instruction();
        void decode_ix_instruction();
        void decode_ix_bit_instruction();
        void decode_iy_instruction();
        void decode_iy_bit_instruction();
        void execute_main_opcode();
        void execute_misc_opcode();
        void execute_bit_opcode();
        void execute_index_opcode();
        void execute_ix_iy_bit_opcode();
};
#endif