#ifndef Z80_H
#define Z80_H

/* Z80 Core Definitions */
#define MAX_MEMORY 65536
#define MAX_IO 256
#define MAX_INSTR_SIZE 4

#define MAX_FETCHED_LENGTH 9
#define MAX_MNEMONIC_LENGTH 16
#define MAX_TEXT_LENGTH 64

class Z80 {
    public:
        unsigned char memory[MAX_MEMORY] = {0};
        unsigned char io[MAX_IO] = {0};
        unsigned int instr_length;
        char mnemonic[MAX_MNEMONIC_LENGTH];
        char instr_string[MAX_TEXT_LENGTH];

        enum INST_TYPE {ADD, SUB, COMP, TEST, NONE};
        enum FLAGS_BITS { S_BIT = 0x80, Z_BIT = 0x40, H_BIT = 0x10, PV_BIT = 0x04, N_BIT = 0x02, C_BIT = 0x01};
        struct Flags {  // X1 and X2 are unused by the Z80
            unsigned char S:1, Z:1, X1:1, H:1, X2:1, PV:1, N:1, C:1;
        };

        // Main register set
        // Accumulator 
        unsigned char A;
        // Flags
        Flags F;
        // General purpose registers
        unsigned char B, C, D, E, H, L;

        // Alernate register set
        // Accumulator 
        unsigned char Aprime;
        // Flags
        Flags Fprime;
        // General purpose registers
        unsigned char Bprime, Cprime, Dprime, Eprime, Hprime, Lprime;

        // Special purpose registers
        // Interrupt vector
        unsigned int I;
        // Memory refresh register
        unsigned int R;
        // Index registers
        unsigned short IX, IY;
        // Stack pointer
        unsigned short SP;

        // *** Other Registers and States, not directly accessible to the programmer ***
        // Program counter
        unsigned short PC;
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

        Z80(unsigned short ramstart=0x8000, unsigned short ramend=0xffff, unsigned short romstart=0x0000, unsigned short romend=0x7fff);
        void load_memory(const char* fname);
        void cold_reset();
        void warm_reset();
        void run_from_address(unsigned short addr);
        void fetch_and_decode();
        unsigned char get_next_byte();
        void execute();
        void print_fetched_instruction();
        void update_flags(unsigned char f_list, INST_TYPE t, unsigned char val1, unsigned char val2);
        void update_C(INST_TYPE t,  unsigned char val);
        void update_N(INST_TYPE t);
        void update_PV(INST_TYPE t, unsigned char val1, unsigned char val2);
        void update_H(INST_TYPE t,  unsigned char val, unsigned char val2);
        void update_Z(INST_TYPE t,  unsigned char val);
        void update_S(INST_TYPE t,  unsigned char val);

    private:
        unsigned short _ramstart;
        unsigned short _ramend;
        unsigned short _romstart;
        unsigned short _romend;
        char fetched[MAX_FETCHED_LENGTH];

        void clear_registers();
};
#endif